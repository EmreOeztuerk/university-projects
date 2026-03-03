import os
import sys
import json
import logging
from datetime import datetime
from mqtt.mqtt_wrapper import MQTTWrapper

# Konfiguration
MQTT_BROKER = 'mqttbroker'
MQTT_PORT = 1883
MQTT_BASE_TOPIC = os.environ.get('BUCKET_MQTT_TOPIC', 'bucket/1')
INITIAL_CAPACITY = float(os.environ.get('BUCKET_CAPACITY', '1000'))  # kWh
TICK_TOPIC = "tickgen/tick"

# Logging setup
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

class Bucket:
    def __init__(self, capacity):
        self.capacity = capacity
        self.current_level = 0
        self.current_target = 0  # Aktuelles Target (wird beibehalten bis neues gesetzt wird)
        logger.info(f"Bucket initialized with capacity: {capacity} kWh")
    
    def process_tick(self):
        """
        Verarbeitet einen Zeitschritt basierend auf dem aktuellen Target
        Returns: Tatsächlicher Energiefluss (positiv = Einspeisung, negativ = Verbrauch)
        """
        if self.current_target < 0:  # Laden
            if self.current_level < self.capacity:
                # Lade mit der Target-Rate
                available_capacity = self.capacity - self.current_level
                charge_amount = min(-self.current_target, available_capacity)
                self.current_level += charge_amount
                return -charge_amount  # Negativ, da Energie verbraucht wird
            else:
                return 0  # Speicher ist voll
        
        elif self.current_target > 0:  # Entladen
            if self.current_level > 0:
                # Entlade mit der Target-Rate
                discharge_amount = min(self.current_target, self.current_level)
                self.current_level -= discharge_amount
                return discharge_amount  # Positiv, da Energie eingespeist wird
            else:
                return 0  # Speicher ist leer
        
        return 0  # Kein Target gesetzt
    
    def set_target(self, target):
        """Setzt ein neues Target für Laden/Entladen"""
        self.current_target = target
        logger.info(f"New target set: {target} kW")
    
    def set_capacity(self, new_capacity):
        """Aktualisiert die maximale Kapazität"""
        if new_capacity < 0:
            logger.warning("Negative capacity not allowed")
            return
        
        self.capacity = new_capacity
        # Wenn aktueller Füllstand über neuer Kapazität, anpassen
        if self.current_level > new_capacity:
            self.current_level = new_capacity
        
        logger.info(f"Capacity updated to: {new_capacity} kWh")

def main():
    bucket = Bucket(INITIAL_CAPACITY)
    mqtt = MQTTWrapper(MQTT_BROKER, MQTT_PORT, name='bucket_1')

    def on_target(client, userdata, msg):
        """Callback für Target-Nachrichten"""
        try:
            target = float(msg.payload.decode('utf-8'))
            bucket.set_target(target)
            
            # Status veröffentlichen
            status = {
                'timestamp': datetime.now().isoformat(),
                'current_level': bucket.current_level,
                'capacity': bucket.capacity,
                'target': bucket.current_target
            }
            
            client.publish(f"{MQTT_BASE_TOPIC}/status", json.dumps(status))
            logger.info(f"New target set: {target}")
        except ValueError as e:
            logger.error(f"Invalid target value received: {msg.payload}")

    def on_tick(client, userdata, msg):
        """Callback für Tick-Nachrichten"""
        try:
            power = bucket.process_tick()
            
            # Status veröffentlichen
            status = {
                'timestamp': msg.payload.decode('utf-8'),  # Verwende Timestamp vom Tick
                'current_level': bucket.current_level,
                'capacity': bucket.capacity,
                'target': bucket.current_target,
                'power': power  # Aktuelle Leistung (positiv = Einspeisung, negativ = Verbrauch)
            }
            
            client.publish(f"{MQTT_BASE_TOPIC}/status", json.dumps(status))
            client.publish(f"{MQTT_BASE_TOPIC}/power", json.dumps({"power": power}))
            
            logger.info(f"Processed tick. Power: {power} kW, Level: {bucket.current_level}/{bucket.capacity} kWh")
        except Exception as e:
            logger.error(f"Error processing tick: {e}")

    def on_set_capacity(client, userdata, msg):
        """Callback für Kapazitätsänderungen"""
        try:
            new_capacity = float(msg.payload.decode('utf-8'))
            bucket.set_capacity(new_capacity)
            
            # Status nach Kapazitätsänderung veröffentlichen
            status = {
                'timestamp': datetime.now().isoformat(),
                'current_level': bucket.current_level,
                'capacity': bucket.capacity,
                'target': bucket.current_target,
                'power': 0
            }
            
            client.publish(f"{MQTT_BASE_TOPIC}/status", json.dumps(status))
        except ValueError as e:
            logger.error(f"Invalid capacity value received: {msg.payload}")

    # MQTT Subscriptions
    mqtt.subscribe(TICK_TOPIC)
    mqtt.subscribe(f"{MQTT_BASE_TOPIC}/target")
    mqtt.subscribe(f"{MQTT_BASE_TOPIC}/set_capacity")
    mqtt.subscribe_with_callback(TICK_TOPIC, on_tick)
    mqtt.subscribe_with_callback(f"{MQTT_BASE_TOPIC}/target", on_target)
    mqtt.subscribe_with_callback(f"{MQTT_BASE_TOPIC}/set_capacity", on_set_capacity)
    
    # Initial status published
    status = {
        'timestamp': datetime.now().isoformat(),
        'current_level': bucket.current_level,
        'capacity': bucket.capacity,
        'target': bucket.current_target,
        'power': 0
    }
    mqtt.publish(f"{MQTT_BASE_TOPIC}/status", json.dumps(status))
    
    try:
        logger.info(f"Bucket storage running. Base topic: {MQTT_BASE_TOPIC}")
        mqtt.loop_forever()
    except KeyboardInterrupt:
        mqtt.stop()
        sys.exit("Bucket storage stopped.")

if __name__ == '__main__':
    main() 