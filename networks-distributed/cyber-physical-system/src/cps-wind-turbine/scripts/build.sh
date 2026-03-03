#!/bin/bash

# Stop and remove existing container if it exists
echo "Cleaning up existing container..."
docker stop wind-turbine 2>/dev/null || true
docker rm wind-turbine 2>/dev/null || true
sleep 1

# Build the wind turbine Docker image
echo "Building Docker image..."
docker build -t cps-wind-turbine:0.1 -f ../Dockerfile ..

# Run the wind turbine container
echo "Starting new container..."
docker run -d \
    --name wind-turbine \
    --network cps-net \
    -p 1883:1883 \
    -e MQTT_BROKER=mqttbroker \
    -e MQTT_PORT=1883 \
    cps-wind-turbine:0.1 