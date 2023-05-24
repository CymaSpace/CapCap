#!/usr/bin/env python3
"""
whisper_mqtt_publisher.py

This script reads the output generated by the Whisper application (when running examples/livestream.sh)
and sends one sentence at a time to an MQTT topic named "caption_text".

The script continuously monitors the output file and publishes each sentence to the MQTT broker.
It uses the Eclipse Paho MQTT client library for Python (paho-mqtt) to establish the MQTT connection.

Requirements:
- paho-mqtt library (install with `pip install paho-mqtt`)

Usage:
1. Make sure the Whisper application is running and generating output.
2. Configure the MQTT broker connection parameters (broker address, port, etc.) in the script.
3. Run this script: `python whisper_mqtt_publisher.py`

Note: This script assumes that the Whisper output file is in text format, with one sentence per line.
Adjust the code if the format or file structure differs.
"""
import time
import os
import paho.mqtt.client as mqtt

# MQTT broker connection parameters
MQTT_BROKER = "mqtt.example.com"
MQTT_PORT = 1883
MQTT_TOPIC = "caption_text"

# Path to the Whisper output file
WHISPER_OUTPUT_FILE = "/path/to/whisper_output.txt"

# Function to establish MQTT connection
def connect_mqtt():
    client = mqtt.Client()
    client.connect(MQTT_BROKER, MQTT_PORT)
    return client

# Function to read and publish sentences from the Whisper output file
def publish_sentences(client):
    with open(WHISPER_OUTPUT_FILE, 'r') as f:
        for line in f:
            sentence = line.strip()
            if sentence:
                client.publish(MQTT_TOPIC, sentence)
                time.sleep(1)  # Adjust the delay between publishing sentences

# Main function
def main():
    client = connect_mqtt()
    publish_sentences(client)
    client.disconnect()

if __name__ == '__main__':
    main()


import subprocess
import re
import time
import paho.mqtt.client as mqtt

# Define the MQTT broker details
mqtt_broker = "your_mqtt_broker_ip"
mqtt_port = 1883
mqtt_topic = "caption_text"

# Start the whisper livestream subprocess
livestream_proc = subprocess.Popen(["./livestream.sh"], stdout=subprocess.PIPE, bufsize=1, universal_newlines=True)

# Connect to the MQTT broker
mqtt_client = mqtt.Client()
mqtt_client.connect(mqtt_broker, mqtt_port)

# Loop to read the whisper output and send sentences to MQTT
for line in iter(livestream_proc.stdout.readline, ""):
    # Extract the caption from the whisper output
    match = re.search(r"Caption: (.+)", line)
    if match:
        caption = match.group(1).strip()

        # Publish the caption to the MQTT topic
        mqtt_client.publish(mqtt_topic, caption)
        print("Published to MQTT:", caption)

    # Check if the whisper process has finished
    if livestream_proc.poll() is not None:
        break

    time.sleep(0.1)

# Wait for the whisper process to complete
livestream_proc.wait()

# Disconnect from the MQTT broker
mqtt_client.disconnect()