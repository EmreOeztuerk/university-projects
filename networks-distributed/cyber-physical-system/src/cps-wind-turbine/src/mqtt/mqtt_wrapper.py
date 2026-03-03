import paho.mqtt.client as mqtt
import logging
import sys


class MQTTWrapper:
    def __init__(self, broker_ip, broker_port, name='MQTTWrapper'):
        self.broker_ip = broker_ip
        self.broker_port = broker_port
        self.name = name

        # Configure logging
        self.log = logging.getLogger(self.name)
        self.log.setLevel(logging.INFO)
        ch = logging.StreamHandler(sys.stderr)
        ch.setLevel(logging.INFO)
        self.log.addHandler(ch)

        self.client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION1, self.name)
        self.client.on_connect = self.on_connect

        self.client.connect(self.broker_ip, self.broker_port, 60)
        
    def loop_start(self):
        self.client.loop_start()

    def loop_forever(self):
        self.client.loop_forever()

    def publish(self, topic, message):
        self.log.debug('publish ' + str(message) + ' to topic ' + topic)
        self.client.publish(topic, str(message))

    def subscribe(self, topic):
        self.log.debug('subscribe to  ' + topic)
        self.client.subscribe(topic)

    def subscribe_with_callback(self, topic, callback):
        self.client.message_callback_add(topic, callback)
        self.subscribe(topic)

    # The callback for when the client receives a CONNACK response from the server.
    def on_connect(self, client, userdata, flags, rc):
        self.log.info("Connected to " + self.broker_ip + ":" + str(self.broker_port) + " with result code " + str(rc))

    def stop(self):
        self.client.disconnect()
        self.client.loop_stop() 