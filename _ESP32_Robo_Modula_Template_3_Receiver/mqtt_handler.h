#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>
// External client reference
extern PubSubClient client;
extern WiFiClientSecure espClient;

// Broker credentials
extern const char* mqtt_server;
extern const char* mqtt_username;
extern const char* mqtt_password;
extern const int mqtt_port;

// Core MQTT functions
void setupMQTT();
void reconnect();
void publishMessage(const char* topic, String payload, boolean retained);
void callback(char* topic, byte* payload, unsigned int length);

#endif
