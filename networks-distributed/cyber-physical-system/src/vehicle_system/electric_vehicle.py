#!/usr/bin/env python3
import logging
import random
import json
from datetime import datetime, time
from typing import Dict, Optional, Tuple
import paho.mqtt.client as mqtt
import logging
import paho.mqtt.client as mqtt

# MQTT Debug-Logging aktivieren
logging.basicConfig(level=logging.DEBUG)

# Zusätzlich MQTT-Nachrichten loggen


class ElectricVehicle:
    """
    Elektrofahrzeug mit intelligenter Auktions-Bietstrategie und Adaptive Mode Support
    """

    def __init__(self, vehicle_id: str, capacity: float, initial_soc: float = 0.3, mqtt_client: mqtt.Client = None):
        self.vehicle_id = vehicle_id
        self.capacity = capacity  # kWh
        self.current_soc = initial_soc  # State of Charge (0-1)
        self.target_soc = 0.8  # 80% Ladeziel
        self.deadline = self._generate_deadline()

        # Adaptive Mode Integration
        self.adaptive_mode = True  # Default: koordiniert (Auktionen)

        # Budget-System
        self.daily_budget = 100.0  # 100€ pro Tag (ein Ladevorgang)
        self.current_budget = 0.0  # Wird basierend auf Deadline berechnet

        # Auktions-Management
        self.active_auction: Optional[str] = None  # cluster_id der aktiven Auktion
        self.active_port: Optional[str] = None
        self.auction_timeout = 0  # Ticks seit letztem Auktionsangebot
        self.max_auction_timeout = 5  # 5 Ticks Timeout

        # Cluster-Suche
        self.available_clusters = set()
        self.blacklisted_clusters = set()  # Cluster die nicht geantwortet haben

        # Charging state
        self.charging_power = 0.0
        self.current_cluster: Optional[str] = None
        self.is_connected = False
        self.charging_start_time: Optional[datetime] = None

        # Physical characteristics
        self.max_charging_power = 11.0  # kW
        self.charging_efficiency = 0.92  # 92%

        # MQTT Client (shared)
        self.client = mqtt_client

        self.logger = logging.getLogger(f"ev_{vehicle_id}")
        self.logger.info(f"🚗 Vehicle {vehicle_id} created: {capacity}kWh, SOC: {initial_soc * 100:.1f}%")

    def set_adaptive_mode(self, adaptive_mode: bool):
        """
        Setzt den Adaptive Mode für das Fahrzeug

        Args:
            adaptive_mode: True für koordiniert, False für unkoordiniert
        """
        old_mode = self.adaptive_mode
        self.adaptive_mode = adaptive_mode

        if old_mode != adaptive_mode:
            mode_str = "koordiniert (Auktionen)" if adaptive_mode else "unkoordiniert (direkt)"
            self.logger.info(f"🔄 Vehicle {self.vehicle_id} mode changed to: {mode_str}")

            if not adaptive_mode:
                # Bei Wechsel zu unkoordiniert: Aktive Auktionen abbrechen
                self.reset_auction_state()

    def _generate_deadline(self) -> time:
        """Generiert zufällige Deadline zwischen 16:00-22:00"""
        hour = random.randint(16, 22)
        minute = random.randint(0, 59)
        return time(hour=hour, minute=minute)

    def calculate_current_budget(self, simulation_time: datetime) -> float:
        """
        Berechnet aktuelles Budget basierend auf Deadline-Nähe

        Args:
            simulation_time: Aktuelle Simulationszeit

        Returns:
            Verfügbares Budget in Euro
        """
        current_time = simulation_time.time()

        # Zeitspanne bis Deadline in Minuten
        current_minutes = current_time.hour * 60 + current_time.minute
        deadline_minutes = self.deadline.hour * 60 + self.deadline.minute

        # Falls Deadline bereits überschritten (nächster Tag)
        if deadline_minutes <= current_minutes:
            deadline_minutes += 24 * 60

        time_until_deadline = deadline_minutes - current_minutes

        # Budget steigt linear zur Deadline
        # 6 Stunden vor Deadline = ~25% Budget
        # 1 Stunde vor Deadline = ~85% Budget
        # 0 Stunden vor Deadline = 100% Budget
        max_time = 6 * 60  # 6 Stunden in Minuten
        time_factor = max(0.0, min(1.0, (max_time - time_until_deadline) / max_time))

        budget = self.daily_budget * (0.2 + 0.8 * time_factor)  # 20% bis 100%

        return min(budget, self.daily_budget)

    def needs_charging(self, simulation_time: datetime) -> Tuple[bool, str]:
        """
        Prüft ob Laden erforderlich ist

        Args:
            simulation_time: Aktuelle Simulationszeit

        Returns:
            (needs_charging, reason)
        """
        if self.current_soc >= self.target_soc:
            return False, "Target SOC reached"

        if self.is_connected:
            return False, "Already charging"

        current_time = simulation_time.time()

        # Zeitkritisch wenn weniger als 2 Stunden bis Deadline
        time_until_deadline = self._time_until_deadline(current_time)
        time_critical = time_until_deadline < 120  # 2 Stunden in Minuten

        if time_critical:
            return True, "Time critical charging needed"
        elif self.current_soc < 0.5:
            return True, "Low SOC charging needed"
        else:
            return True, "Regular charging needed"

    def _time_until_deadline(self, current_time: time) -> int:
        """Berechnet Minuten bis Deadline"""
        current_minutes = current_time.hour * 60 + current_time.minute
        deadline_minutes = self.deadline.hour * 60 + self.deadline.minute

        if deadline_minutes <= current_minutes:
            deadline_minutes += 24 * 60

        return deadline_minutes - current_minutes

    def evaluate_auction_offer(self, cluster_id: str, port_id: str, price: float, simulation_time: datetime) -> bool:
        """
        Bewertet Auktionsangebot und entscheidet ob geboten werden soll (nur koordinierter Modus)

        Args:
            cluster_id: Cluster ID
            port_id: Port ID
            price: Aktueller Auktionspreis
            simulation_time: Aktuelle Simulationszeit

        Returns:
            True wenn geboten werden soll
        """
        # Nur in koordiniertem Modus Auktionen evaluieren
        if not self.adaptive_mode:
            return False

        if cluster_id in self.blacklisted_clusters:
            return False

        if self.active_auction is not None:
            return False  # Bereits in anderer Auktion aktiv

        current_budget = self.calculate_current_budget(simulation_time)

        # Energiebedarf berechnen
        energy_needed = (self.target_soc - self.current_soc) * self.capacity
        total_cost = price * energy_needed

        # Sofort bieten wenn Preis unter Budget
        can_afford = total_cost <= current_budget

        if can_afford:
            self.logger.info(f"💰 Can afford {cluster_id}:{port_id} at {price:.2f}€/kWh (budget: {current_budget:.2f}€)")
            return True
        else:
            self.logger.debug(f"💸 Too expensive: {total_cost:.2f}€ > {current_budget:.2f}€ budget")
            return False

    def place_bid(self, cluster_id: str, port_id: str, price: float) -> None:
        """
        Platziert Gebot bei Cluster (nur koordinierter Modus)

        Args:
            cluster_id: Cluster ID
            port_id: Port ID
            price: Gebotener Preis
        """
        if not self.adaptive_mode:
            return

        if not self.client:
            self.logger.error("❌ No MQTT client available for bidding")
            return

        bid = {
            'vehicle_id': self.vehicle_id,
            'port_id': port_id,
            'bid_amount': price,
            'timestamp': datetime.now().isoformat(),
            'adaptive_mode': self.adaptive_mode
        }

        self.client.publish(
            f"auction/cluster_{cluster_id}/bid",
            json.dumps(bid)
        )

        # Auktionsstatus setzen
        self.active_auction = cluster_id
        self.active_port = port_id
        self.auction_timeout = 0

        self.logger.info(f"🎯 Placed bid for {cluster_id}:{port_id} at {price:.2f}€/kWh")

    def request_uncoordinated_charging(self, cluster_id: str) -> None:
        """
        Sendet direkte Ladeanfrage im unkoordinierten Modus

        Args:
            cluster_id: Cluster ID
        """
        if self.adaptive_mode:
            return  # Nur im unkoordinierten Modus

        if not self.client:
            self.logger.error("❌ No MQTT client available for uncoordinated request")
            return

        # Dummy-Bid für unkoordinierten Modus (wird von Cluster in Warteschlange eingereiht)
        request = {
            'vehicle_id': self.vehicle_id,
            'port_id': 'any_available',
            'bid_amount': 0.3,  # Fester niedriger Preis
            'timestamp': datetime.now().isoformat(),
            'adaptive_mode': self.adaptive_mode,
            'uncoordinated_request': True
        }

        self.client.publish(
            f"auction/cluster_{cluster_id}/bid",
            json.dumps(request)
        )

        # Pseudo-Auktionsstatus für Timeout-Behandlung
        self.active_auction = cluster_id
        self.active_port = 'direct_request'
        self.auction_timeout = 0

        self.logger.info(f"🎲 Uncoordinated charging request to {cluster_id}")

    def handle_auction_result(self, result_data: dict) -> None:
        """
        Verarbeitet Auktionsergebnis

        Args:
            result_data: Dict mit Auktionsergebnis
        """
        if result_data.get('vehicle_id') != self.vehicle_id:
            return

        if result_data.get('success', False):
            # Auktion gewonnen oder direkte Zuweisung erhalten
            self.current_cluster = result_data['cluster_id']
            self.is_connected = True
            self.charging_power = 3.7  # Standard-Ladeleistung
            self.active_auction = None
            self.active_port = None

            if result_data.get('direct_assignment', False):
                self.logger.info(
                    f"🔌 Direct assignment to {self.current_cluster} at {result_data.get('price', 0):.2f}€/kWh")
            else:
                self.logger.info(
                    f"🏆 Won auction for {self.current_cluster} with {result_data.get('winning_bid', 0):.2f}€/kWh")
        else:
            # Auktion verloren
            self.reset_auction_state()
            self.logger.info(f"❌ Lost auction for {self.active_auction}")

    def reset_auction_state(self) -> None:
        """Setzt Auktionsstatus zurück"""
        if self.active_auction:
            self.blacklisted_clusters.add(self.active_auction)

        self.active_auction = None
        self.active_port = None
        self.auction_timeout = 0

    def handle_timeout(self) -> None:
        """Behandelt Auktions-Timeout"""
        if self.active_auction:
            self.logger.warning(f"⏰ Auction timeout for {self.active_auction}")
            self.reset_auction_state()

    def find_new_cluster(self) -> Optional[str]:
        """
        Sucht neuen verfügbaren Cluster

        Returns:
            Cluster ID oder None
        """
        available = self.available_clusters - self.blacklisted_clusters

        if available:
            # Zufälligen verfügbaren Cluster wählen
            chosen = random.choice(list(available))
            self.logger.debug(f"🔍 Found new cluster: {chosen}")
            return chosen
        else:
            # Blacklist zurücksetzen wenn keine Cluster verfügbar
            self.blacklisted_clusters.clear()
            self.logger.warning("🔄 Reset cluster blacklist - trying all again")
            return None

    def update_available_clusters(self, cluster_id: str) -> None:
        """
        Aktualisiert Liste verfügbarer Cluster

        Args:
            cluster_id: Verfügbarer Cluster
        """
        self.available_clusters.add(cluster_id)

    def tick(self, simulation_time: datetime) -> None:
        """
        Verarbeitet Simulation-Tick

        Args:
            simulation_time: Aktuelle Simulationszeit
        """
        # Budget aktualisieren
        self.current_budget = self.calculate_current_budget(simulation_time)

        # Auktions-Timeout prüfen
        if self.active_auction:
            self.auction_timeout += 1
            if self.auction_timeout >= self.max_auction_timeout:
                self.handle_timeout()

        # Deadline prüfen
        self.check_deadline(simulation_time)

        # Ladeprozess simulieren falls verbunden
        if self.is_connected and self.charging_power > 0:
            self.process_charging(simulation_time)

    def check_deadline(self, simulation_time: datetime) -> bool:
        """
        Prüft Deadline und setzt tägliche Ziele zurück

        Args:
            simulation_time: Aktuelle Simulationszeit

        Returns:
            True wenn Deadline noch nicht erreicht
        """
        current_time = simulation_time.time()

        if current_time >= self.deadline:
            # Deadline erreicht - neuen Tag starten
            goal_met = self.current_soc >= self.target_soc

            if goal_met:
                self.logger.info(f"✅ Daily goal reached: {self.current_soc * 100:.1f}%")
            else:
                self.logger.warning(
                    f"❌ Missed daily goal: {self.current_soc * 100:.1f}% < {self.target_soc * 100:.1f}%")

            # Neuen Tag vorbereiten
            self.deadline = self._generate_deadline()
            self.target_soc = random.uniform(0.7, 0.9)  # Neues Ziel
            self.blacklisted_clusters.clear()  # Cluster-Blacklist zurücksetzen

            return True

        return True

    def process_charging(self, simulation_time: datetime) -> None:
        """
        Simuliert Ladeprozess

        Args:
            simulation_time: Aktuelle Simulationszeit
        """
        if not self.charging_start_time:
            self.charging_start_time = simulation_time

        # Realistische Ladekurve
        actual_power = self.calculate_charging_power(self.charging_power)

        # Energie pro Tick (angenommen: 1 Tick = 5 Minuten)
        time_delta_hours = 5.0 / 60.0  # 5 Minuten in Stunden
        energy_delta = actual_power * time_delta_hours * self.charging_efficiency

        old_soc = self.current_soc
        self.current_soc = min(1.0, self.current_soc + (energy_delta / self.capacity))

        if old_soc != self.current_soc:
            self.logger.debug(f"🔋 Charging: {old_soc * 100:.1f}% → {self.current_soc * 100:.1f}%")

        # Laden beenden wenn Ziel erreicht
        if self.current_soc >= self.target_soc:
            self.disconnect_from_cluster()

    def calculate_charging_power(self, requested_power: float) -> float:
        """
        Berechnet tatsächliche Ladeleistung basierend auf SOC

        Args:
            requested_power: Angeforderte Leistung

        Returns:
            Tatsächliche Ladeleistung
        """
        if self.current_soc >= 0.8:
            # Ladeleistung reduziert sich über 80%
            max_power = self.max_charging_power * (1 - (self.current_soc - 0.8) * 2)
        else:
            max_power = self.max_charging_power

        return min(requested_power, max_power, self.max_charging_power)

    def disconnect_from_cluster(self) -> None:
        """Trennt Verbindung zum Cluster"""
        if self.current_cluster and self.client:
            # Release-Nachricht senden
            release_msg = {
                'vehicle_id': self.vehicle_id,
                'timestamp': datetime.now().isoformat()
            }

            self.client.publish(
                f"charging/cluster/{self.current_cluster}/release",
                json.dumps(release_msg)
            )

            self.logger.info(f"🔌 Disconnected from {self.current_cluster}")

        self.current_cluster = None
        self.is_connected = False
        self.charging_power = 0.0
        self.charging_start_time = None

    def calculate_required_energy(self) -> float:
        """
        Berechnet benötigte Energie bis Ziel-SOC

        Returns:
            Benötigte Energie in kWh
        """
        return (self.target_soc - self.current_soc) * self.capacity

    def get_status(self) -> dict:
        """
        Gibt aktuellen Fahrzeugstatus zurück

        Returns:
            Dict mit Fahrzeugstatus
        """
        return {
            'vehicle_id': self.vehicle_id,
            'current_soc': self.current_soc,
            'target_soc': self.target_soc,
            'current_budget': self.current_budget,
            'daily_budget': self.daily_budget,
            'is_connected': self.is_connected,
            'current_cluster': self.current_cluster,
            'active_auction': self.active_auction,
            'charging_power': self.charging_power,
            'deadline': self.deadline.strftime('%H:%M'),
            'required_energy': self.calculate_required_energy(),
            'adaptive_mode': self.adaptive_mode,
            'operation_mode': 'coordinated' if self.adaptive_mode else 'uncoordinated'
        }