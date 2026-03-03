#!/usr/bin/env python3
import os
import json
import logging
from datetime import datetime
from typing import Dict, Optional, List
import paho.mqtt.client as mqtt
from grid_monitor import GridMonitor
from auction_state import AuctionState

import logging
import paho.mqtt.client as mqtt

# MQTT Debug-Logging aktivieren
logging.basicConfig(level=logging.DEBUG)

# Konfiguration
CLUSTER_ID = os.getenv('CLUSTER_ID', 'cluster_0')
CLUSTER_PORTS = int(os.getenv('CLUSTER_PORTS', '4'))
CLUSTER_POWER = float(os.getenv('CLUSTER_POWER', '44'))
BUFFER_CAPACITY = float(os.getenv('BUFFER_CAPACITY', '100'))
MQTT_BROKER = os.getenv('MQTT_BROKER', 'mqttbroker')
MQTT_PORT = int(os.getenv('MQTT_PORT', '1883'))

# Logging
logging.basicConfig(level=logging.INFO)


class ChargingCluster:
    """
    Verteiltes Ladecluster mit Holländischen Auktionen, Adaptive Mode und intelligenter Buffer-Nutzung
    """

    def __init__(self):
        self.cluster_id = CLUSTER_ID
        self.total_ports = CLUSTER_PORTS
        self.total_power = CLUSTER_POWER
        self.buffer_capacity = BUFFER_CAPACITY
        self.buffer_level = BUFFER_CAPACITY * 0.8

        # Adaptive Mode Integration
        self.adaptive_mode = True  # Default: koordiniert (Auktionen)
        self.waiting_vehicles = []  # Queue für unkoordiniertes Verhalten

        # Port management
        self.ports = {}
        for i in range(self.total_ports):
            self.ports[f"port_{i}"] = {
                'occupied': False,
                'vehicle_id': None,
                'power': 0.0
            }

        # Auktions-System
        self.active_auctions: Dict[str, AuctionState] = {}
        self.grid_monitor = GridMonitor()
        self.base_price = 0.5  # 0.5€/kWh Basispreis

        # Energy Tracking für Buffer-Effizienz
        self.energy_from_grid_direct = 0.0  # kWh direkt Netz→Fahrzeug
        self.energy_from_buffer = 0.0  # kWh Buffer→Fahrzeug
        self.energy_to_buffer = 0.0  # kWh Netz→Buffer (proaktiv)

        # Buffer-Konfiguration
        self.buffer_charge_threshold = 5.0  # Grid balance threshold für proaktives Laden
        self.buffer_charge_rate = 10.0  # kW - maximale Buffer-Laderate

        # Zeitdelta für Energieberechnungen (5 Minuten pro Tick)
        self.time_delta_hours = 5.0 / 60.0  # 5 Minuten in Stunden

        # MQTT Setup mit Last Will
        import uuid
        client_id = f"cluster_{self.cluster_id}_{uuid.uuid4().hex[:8]}"
        self.client = mqtt.Client(client_id)

        self.client.will_set(
            f"auction/cluster_{self.cluster_id}/status",
            json.dumps({"status": "offline", "cluster_id": self.cluster_id}),
            qos=1, retain=True
        )
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message

        self.logger = logging.getLogger(f"cluster_{self.cluster_id}")
        self.logger.info(f"🏭 Cluster {self.cluster_id} initialized with {self.total_ports} ports, {self.total_power}kW")

    def on_connect(self, client, userdata, flags, rc):
        """MQTT Verbindung hergestellt"""
        if rc == 0:
            self.logger.info(f"📡 Cluster {self.cluster_id} connected to MQTT")

            # Subscriptions für verteiltes Auktionssystem
            client.subscribe("tickgen/tick")
            client.subscribe(f"auction/cluster_{self.cluster_id}/bid")
            client.subscribe("system/grid/balance")
            client.subscribe(f"charging/cluster/{self.cluster_id}/release")

            # Adaptive Mode Subscription
            client.subscribe("mgmt/adaptive_mode")

            # Status als online markieren
            client.publish(
                f"auction/cluster_{self.cluster_id}/status",
                json.dumps({"status": "online", "cluster_id": self.cluster_id}),
                qos=1, retain=True
            )
        else:
            self.logger.error(f"❌ Failed to connect to MQTT, rc={rc}")

    def on_message(self, client, userdata, msg):
        """MQTT Nachrichten verarbeiten"""
        try:
            if "tickgen/tick" in msg.topic:
                self.handle_tick(msg)
            elif f"auction/cluster_{self.cluster_id}/bid" in msg.topic:
                self.handle_bid(msg)
            elif "system/grid/balance" in msg.topic:
                self.handle_grid_update(msg)
            elif "release" in msg.topic:
                self.handle_release_request(msg)
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
                mode_str = "koordiniert (Auktionen)" if adaptive_mode else "unkoordiniert (direkt)"
                self.logger.info(f"🔄 Adaptive Mode geändert zu: {mode_str}")

                if not adaptive_mode:
                    # Alle aktiven Auktionen beenden
                    self.cancel_all_auctions()
                    # Warteschlange für direkte Zuweisungen leeren
                    self.waiting_vehicles.clear()

        except Exception as e:
            self.logger.error(f"💥 Error handling adaptive mode: {e}")

    def cancel_all_auctions(self):
        """Beendet alle aktiven Auktionen"""
        for auction_id in list(self.active_auctions.keys()):
            self.logger.info(f"❌ Cancelling auction for {auction_id}")
            del self.active_auctions[auction_id]

    def handle_tick(self, msg):
        """Verarbeitet Simulation Tick"""
        try:
            simulation_time_str = msg.payload.decode('utf-8')
            simulation_time = datetime.fromisoformat(simulation_time_str)

            if self.adaptive_mode:
                # Koordiniertes Verhalten: Auktionen
                self.update_auctions()
                self.start_new_auctions()
            else:
                # Unkoordiniertes Verhalten: Direkte Zuweisung
                self.process_direct_assignments()

            # Buffer simulation mit intelligenter Steuerung
            self.update_buffer()

            # Debug-Output für extreme Situationen
            if self.buffer_level < 5.0 and sum(port['power'] for port in self.ports.values()) > 0:
                self.logger.warning(f"⚠️ Low buffer during charging: {self.buffer_level:.1f}kWh remaining")

            if self.grid_monitor.get_balance() > self.buffer_charge_threshold and self.buffer_level >= self.buffer_capacity * 0.95:
                self.logger.info(f"✅ Buffer full during grid surplus - optimal state reached")

            # Status publizieren
            self.publish_status()

        except Exception as e:
            self.logger.error(f"💥 Error handling tick: {e}")

    def process_direct_assignments(self):
        """Verarbeitet direkte Port-Zuweisungen im unkoordinierten Modus"""
        available_ports = self.get_available_port_ids()

        # Fahrzeuge aus Warteschlange zu verfügbaren Ports zuweisen
        while self.waiting_vehicles and available_ports:
            vehicle_id = self.waiting_vehicles.pop(0)
            port_id = available_ports.pop(0)

            self.assign_port(port_id, vehicle_id)

            # Erfolgreiche direkte Zuweisung melden
            response = {
                'vehicle_id': vehicle_id,
                'cluster_id': self.cluster_id,
                'port_id': port_id,
                'success': True,
                'direct_assignment': True,
                'price': 0.3  # Fester günstiger Preis für unkoordiniert
            }

            self.client.publish(
                f"auction/cluster_{self.cluster_id}/result",
                json.dumps(response)
            )

            self.logger.info(f"🔌 Direct assignment: {vehicle_id} → {port_id}")

    def handle_bid(self, msg):
        """Verarbeitet eingehende Gebote"""
        try:
            data = json.loads(msg.payload.decode('utf-8'))
            vehicle_id = data['vehicle_id']

            if not self.adaptive_mode:
                # Unkoordinierter Modus: Fahrzeug in Warteschlange
                if vehicle_id not in self.waiting_vehicles:
                    self.waiting_vehicles.append(vehicle_id)
                    self.logger.info(f"🚗 Vehicle {vehicle_id} added to waiting queue (uncoordinated mode)")
                return

            # Koordinierter Modus: Normale Auktionslogik
            port_id = data['port_id']
            bid_amount = float(data['bid_amount'])

            if port_id in self.active_auctions:
                auction = self.active_auctions[port_id]

                if auction.accept_bid(vehicle_id, bid_amount):
                    # Gebot erfolgreich - Port zuweisen
                    self.assign_port(port_id, vehicle_id)

                    # Erfolgsmeldung senden
                    response = {
                        'vehicle_id': vehicle_id,
                        'cluster_id': self.cluster_id,
                        'port_id': port_id,
                        'success': True,
                        'winning_bid': bid_amount
                    }

                    self.client.publish(
                        f"auction/cluster_{self.cluster_id}/result",
                        json.dumps(response)
                    )

                    # Auktion beenden
                    del self.active_auctions[port_id]

        except Exception as e:
            self.logger.error(f"💥 Error handling bid: {e}")

    def handle_grid_update(self, msg):
        """Verarbeitet Grid-Balance Updates"""
        try:
            data = json.loads(msg.payload.decode('utf-8'))
            self.grid_monitor.update_balance(data)
        except Exception as e:
            self.logger.error(f"💥 Error handling grid update: {e}")

    def handle_release_request(self, msg):
        """Verarbeitet Port-Freigabe"""
        try:
            data = json.loads(msg.payload.decode('utf-8'))
            vehicle_id = data['vehicle_id']

            for port_id, port in self.ports.items():
                if port['vehicle_id'] == vehicle_id:
                    port['occupied'] = False
                    port['vehicle_id'] = None
                    port['power'] = 0.0
                    self.logger.info(f"🔌 Released {port_id} from vehicle {vehicle_id}")
                    break
        except Exception as e:
            self.logger.error(f"💥 Error handling release: {e}")

    def calculate_start_price(self) -> float:
        """Berechnet Startpreis basierend auf Netzlast und Auslastung"""
        available_ports = self.get_available_ports()
        total_ports = len(self.ports)

        # Auslastungsfaktor (0.0 - 2.0)
        utilization_factor = 1.0 + (total_ports - available_ports) / total_ports

        # Netzlast-Faktor
        grid_factor = 1.0
        if self.grid_monitor.is_grid_stressed():
            grid_factor = 1.5  # 50% Aufschlag bei Netzstress
        elif self.grid_monitor.get_balance() > 0:
            grid_factor = 0.8  # 20% Rabatt bei Überschuss

        start_price = self.base_price * utilization_factor * grid_factor
        return min(3.0, max(0.30, start_price))  # Grenzen einhalten

    def get_available_ports(self) -> int:
        """Gibt Anzahl verfügbarer Ports zurück"""
        return sum(1 for port in self.ports.values() if not port['occupied'])

    def get_available_port_ids(self) -> List[str]:
        """Gibt Liste verfügbarer Port-IDs zurück"""
        return [port_id for port_id, port in self.ports.items() if not port['occupied']]

    def start_new_auctions(self):
        """Startet neue Auktionen für verfügbare Ports (nur im koordinierten Modus)"""
        if not self.adaptive_mode:
            return

        available_ports = [
            port_id for port_id, port in self.ports.items()
            if not port['occupied'] and port_id not in self.active_auctions
        ]

        # Nur Auktionen starten wenn genug Puffer vorhanden
        if self.buffer_level < 10:
            return

        for port_id in available_ports:
            start_price = self.calculate_start_price()
            auction = AuctionState(port_id, self.cluster_id, start_price)
            self.active_auctions[port_id] = auction

            # Auktionsangebot publizieren
            offer = {
                'cluster_id': self.cluster_id,
                'port_id': port_id,
                'current_price': start_price,
                'ticks_remaining': 5,
                'adaptive_mode': self.adaptive_mode
            }

            self.client.publish(
                f"auction/cluster_{self.cluster_id}/offer",
                json.dumps(offer)
            )

    def update_auctions(self):
        """Aktualisiert alle aktiven Auktionen (nur im koordinierten Modus)"""
        if not self.adaptive_mode:
            return

        available_ports = self.get_available_ports()
        price_factor = self.grid_monitor.get_price_adjustment_factor(available_ports)

        expired_auctions = []

        for port_id, auction in self.active_auctions.items():
            auction.tick(price_factor)

            if auction.is_expired():
                expired_auctions.append(port_id)
            else:
                # Preisupdate publizieren
                offer = {
                    'cluster_id': self.cluster_id,
                    'port_id': port_id,
                    'current_price': auction.current_price,
                    'ticks_remaining': auction.ticks_remaining,
                    'adaptive_mode': self.adaptive_mode
                }

                self.client.publish(
                    f"auction/cluster_{self.cluster_id}/offer",
                    json.dumps(offer)
                )

        # Abgelaufene Auktionen entfernen
        for port_id in expired_auctions:
            del self.active_auctions[port_id]

    def assign_port(self, port_id: str, vehicle_id: str):
        """Weist Port einem Fahrzeug zu"""
        if port_id in self.ports:
            self.ports[port_id]['occupied'] = True
            self.ports[port_id]['vehicle_id'] = vehicle_id
            self.ports[port_id]['power'] = 3.7

    def update_buffer(self):
        """Simuliert intelligenten Pufferspeicher mit proaktivem Laden"""
        current_vehicle_power = sum(port['power'] for port in self.ports.values())

        # Proaktives Buffer-Laden wenn Netz entspannt ist
        if self.grid_monitor.get_balance() > self.buffer_charge_threshold:
            if self.buffer_level < self.buffer_capacity:
                # Verfügbare Ladekapazität für Buffer berechnen
                available_buffer_space = self.buffer_capacity - self.buffer_level
                charge_power = min(self.buffer_charge_rate, available_buffer_space / self.time_delta_hours)

                # Buffer laden und Energie tracken
                energy_charged = charge_power * self.time_delta_hours
                self.buffer_level = min(self.buffer_capacity, self.buffer_level + energy_charged)
                self.energy_to_buffer += energy_charged

                self.logger.debug(f"🔋 Proactive buffer charging: +{energy_charged:.2f}kWh (grid surplus)")

        # Fahrzeuge versorgen - intelligent zwischen Buffer und Netz aufteilen
        if current_vehicle_power > 0:
            total_energy_needed = current_vehicle_power * self.time_delta_hours

            # Erst versuchen aus Buffer zu bedienen
            available_buffer_energy = self.buffer_level
            energy_from_buffer = min(total_energy_needed, available_buffer_energy)

            if energy_from_buffer > 0:
                self.buffer_level -= energy_from_buffer
                self.energy_from_buffer += energy_from_buffer

                # Rest aus Netz
                energy_from_grid = total_energy_needed - energy_from_buffer
                self.energy_from_grid_direct += energy_from_grid

                self.logger.debug(
                    f"⚡ Mixed supply: {energy_from_buffer:.2f}kWh buffer + {energy_from_grid:.2f}kWh grid")
            else:
                # Komplett aus Netz (Buffer leer)
                self.energy_from_grid_direct += total_energy_needed
                self.logger.debug(f"⚡ Grid only: {total_energy_needed:.2f}kWh (buffer empty)")

        # Buffer natürlicher Verlust (sehr gering)
        if self.buffer_level > 0:
            loss_rate = 0.001  # 0.1% pro Tick
            energy_loss = self.buffer_level * loss_rate
            self.buffer_level = max(0, self.buffer_level - energy_loss)

    def publish_energy_metrics(self):
        """Publiziert detaillierte Energie-Metriken"""
        total_vehicle_energy = self.energy_from_buffer + self.energy_from_grid_direct

        # Buffer-Effizienz berechnen
        if total_vehicle_energy > 0:
            buffer_utilization = self.energy_from_buffer / total_vehicle_energy
            peak_shaving_benefit = self.energy_from_buffer  # Energie die nicht direkt aus Netz kam
        else:
            buffer_utilization = 0.0
            peak_shaving_benefit = 0.0

        # Grid Impact Score (je höher desto besser für Netz)
        net_grid_impact = self.energy_to_buffer - self.energy_from_grid_direct

        metrics = {
            "cluster_id": self.cluster_id,
            "timestamp": datetime.now().isoformat(),

            # Energie-Flows
            "energy_from_grid_direct": round(self.energy_from_grid_direct, 3),
            "energy_from_buffer": round(self.energy_from_buffer, 3),
            "energy_to_buffer": round(self.energy_to_buffer, 3),

            # Effizienz-Metriken
            "buffer_utilization": round(buffer_utilization, 3),
            "peak_shaving_benefit": round(peak_shaving_benefit, 3),
            "net_grid_impact": round(net_grid_impact, 3),

            # Buffer-Status
            "current_buffer_level": round(self.buffer_level, 2),
            "buffer_fill_percentage": round((self.buffer_level / self.buffer_capacity) * 100, 1),

            # Grid-Kontext
            "grid_balance": self.grid_monitor.get_balance(),
            "is_proactive_charging": self.grid_monitor.get_balance() > self.buffer_charge_threshold,

            # Zusätzliche Metriken
            "total_vehicle_energy": round(total_vehicle_energy, 3),
            "current_vehicle_power": round(sum(port['power'] for port in self.ports.values()), 2)
        }

        self.client.publish(
            f"energy_metrics/cluster_{self.cluster_id}/stats",
            json.dumps(metrics)
        )

        self.logger.debug(
            f"📊 Energy metrics: {buffer_utilization * 100:.1f}% from buffer, grid impact: {net_grid_impact:+.2f}kWh")

    def reset_energy_metrics(self):
        """Setzt Energy-Metriken zurück (z.B. täglich)"""
        self.energy_from_grid_direct = 0.0
        self.energy_from_buffer = 0.0
        self.energy_to_buffer = 0.0
        self.logger.info(f"🔄 Energy metrics reset for {self.cluster_id}")

    def publish_status(self):
        """Publiziert Cluster-Status"""
        occupied_ports = sum(1 for port in self.ports.values() if port['occupied'])
        available_ports = self.get_available_ports()
        current_power_usage = sum(port['power'] for port in self.ports.values())

        status = {
            'cluster_id': self.cluster_id,
            'timestamp': datetime.now().isoformat(),
            'available_ports': available_ports,
            'occupied_ports': occupied_ports,
            'active_auctions': len(self.active_auctions) if self.adaptive_mode else 0,
            'waiting_vehicles': len(self.waiting_vehicles),
            'current_power_usage': current_power_usage,
            'available_power': self.total_power - current_power_usage,
            'buffer_level': self.buffer_level,
            'buffer_capacity': self.buffer_capacity,
            'grid_balance': self.grid_monitor.get_balance(),
            'adaptive_mode': self.adaptive_mode,
            'operation_mode': 'coordinated' if self.adaptive_mode else 'uncoordinated',

            # Neue Energy Tracking Felder
            'proactive_charging_active': self.grid_monitor.get_balance() > self.buffer_charge_threshold,
            'buffer_efficiency': round(
                self.energy_from_buffer / (self.energy_from_buffer + self.energy_from_grid_direct), 3) if (
                                                                                                                      self.energy_from_buffer + self.energy_from_grid_direct) > 0 else 0
        }

        self.client.publish(
            f"cluster/{self.cluster_id}/status",
            json.dumps(status)
        )

        # Zusätzlich detaillierte Energy Metrics publishen (alle 5 Ticks)
        if hasattr(self, 'metrics_counter'):
            self.metrics_counter += 1
        else:
            self.metrics_counter = 1

        if self.metrics_counter % 5 == 0:  # Alle 5 Ticks = alle 25 Minuten
            self.publish_energy_metrics()

    def run(self):
        """Startet das Cluster"""
        try:
            self.client.connect(MQTT_BROKER, MQTT_PORT, 60)
            self.client.loop_start()
            self.logger.info(f"🚀 Cluster {self.cluster_id} started with adaptive mode and energy tracking!")

            # Blocking loop - reagiert nur auf MQTT Messages
            self.client.loop_forever()

        except KeyboardInterrupt:
            self.logger.info(f"🛑 Cluster {self.cluster_id} shutting down")
        except Exception as e:
            self.logger.error(f"💥 Cluster error: {e}")
        finally:
            self.client.loop_stop()
            self.client.disconnect()


if __name__ == "__main__":
    cluster = ChargingCluster()
    cluster.run()