#!/bin/bash

# Run the bucket container
echo "Starting bucket storage..."
docker run -d \
    --name bucket_1 \
    --network cps-net \
    -e BUCKET_MQTT_TOPIC="bucket/1" \
    -e BUCKET_CAPACITY="1000" \
    bucket:0.1 