#!/usr/bin/env python3
import sys
import json
import logging
from datetime import datetime
from mqtt.mqtt_wrapper import MQTTWrapper

# Configure logging
logging.basicConfig(level=logging.DEBUG)
logger = logging.getLogger(__name__)

# MQTT topics
TICK_TOPIC = "tickgen/tick"
GRID_BALANCE_TOPIC = "system/grid/balance"


class GridCalculationService:
    def __init__(self):
        """Initialize the grid calculation service"""
        # Storage for current values
        self.wind_power = 0.0  # Current wind generation in kW
        self.cluster_consumption = {}  # Dict: cluster_id -> consumption
        self.consumer_consumption = {}  # Dict: consumer_id -> consumption

        # Track last update timestamp
        self.last_calculation = None

        logger.info("Grid Calculation Service initialized")

    def update_wind_power(self, power_value):
        """Update wind power generation"""
        self.wind_power = float(power_value)
        logger.debug(f"Wind power updated: {self.wind_power:.2f} kW")

    def update_cluster_consumption(self, cluster_id, consumption):
        """Update consumption for a specific cluster"""
        self.cluster_consumption[cluster_id] = float(consumption)
        logger.debug(f"Cluster {cluster_id} consumption updated: {consumption:.2f} kW")

    def update_consumer_consumption(self, consumer_id, consumption):
        """Update consumption for a specific consumer"""
        self.consumer_consumption[consumer_id] = float(consumption)
        logger.debug(f"Consumer {consumer_id} consumption updated: {consumption:.2f} kW")

    def calculate_grid_balance(self, timestamp):
        """Calculate grid balance: Generation - Consumption"""
        # Total generation (currently only wind)
        total_generation = self.wind_power

        # Total consumption from clusters
        cluster_total = sum(self.cluster_consumption.values())

        # Total consumption from consumers
        consumer_total = sum(self.consumer_consumption.values())

        # Total consumption
        total_consumption = cluster_total + consumer_total

        # Grid balance: positive = surplus, negative = deficit
        balance = total_generation - total_consumption

        logger.info(f"Grid Balance: {balance:.2f} kW (Gen: {total_generation:.2f}, Cons: {total_consumption:.2f})")

        # Create balance data
        balance_data = {
            "balance": round(balance, 2),
            "total_generation": round(total_generation, 2),
            "total_consumption": round(total_consumption, 2),
            "cluster_consumption": round(cluster_total, 2),
            "consumer_consumption": round(consumer_total, 2),
            "timestamp": timestamp
        }

        return balance_data


def on_message_tick(client, userdata, msg):
    """Handle tick messages and trigger grid balance calculation"""
    global grid_service

    # Extract timestamp from tick
    timestamp = msg.payload.decode("utf-8")

    # Calculate and publish grid balance
    try:
        balance_data = grid_service.calculate_grid_balance(timestamp)

        # Publish to grid balance topic
        client.publish(GRID_BALANCE_TOPIC, json.dumps(balance_data))
        logger.debug(f"Published grid balance: {balance_data['balance']:.2f} kW")

    except Exception as e:
        logger.error(f"Error calculating grid balance: {e}")


def on_message_windturbine(client, userdata, msg):
    """Handle wind turbine power messages"""
    global grid_service

    try:
        data = json.loads(msg.payload.decode("utf-8"))
        power = data.get("payload", 0.0)
        grid_service.update_wind_power(power)
    except Exception as e:
        logger.error(f"Error processing wind turbine message: {e}")


def on_message_cluster(client, userdata, msg):
    """Handle cluster status messages"""
    global grid_service

    try:
        # Extract cluster_id from topic (cluster/cluster_X/status)
        topic_parts = msg.topic.split('/')
        cluster_id = topic_parts[1]  # cluster_0, cluster_1, etc.

        # Parse cluster status
        data = json.loads(msg.payload.decode("utf-8"))
        consumption = data.get("current_power_usage", 0.0)

        grid_service.update_cluster_consumption(cluster_id, consumption)

    except Exception as e:
        logger.error(f"Error processing cluster message from {msg.topic}: {e}")


def on_message_consumer(client, userdata, msg):
    """Handle consumer demand messages"""
    global grid_service

    try:
        # Extract consumer info from topic
        topic_parts = msg.topic.split('/')
        consumer_type = topic_parts[1]  # housingcomplex, g0, g1, g5, etc.

        # Parse consumer data
        data = json.loads(msg.payload.decode("utf-8"))
        demand = data.get("payload", 0.0)

        grid_service.update_consumer_consumption(consumer_type, demand)

    except Exception as e:
        logger.error(f"Error processing consumer message from {msg.topic}: {e}")


def main():
    """Main function to start the grid calculation service"""
    global grid_service

    logger.info("Starting Grid Calculation Service...")

    # Initialize grid calculation service
    grid_service = GridCalculationService()

    # Initialize MQTT client
    mqtt = MQTTWrapper('mqttbroker', 1883, name='grid_calculation_service')

    # Subscribe to tick messages (trigger calculations)
    mqtt.subscribe_with_callback(TICK_TOPIC, on_message_tick)
    logger.info(f"Subscribed to: {TICK_TOPIC}")

    # Subscribe to wind turbine data
    mqtt.subscribe_with_callback("windturbine/+/power", on_message_windturbine)
    logger.info("Subscribed to: windturbine/+/power")

    # Subscribe to all cluster status messages
    mqtt.subscribe_with_callback("cluster/+/status", on_message_cluster)
    logger.info("Subscribed to: cluster/+/status")

    # Subscribe to consumer demand messages
    mqtt.subscribe_with_callback("consumer/+/+/demand", on_message_consumer)
    logger.info("Subscribed to: consumer/+/demand")

    try:
        logger.info("Starting MQTT loop for grid calculation...")
        mqtt.loop_forever()
    except (KeyboardInterrupt, SystemExit):
        mqtt.stop()
        logger.info("Grid Calculation Service stopped gracefully")
        sys.exit(0)


if __name__ == '__main__':
    main()