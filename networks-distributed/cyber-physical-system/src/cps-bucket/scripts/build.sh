#!/bin/bash

# Stop and remove existing container if it exists
echo "Cleaning up existing container..."
docker stop bucket_1 2>/dev/null || true
docker rm bucket_1 2>/dev/null || true

# Build the bucket Docker image
echo "Building Docker image..."
BASE_DIR="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )/../src/"
docker build ${BASE_DIR}bucket -t bucket:0.1 