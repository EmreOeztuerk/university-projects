import os
import sys
import json
import random
import logging
from datetime import datetime, date
from mqtt.mqtt_wrapper import MQTTWrapper
from demandlib import bdew  # BDEW-Bibliothek für Standardlastprofile


# Konfiguration aus Umgebungsvariablen
NAME = os.environ.get('EC_NAME')  # z.B. H0 oder G0
PROFILE_TYPE = NAME.lower() if NAME else 'h0'  # BDEW-Profile verwenden Kleinbuchstaben
YEAR = datetime.now().year  # Aktuelles Jahr für die Profile
POPULATION_FACTOR = int(os.environ.get('EC_POPULATION_FACTOR', 1))
BASE_DEMAND = int(os.environ.get('EC_BASE_DEMAND', 1))
ANNUAL_DEMAND = BASE_DEMAND * POPULATION_FACTOR  # 2000 kWh/a pro Person
MQTT_BASE_TOPIC = os.environ.get('EC_MQTT_TOPIC')
TICK_TOPIC = "tickgen/tick"
bdew_profiles = None
scale_factor = 1.0



def initialize_bdew_profiles():
    """
    Initialisiert die BDEW-Profile basierend auf dem angegebenen Typ und Jahr.
    Gibt skalierte Leistungsprofile zurück.
    """
    # BDEW-Profile für das aktuelle Jahr initialisieren
    e_slp = bdew.ElecSlp(year=YEAR)
    
    # Optional: Feiertage hinzufügen, die als Sonntage behandelt werden
    # holidays = {date(YEAR, 1, 1): "Neujahr", date(YEAR, 5, 1): "Tag der Arbeit"}
    # e_slp = bdew.ElecSlp(year=YEAR, holidays=holidays)
    
    # Skalierte Leistungsprofile erstellen (kW statt kWh)
    scaled_power_profiles = e_slp.get_scaled_power_profiles({PROFILE_TYPE: ANNUAL_DEMAND}, conversion_factor=4)
    return scaled_power_profiles

def on_message_scale(client, userdata, msg):
    """Callback-Funktion für Skalierungsfaktor-Nachrichten"""
    global scale_factor
    scale_factor = float(msg.payload.decode("utf-8"))

def on_message_tick(client, userdata, msg):
    """Callback-Funktion für Tick-Nachrichten"""
    global MQTT_BASE_TOPIC
    global bdew_profiles
    global scale_factor

    ts_iso = msg.payload.decode("utf-8")
    ts = datetime.fromisoformat(ts_iso)
    
    # Zeit auf einen 15-Minuten-Zeitraum abrunden
    rounded_time = ts.replace(minute=(ts.minute // 15) * 15, second=0, microsecond=0)
    
    try:
        # Zufällige Abweichung hinzufügen
        offset = random.uniform(-0.05, 0.05)
        
        # Wenn der Zeitpunkt in den Profilen existiert
        if rounded_time in bdew_profiles.index:
            value = bdew_profiles.loc[rounded_time, PROFILE_TYPE] * (1 + offset) * scale_factor
            data = {"payload": value, "timestamp": ts_iso}
            client.publish(MQTT_BASE_TOPIC + "/demand", json.dumps(data))
        else:
            # Fallback für Zeitpunkte außerhalb des vorhandenen Profils
            current_year_time = datetime(YEAR, rounded_time.month, rounded_time.day, 
                                       rounded_time.hour, rounded_time.minute)
            if current_year_time in bdew_profiles.index:
                value = bdew_profiles.loc[current_year_time, PROFILE_TYPE] * (1 + offset) * scale_factor
                data = {"payload": value, "timestamp": ts_iso}
                client.publish(MQTT_BASE_TOPIC + "/demand", json.dumps(data))
            else:
                logging.warning(f"Zeitpunkt {rounded_time} nicht in BDEW-Profilen gefunden")
    except Exception as e:
        logging.error(f"Fehler bei der Verarbeitung des Ticks: {e}")

def main():
    """Hauptfunktion"""
    print(f"MQTT Base Topic: {MQTT_BASE_TOPIC}")
    print(f"Profil-Typ: {PROFILE_TYPE}")
    print(f"Jährlicher Bedarf: {ANNUAL_DEMAND} kWh")

    global bdew_profiles
    bdew_profiles = initialize_bdew_profiles()

    # MQTT-Verbindung einrichten
    mqtt = MQTTWrapper('mqttbroker', 1883, name=NAME)
    mqtt.subscribe(TICK_TOPIC)
    mqtt.subscribe_with_callback(TICK_TOPIC, on_message_tick)
    mqtt.subscribe(MQTT_BASE_TOPIC + "/scale")
    mqtt.subscribe_with_callback(MQTT_BASE_TOPIC + "/scale", on_message_scale)

    try:
        print("Warte auf MQTT-Nachrichten...")
        mqtt.loop_forever()
    except(KeyboardInterrupt, SystemExit):
        mqtt.stop()
        sys.exit("KeyboardInterrupt -- Programm wird sauber beendet.")

if __name__ == '__main__':
    main()
