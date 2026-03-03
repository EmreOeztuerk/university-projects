#!/bin/bash

# Subscribe to wind turbine MQTT topics
mosquitto_sub -h mqttbroker -t "windturbine/1/power" -v 