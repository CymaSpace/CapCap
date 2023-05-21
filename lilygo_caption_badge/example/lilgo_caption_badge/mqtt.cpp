#include "mqtt.h"
#include <WiFi.h>
#include <cstring>

const char* mqtt_server = "192.168.1.132";
const char* caption_topic = "caption_text";

WiFiClient espClient;
PubSubClient client(espClient);

String received_text; // Variable to store the received caption text

void callback(char* topic, byte* payload, unsigned int length) {
  // Convert payload to string
  char message[length + 1];
  strncpy(message, (char*)payload, length);
  message[length] = '\0';

  // Append the received sentence to the accumulated text
  received_text += message;

  // Add a newline character for new sentences
  received_text += "\n";

  // Set the label text on your screen
  lv_label_set_text(mqtt_label, received_text.c_str());
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
