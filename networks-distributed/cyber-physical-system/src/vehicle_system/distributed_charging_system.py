#!/usr/bin/env python3
import os
import json
import logging
import random
from datetime import datetime
from typing import Dict, List
import paho.mqtt.client as mqtt
from electric_vehicle import ElectricVehicle
import logging
import paho.mqtt.client as mqtt

# MQTT Debug-Logging aktivieren

logging.basicConfig(level=logging.DEBUG)

# Zusätzlich MQTT-Nachrichten loggen


class DistributedChargingSystem:
    """
    Verteiltes Ladesystem mit Adaptive Mode Support
    Koordiniert vs. Unkoordiniert basierend auf mgmt/adaptive_mode
    """

    def __init__(self):
        # Konfiguration
        self.num_vehicles = int(os.environ.get('NUM_VEHICLES', '50'))
        self.mqtt_broker = os.environ.get('MQTT_BROKER', 'mqttbroker')
        self.mqtt_port = int(os.environ.get('MQTT_PORT', '1883'))

        # Logging
        log_level = os.environ.get('LOG_LEVEL', 'INFO')
        logging.basicConfig(level=getattr(logging, log_level))
        self.logger = logging.getLogger("distributed_charging_system")

        # Adaptive Mode Integration
        self.adaptive_mode = True  # Default: koordiniert (Auktionen)

        # System-Zustand
        self.vehicles: Dict[str, ElectricVehicle] = {}
        self.cluster_status: Dict[str, dict] = {}
        self.current_simulation_time: datetime = datetime.now()

        # MQTT Client
        import uuid
        client_id = f"distributed_charging_system_{uuid.uuid4().hex[:8]}"
        self.client = mqtt.Client(client_id)

        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message

        self.logger.info(f"🚀 Distributed Charging System initialized with {self.num_vehicles} vehicles")

    def on_connect(self, client, userdata, flags, rc):
        """MQTT Verbindung hergestellt"""
        if rc == 0:
            self.logger.info("📡 Connected to MQTT broker")

            # Subscriptions für verteiltes System
            client.subscribe("tickgen/tick")
            client.subscribe("auction/+/offer")  # Alle Cluster-Angebote
            client.subscribe("auction/+/result")  # Alle Auktionsergebnisse
            client.subscribe("cluster/+/status")  # Cluster-Status

            # Adaptive Mode Subscription
            client.subscribe("mgmt/adaptive_mode")

        else:
            self.logger.error(f"❌ Failed to connect to MQTT, rc={rc}")

    def on_message(self, client, userdata, msg):
        """Verarbeitet MQTT Nachrichten"""
        try:
            if "tickgen/tick" in msg.topic:
                self.handle_tick(msg)
            elif "auction/" in msg.topic and "/offer" in msg.topic:
                self.handle_auction_offer(msg)
            elif "auction/" in msg.topic and "/result" in msg.topic:
                self.handle_auction_result(msg)
            elif "cluster/" in msg.topic and "/status" in msg.topic:
                self.handle_cluster_status(msg)
            elif "mgmt/adaptive_mode" in msg.topic:
                self.handle_adaptive_mode(msg)
        except Exception as e:
            self.logger.error(f"💥 Error processing message: {e}")

    def handle_adaptive_mode(self, msg):
        """Verarbeitet Adaptive Mode Umschaltung"""
        try:
            adaptive_mode = msg.payload.decode('utf-8').lower() == 'true'
            old_mode = self.adaptive_mode
            self.adaptive_mode = adaptive_mode

            if old_mode != adaptive_mode:
                mode_str = "koordiniert (Auktionen)" if adaptive_mode else "unkoordiniert (zufällig)"
                self.logger.info(f"🔄 System Adaptive Mode geändert zu: {mode_str}")

                # Allen Fahrzeugen den neuen Modus mitteilen
                for vehicle in self.vehicles.values():
                    vehicle.set_adaptive_mode(adaptive_mode)

        except Exception as e:
            self.logger.error(f"💥 Error handling adaptive mode: {e}")

    def handle_tick(self, msg):
        """Verarbeitet Simulation Tick"""
        try:
            simulation_time_str = msg.payload.decode('utf-8')
            self.current_simulation_time = datetime.fromisoformat(simulation_time_str)

            # Alle Fahrzeuge aktualisieren
            for vehicle in self.vehicles.values():
                vehicle.tick(self.current_simulation_time)

            # Fahrzeuge management basierend auf Adaptive Mode
            if self.adaptive_mode:
                self.manage_coordinated_charging()
            else:
                self.manage_uncoordinated_charging()

            # System-Status publizieren (alle 10 Ticks)
            if hasattr(self, 'tick_counter'):
                self.tick_counter += 1
            else:
                self.tick_counter = 1

            if self.tick_counter % 10 == 0:
                self.publish_system_status()

        except Exception as e:
            self.logger.error(f"💥 Error handling tick: {e}")

    def manage_coordinated_charging(self):
        """Verwaltet koordiniertes Laden via Auktionen"""
        charging_vehicles = 0
        waiting_vehicles = 0
        completed_vehicles = 0

        for vehicle in self.vehicles.values():
            needs_charging, reason = vehicle.needs_charging(self.current_simulation_time)

            if vehicle.is_connected:
                charging_vehicles += 1
            elif needs_charging:
                waiting_vehicles += 1

                # Fahrzeug sucht neuen Cluster wenn nicht in Auktion
                if not vehicle.active_auction:
                    available_cluster = vehicle.find_new_cluster()
                    if available_cluster:
                        self.logger.debug(f"🔍 {vehicle.vehicle_id} searching for charging in {available_cluster}")
            else:
                completed_vehicles += 1

        # Statistiken loggen (alle 50 Ticks)
        if hasattr(self, 'tick_counter') and self.tick_counter % 50 == 0:
            self.logger.info(
                f"📊 Coordinated: {charging_vehicles} charging, {waiting_vehicles} waiting, {completed_vehicles} completed")

    def manage_uncoordinated_charging(self):
        """Verwaltet unkoordiniertes Laden durch zufällige Cluster-Auswahl"""
        charging_vehicles = 0
        waiting_vehicles = 0
        completed_vehicles = 0

        # Verfügbare Cluster sammeln
        available_clusters = [
            cluster_id for cluster_id, status in self.cluster_status.items()
            if status.get('available_ports', 0) > 0
        ]

        for vehicle in self.vehicles.values():
            needs_charging, reason = vehicle.needs_charging(self.current_simulation_time)

            if vehicle.is_connected:
                charging_vehicles += 1
            elif needs_charging and not vehicle.active_auction:
                waiting_vehicles += 1

                # Zufällige Cluster-Auswahl im unkoordinierten Modus
                if available_clusters and random.random() < 0.1:  # 10% Chance pro Tick
                    chosen_cluster = random.choice(available_clusters)
                    vehicle.request_uncoordinated_charging(chosen_cluster)
                    self.logger.debug(f"🎲 {vehicle.vehicle_id} randomly requesting {chosen_cluster}")
            else:
                completed_vehicles += 1

        # Statistiken loggen (alle 50 Ticks)
        if hasattr(self, 'tick_counter') and self.tick_counter % 50 == 0:
            self.logger.info(
                f"📊 Uncoordinated: {charging_vehicles} charging, {waiting_vehicles} waiting, {completed_vehicles} completed")

    def handle_auction_offer(self, msg):
        """Verarbeitet Auktionsangebote von Clustern"""
        try:
            data = json.loads(msg.payload.decode('utf-8'))
            cluster_id = data['cluster_id']
            port_id = data['port_id']
            price = data['current_price']

            # Nur verarbeiten wenn im koordinierten Modus
            if not self.adaptive_mode:
                return

            # Allen interessierten Fahrzeugen das Angebot zeigen
            for vehicle in self.vehicles.values():
                if not vehicle.is_connected and not vehicle.active_auction:
                    needs_charging, reason = vehicle.needs_charging(self.current_simulation_time)

                    if needs_charging:
                        # Fahrzeug verfügbare Cluster aktualisieren
                        vehicle.update_available_clusters(cluster_id)

                        # Prüfen ob Fahrzeug bieten möchte
                        if vehicle.evaluate_auction_offer(cluster_id, port_id, price, self.current_simulation_time):
                            vehicle.place_bid(cluster_id, port_id, price)
                            break  # Nur ein Fahrzeug pro Angebot

        except Exception as e:
            self.logger.error(f"💥 Error handling auction offer: {e}")

    def handle_auction_result(self, msg):
        """Verarbeitet Auktionsergebnisse"""
        try:
            data = json.loads(msg.payload.decode('utf-8'))
            vehicle_id = data.get('vehicle_id')

            if vehicle_id in self.vehicles:
                self.vehicles[vehicle_id].handle_auction_result(data)

        except Exception as e:
            self.logger.error(f"💥 Error handling auction result: {e}")

    def handle_cluster_status(self, msg):
        """Verarbeitet Cluster-Status Updates"""
        try:
            data = json.loads(msg.payload.decode('utf-8'))
            cluster_id = data['cluster_id']
            self.cluster_status[cluster_id] = data

        except Exception as e:
            self.logger.error(f"💥 Error handling cluster status: {e}")

    def create_vehicles(self):
        """Erstellt Fahrzeugflotte"""
        self.logger.info(f"🚗 Creating {self.num_vehicles} vehicles")

        for i in range(self.num_vehicles):
            vehicle_id = f"ev_{i}"
            capacity = random.uniform(40, 100)  # 40-100 kWh
            initial_soc = random.uniform(0.1, 0.6)  # 10-60% Start-SOC

            vehicle = ElectricVehicle(
                vehicle_id=vehicle_id,
                capacity=capacity,
                initial_soc=initial_soc,
                mqtt_client=self.client
            )

            # Adaptive Mode setzen
            vehicle.set_adaptive_mode(self.adaptive_mode)

            self.vehicles[vehicle_id] = vehicle

        self.logger.info(f"✅ Created {len(self.vehicles)} vehicles with adaptive mode support")

    def publish_system_status(self):
        """Publiziert Gesamtsystem-Status"""
        charging_count = sum(1 for v in self.vehicles.values() if v.is_connected)
        waiting_count = sum(1 for v in self.vehicles.values()
                            if v.needs_charging(self.current_simulation_time)[0] and not v.is_connected)
        auction_count = sum(1 for v in self.vehicles.values() if v.active_auction)

        avg_soc = sum(v.current_soc for v in self.vehicles.values()) / len(self.vehicles)
        avg_budget = sum(v.current_budget for v in self.vehicles.values()) / len(self.vehicles)

        # Cluster-Statistiken
        total_auctions = sum(s.get('active_auctions', 0) for s in self.cluster_status.values())
        total_waiting = sum(s.get('waiting_vehicles', 0) for s in self.cluster_status.values())

        status = {
            'timestamp': self.current_simulation_time.isoformat(),
            'adaptive_mode': self.adaptive_mode,
            'operation_mode': 'coordinated' if self.adaptive_mode else 'uncoordinated',
            'vehicles': {
                'total': len(self.vehicles),
                'charging': charging_count,
                'waiting': waiting_count,
                'in_auction': auction_count if self.adaptive_mode else 0,
                'avg_soc': round(avg_soc * 100, 1),
                'avg_budget': round(avg_budget, 2)
            },
            'clusters': {
                'available': len(self.cluster_status),
                'total_auctions': total_auctions if self.adaptive_mode else 0,
                'total_waiting': total_waiting if not self.adaptive_mode else 0
            },
            'system_type': 'distributed_auction_based' if self.adaptive_mode else 'distributed_random'
        }

        self.client.publish("vehicle/collected/status", json.dumps(status))

    def get_system_metrics(self) -> dict:
        """Gibt detaillierte System-Metriken zurück"""
        metrics = {
            'vehicles': {},
            'clusters': dict(self.cluster_status),
            'summary': {
                'total_vehicles': len(self.vehicles),
                'active_clusters': len(self.cluster_status),
                'simulation_time': self.current_simulation_time.isoformat()
            }
        }

        # Fahrzeug-Details
        for vehicle_id, vehicle in self.vehicles.items():
            metrics['vehicles'][vehicle_id] = vehicle.get_status()

        return metrics

    def run(self):
        """Startet das verteilte Ladesystem"""
        self.logger.info("🚀 Starting Distributed Charging System")

        try:
            # MQTT verbinden
            self.client.connect(self.mqtt_broker, self.mqtt_port, 60)
            self.client.loop_start()

            # Fahrzeuge erstellen
            self.create_vehicles()

            self.logger.info("✅ Distributed Charging System running - waiting for ticks and auctions")

            # Event-driven loop - reagiert nur auf MQTT
            self.client.loop_forever()

        except KeyboardInterrupt:
            self.logger.info("🛑 Shutting down Distributed Charging System")
        except Exception as e:
            self.logger.error(f"💥 System error: {e}")
        finally:
            # Alle Fahrzeuge disconnecten
            for vehicle in self.vehicles.values():
                if vehicle.is_connected:
                    vehicle.disconnect_from_cluster()

            self.client.loop_stop()
            self.client.disconnect()


if __name__ == "__main__":
    system = DistributedChargingSystem()
    system.run()