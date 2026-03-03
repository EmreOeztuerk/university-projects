import sys
import json
import logging
from datetime import datetime
import pandas as pd
from mqtt.mqtt_wrapper import MQTTWrapper

# Configure logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

# MQTT topic for publishing wind turbine data
POWER_TOPIC = "windturbine/1/power"
# MQTT topic for receiving tick messages
TICK_TOPIC = "tickgen/tick"

class WindDataIterator:
    def __init__(self, power_values):
        """Initialize with the list of power values"""
        self.power_values = power_values
        self.current_index = 0
        self.total_values = len(power_values)
        logger.info(f"Initialized wind data iterator with {self.total_values} values")
        
    def get_next_power(self):
        """Get the next power value and advance the counter"""
        if self.total_values == 0:
            return 0
            
        value = self.power_values[self.current_index]
        self.current_index = (self.current_index + 1) % self.total_values
        return value

def load_wind_data():
    """
    Load wind energy data from CSV file.
    Returns a WindDataIterator that provides sequential access to power values.
    """
    try:
        # Read the CSV file
        logger.info("Loading wind data from CSV...")
        df = pd.read_csv('/data/wind_energy_output.csv')
        
        # Ensure we have the Power_W column
        if 'Power_W' not in df.columns:
            logger.error("Error: CSV file must contain 'Power_W' column")
            sys.exit(1)
            
        # Convert power from Watts to kilowatts
        power_values = df['Power_W'].div(1000.0).tolist()
        
        # Log some statistics
        power_series = pd.Series(power_values)
        logger.info(f"Loaded wind data with {len(power_values)} entries")
        logger.info(f"Power range: {power_series.min():.2f} kW to {power_series.max():.2f} kW")
        logger.info(f"Average power: {power_series.mean():.2f} kW")
        
        return WindDataIterator(power_values)
    except Exception as e:
        logger.error(f"Error loading wind data: {e}")
        sys.exit(1)

def on_message_tick(client, userdata, msg):
    """
    Callback function that processes messages from the tick generator topic.
    It gets the next power value from the wind data iterator and publishes it.
    """
    global wind_data
    
    # Extract the timestamp from the tick message and decode it from UTF-8
    ts_iso = msg.payload.decode("utf-8")
    
    # Get the next power value
    current_power = wind_data.get_next_power()
    logger.info(f"Publishing power value: {current_power:.2f} kW")

    # Create message payload with timestamp from tick
    data = {
        "payload": float(current_power*3),
        "timestamp": ts_iso
    }
    
    # Publish the data to the wind turbine topic in JSON format
    client.publish(POWER_TOPIC, json.dumps(data))

def main():
    """
    Main function to initialize the MQTT client, set up subscriptions,
    and start the message loop.
    """
    global wind_data
    
    logger.info("Starting wind turbine simulation...")
    
    # Load wind data
    wind_data = load_wind_data()
    
    # Initialize the MQTT client and connect to the broker
    mqtt = MQTTWrapper('mqttbroker', 1883, name='windturbine_1')
    
    # Subscribe to the tick topic
    mqtt.subscribe(TICK_TOPIC)
    # Subscribe with a callback function to handle incoming tick messages
    mqtt.subscribe_with_callback(TICK_TOPIC, on_message_tick)
    
    logger.info(f"Subscribed to tick topic: {TICK_TOPIC}")
    
    try:
        # Start the MQTT loop to process incoming and outgoing messages
        logger.info("Starting MQTT loop...")
        mqtt.loop_forever()
    except (KeyboardInterrupt, SystemExit):
        # Gracefully stop the MQTT client and exit the program on interrupt
        mqtt.stop()
        sys.exit("KeyboardInterrupt -- shutdown gracefully.")

if __name__ == '__main__':
    # Entry point for the script
    main() 