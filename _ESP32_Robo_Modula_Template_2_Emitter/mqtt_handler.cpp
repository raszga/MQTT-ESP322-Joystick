#include "mqtt_handler.h"
#include "hSensor.h"
// MQTT globals
WiFiClientSecure espClient;
PubSubClient client(espClient);

const char* mqtt_server = "0f8d1257e8e8466da2a04a322c11012b.s1.eu.hivemq.cloud";
const char* mqtt_username = "CR22081";
const char* mqtt_password = "Timisoara1900";
const int mqtt_port = 8883;

void setupMQTT() {
  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "CR22081->" + String(idESP);
    Serial.println(clientId);

    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Connected");
      client.subscribe("ROBO");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" -> retrying in 5s");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; ++i) {
    message += (char)payload[i];
  }
  Serial.println("Received: " + message);
}

void publishMessage(const char* topic, String payload, boolean retained) {
  if (!client.publish(topic, payload.c_str(), retained)) {
    Serial.println("Message lost [" + String(topic) + "]: " + payload);
  }
}
