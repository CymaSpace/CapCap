#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include <lvgl.h>

extern PubSubClient client;
extern lv_obj_t *mqtt_label;

void callback(char* topic, byte* message, unsigned int length);
void mqtt_setup();
void reconnect();

#endif
