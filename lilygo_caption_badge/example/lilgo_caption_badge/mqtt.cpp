#include "mqtt.h"
#include <WiFi.h>

const char* mqtt_server = "192.168.1.132";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
    char messageTemp[length+1];
    payload[length] = '\0'; // Ensure payload is null-terminated
    strncpy(messageTemp, (char*)payload, length);
    Serial.print("Received message on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    Serial.println(messageTemp);

  // Set the label text on your screen
  lv_label_set_text(mqtt_label, messageTemp);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("state");
    } else {
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void mqtt_setup() {
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
