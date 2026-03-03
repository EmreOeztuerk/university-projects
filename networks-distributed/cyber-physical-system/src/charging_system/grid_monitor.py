#!/usr/bin/env python3
import logging
from typing import Optional


class GridMonitor:
    """
    Überwacht die Netzlast und berechnet Preisanpassungsfaktoren für Auktionen
    """

    def __init__(self):
        self.current_balance: float = 0.0  # Positiv = Überschuss, Negativ = Mangel
        self.logger = logging.getLogger("grid_monitor")

    def update_balance(self, balance_data: dict) -> None:
        """
        Aktualisiert die Netzbalance basierend auf Grid-Daten

        Args:
            balance_data: Dict mit 'balance' Wert (pos/neg)
        """
        try:
            self.current_balance = float(balance_data.get('balance', 0.0))
            self.logger.debug(f"Grid balance updated: {self.current_balance}")
        except (ValueError, TypeError) as e:
            self.logger.error(f"Error updating grid balance: {e}")

    def get_price_adjustment_factor(self, available_ports: int) -> float:
        """
        Berechnet Preisanpassungsfaktor basierend auf Netzlast und verfügbaren Ports

        Args:
            available_ports: Anzahl freier Ports im Cluster

        Returns:
            Faktor für Preisanpassung (1.05 = +5%, 0.95 = -5%)
        """
        # Basis: Keine Anpassung
        factor = 1.0

        if self.current_balance > 0:
            # Überschuss: Preise fallen um 5%
            factor = 0.95
            self.logger.debug("Grid surplus detected - prices falling")

        elif self.current_balance < 0 and available_ports > 0:
            # Mangel + freie Plätze: Preise steigen um 5%
            factor = 1.05
            self.logger.debug("Grid deficit + capacity available - prices rising")

        return factor

    def is_grid_stressed(self) -> bool:
        """
        Prüft ob das Netz unter Stress steht (negativer Balance)

        Returns:
            True wenn Netz gestresst ist
        """
        return self.current_balance < 0

    def get_balance(self) -> float:
        """
        Gibt aktuelle Netzbalance zurück

        Returns:
            Aktuelle Balance (positiv = Überschuss, negativ = Mangel)
        """
        return self.current_balance