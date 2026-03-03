#!/bin/bash

echo "🧹 Cleaning up existing cluster containers..."

# Stop and remove all cluster containers
docker stop cluster_0 cluster_1 cluster_2 cluster_3 cluster_4 cluster_5 cluster_6 cluster_7 cluster_8 cluster_9 2>/dev/null || true
docker rm cluster_0 cluster_1 cluster_2 cluster_3 cluster_4 cluster_5 cluster_6 cluster_7 cluster_8 cluster_9 2>/dev/null || true

echo "🔨 Building Cluster Docker image..."
BASE_DIR="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
docker build $BASE_DIR -f Dockerfile -t charging_cluster:0.1

echo "🚀 Starting charging clusters..."

# Start Cluster 0 - with auto-restart
echo "  ⚡ Starting cluster_0 (4 ports, 44kW)"
docker run -d \
  --name cluster_0 \
  --network cps-net \
  --restart unless-stopped \
  -e MQTT_BROKER="mqttbroker" \
  -e MQTT_PORT="1883" \
  -e CLUSTER_ID="cluster_0" \
  -e CLUSTER_PORTS="100" \
  -e CLUSTER_POWER="440" \
  -e BUFFER_CAPACITY="1500" \
  charging_cluster:0.1


sleep 2

echo ""
echo "✅ All cluster containers started with auto-restart!"
echo "=========================="
echo "📊 Running cluster containers:"
docker ps --format "table {{.Names}}\t{{.Status}}\t{{.Ports}}" | grep cluster_

echo ""
echo "📋 Useful commands:"
echo "  📊 Cluster 0 logs:     docker logs -f cluster_0"
echo "  ⚡ Cluster 5 logs:     docker logs -f cluster_5"
echo "  🔄 Restart stats:      docker stats --no-stream"
echo "  🛑 Stop all clusters:  docker stop cluster_0 cluster_1 cluster_2 cluster_3 cluster_4 cluster_5 cluster_6 cluster_7 cluster_8 cluster_9"
echo "  🧹 Remove all:         docker rm cluster_0 cluster_1 cluster_2 cluster_3 cluster_4 cluster_5 cluster_6 cluster_7 cluster_8 cluster_9"
echo ""
echo "ℹ️  Note: All cluster containers have --restart unless-stopped policy"
echo "   They will automatically restart on crashes until manually stopped"