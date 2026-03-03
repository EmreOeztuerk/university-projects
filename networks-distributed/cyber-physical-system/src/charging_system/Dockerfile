FROM python:3.9-slim

# Set working directory
WORKDIR /app

# Copy cluster-related Python files
COPY charging_cluster.py /app/
COPY auction_state.py /app/
COPY grid_monitor.py /app/

# Set Python path
ENV PYTHONPATH=/app

RUN pip install paho-mqtt==1.6.1

# Default environment variables for cluster
ENV CLUSTER_ID="cluster_0"
ENV CLUSTER_PORTS="4"
ENV CLUSTER_POWER="44"
ENV BUFFER_CAPACITY="100"
ENV MQTT_BROKER="mqttbroker"
ENV MQTT_PORT="1883"

# Start cluster service
CMD ["python", "-u", "charging_cluster.py"]