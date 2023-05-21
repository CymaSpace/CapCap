import paho.mqtt.client as mqtt
import time

mqtt_broker = "192.168.1.132"
mqtt_port = 1883
topic = "caption_text"

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT broker")
    else:
        print("Failed to connect to MQTT broker")

def on_publish(client, userdata, mid):
    print("Message published")
    print(mid)

# Create an MQTT client
client = mqtt.Client()
client.on_connect = on_connect
client.on_publish = on_publish

# Connect to the MQTT broker
client.connect(mqtt_broker, mqtt_port)

# Publish three lines of text
lines = [
    "This is the first line.",
    "This is the second line.",
    "This is the third line."
]

for line in lines:
    client.publish(topic, line)
    print("Published line:", line)
    time.sleep(3) # Adjust the delay time as needed

# Disconnect from the MQTT broker
client.disconnect()
