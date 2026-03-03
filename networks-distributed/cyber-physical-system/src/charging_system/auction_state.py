#!/usr/bin/env python3
import logging
from typing import Optional
from datetime import datetime


class AuctionState:
    """
    Verwaltet den Zustand einer einzelnen Holländischen Auktion für einen Ladeport
    """

    def __init__(self, port_id: str, cluster_id: str, start_price: float):
        self.port_id = port_id
        self.cluster_id = cluster_id
        self.start_price = start_price
        self.current_price = start_price
        self.ticks_remaining = 5  # 5 Ticks Timeout
        self.highest_bidder: Optional[str] = None
        self.winning_bid: Optional[float] = None
        self.is_active = True
        self.start_time = datetime.now()

        self.logger = logging.getLogger(f"auction_{cluster_id}_{port_id}")
        self.logger.info(f"🎯 Auction started for {port_id} at {start_price:.2f}€/kWh")

    def update_price(self, price_adjustment_factor: float) -> None:
        """
        Aktualisiert den Auktionspreis basierend auf Netzlast

        Args:
            price_adjustment_factor: Faktor für Preisanpassung (z.B. 0.95 = -5%)
        """
        if not self.is_active:
            return

        old_price = self.current_price
        self.current_price *= price_adjustment_factor

        # Grenzen einhalten
        self.current_price = max(0.01, min(50.0, self.current_price))  # Min 0.50€, Max 50€

        if old_price != self.current_price:
            self.logger.debug(f"💰 Price updated: {old_price:.2f}€ → {self.current_price:.2f}€/kWh")

    def tick(self, price_adjustment_factor: float) -> None:
        """
        Verarbeitet einen Simulation-Tick

        Args:
            price_adjustment_factor: Faktor für Preisanpassung
        """
        if not self.is_active:
            return

        self.ticks_remaining -= 1
        self.update_price(price_adjustment_factor)

        if self.ticks_remaining <= 0:
            self.expire()

    def accept_bid(self, vehicle_id: str, bid_amount: float) -> bool:
        """
        Versucht ein Gebot zu akzeptieren

        Args:
            vehicle_id: ID des bietenden Fahrzeugs
            bid_amount: Gebotene Summe

        Returns:
            True wenn Gebot akzeptiert wurde
        """
        if not self.is_active:
            self.logger.warning(f"❌ Bid rejected - auction inactive")
            return False

        if bid_amount >= self.current_price:
            self.highest_bidder = vehicle_id
            self.winning_bid = bid_amount
            self.is_active = False
            self.logger.info(f"✅ Auction won by {vehicle_id} with {bid_amount:.2f}€/kWh")
            return True
        else:
            self.logger.debug(f"❌ Bid too low: {bid_amount:.2f}€ < {self.current_price:.2f}€")
            return False

    def expire(self) -> None:
        """
        Lässt die Auktion ablaufen
        """
        if self.is_active:
            self.is_active = False
            self.logger.info(f"⏰ Auction expired for {self.port_id} - no bids received")

    def is_expired(self) -> bool:
        """
        Prüft ob die Auktion abgelaufen ist

        Returns:
            True wenn Auktion abgelaufen oder beendet
        """
        return not self.is_active or self.ticks_remaining <= 0

    def get_winner(self) -> Optional[str]:
        """
        Gibt den Gewinner der Auktion zurück

        Returns:
            Vehicle ID des Gewinners oder None
        """
        return self.highest_bidder if not self.is_active and self.highest_bidder else None

    def get_status(self) -> dict:
        """
        Gibt den aktuellen Auktionsstatus zurück

        Returns:
            Dict mit Auktionsstatus
        """
        return {
            'port_id': self.port_id,
            'cluster_id': self.cluster_id,
            'current_price': self.current_price,
            'ticks_remaining': self.ticks_remaining,
            'is_active': self.is_active,
            'winner': self.highest_bidder,
            'winning_bid': self.winning_bid
        }