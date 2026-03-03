FROM python:3.9-slim

# Set working directory
WORKDIR /app

# Copy vehicle-related Python files
COPY distributed_charging_system.py /app/
COPY electric_vehicle.py /app/

# Set Python path
ENV PYTHONPATH=/app

RUN pip install paho-mqtt==1.6.1

# Default environment variables for vehicles
ENV NUM_VEHICLES="30"
ENV MQTT_BROKER="mqttbroker"
ENV MQTT_PORT="1883"
ENV LOG_LEVEL="INFO"

# Start vehicle system
CMD ["python", "-u", "distributed_charging_system.py"]