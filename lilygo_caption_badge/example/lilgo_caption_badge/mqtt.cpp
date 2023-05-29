#include "mqtt.h"
#include "factory_gui.h" // Include this to have access to add_mqtt_message function
#include <WiFi.h>
#include <cstring>

const char* mqtt_server = "192.168.1.132";
const char* caption_topic = "caption_text";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  // Convert payload to string
  char message[length + 1];
  strncpy(message, (char*)payload, length);
  message[length] = '\0';

  // Append the received message using the new function
  add_mqtt_message(message);
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      // Once connected, subscribe to the caption topic
      client.subscribe(caption_topic);
    } else {
      delay(5000); // Wait 5 seconds before retrying connection
    }
  }
}

void mqtt_setup() {
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
