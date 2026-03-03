#!/bin/bash

echo "🧹 Cleaning up existing vehicle containers..."

# Stop and remove all vehicle containers
docker stop vehicle_fleet_1 vehicle_fleet_2 vehicle_fleet_3 2>/dev/null || true
docker rm vehicle_fleet_1 vehicle_fleet_2 vehicle_fleet_3 2>/dev/null || true

echo "🔨 Building Vehicle Docker image..."
BASE_DIR="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
docker build $BASE_DIR -f Dockerfile -t charging_vehicles:0.1

echo "🚗 Starting vehicle fleets..."

# Start Vehicle Fleet 1 (main fleet) - with auto-restart
echo "  🚗 Starting vehicle_fleet_1 (50 vehicles)"
docker run -d \
  --name vehicle_fleet_1 \
  --network cps-net \
  --restart unless-stopped \
  -e NUM_VEHICLES="300" \
  -e MQTT_BROKER="mqttbroker" \
  -e MQTT_PORT="1883" \
  -e LOG_LEVEL="INFO" \
  charging_vehicles:0.1

sleep 2



sleep 2

echo ""
echo "✅ All vehicle fleet containers started with auto-restart!"
echo "=========================="
echo "📊 Running vehicle containers:"
docker ps --format "table {{.Names}}\t{{.Status}}\t{{.Ports}}" | grep vehicle_fleet_

echo ""
echo "🔄 Restart status check:"
docker inspect --format="table {{.Name}}\t{{.HostConfig.RestartPolicy.Name}}" vehicle_fleet_1 vehicle_fleet_2 vehicle_fleet_3

echo ""
echo "📋 Useful commands:"
echo "  📊 Main fleet logs:    docker logs -f vehicle_fleet_1"
echo "  🚗 Fleet 2 logs:       docker logs -f vehicle_fleet_2"
echo "  🔧 Test fleet logs:    docker logs -f vehicle_fleet_3"
echo "  🔄 Vehicle stats:      docker stats vehicle_fleet_1 vehicle_fleet_2 vehicle_fleet_3"
echo "  🛑 Stop all vehicles:  docker stop vehicle_fleet_1 vehicle_fleet_2 vehicle_fleet_3"
echo "  🧹 Remove all:         docker rm vehicle_fleet_1 vehicle_fleet_2 vehicle_fleet_3"
echo ""
echo "ℹ️  Total vehicles: 100 (50 + 30 + 20)"
echo "   All containers have --restart unless-stopped policy"
echo "   They will automatically restart on crashes until manually stopped"