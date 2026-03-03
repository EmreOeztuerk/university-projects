#!/bin/bash

# Stop and remove existing container if it exists
echo "Cleaning up existing container..."
docker stop calculation-service 2>/dev/null || true
docker rm calculation-service 2>/dev/null || true
sleep 1

# Build the wind turbine Docker image
echo "Building Docker image..."
docker build -t calculation-service:0.1 -f ../Dockerfile ..

# Run the wind turbine container
echo "Starting new container..."
docker run -d \
    --name calculation-service \
    --network cps-net \
    -e MQTT_BROKER=mqttbroker \
    -e MQTT_PORT=1883 \
    calculation-service:0.1