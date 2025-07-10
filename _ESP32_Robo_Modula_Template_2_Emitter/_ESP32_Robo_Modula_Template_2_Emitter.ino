#include "mqtt_handler.h"
#include "hSensor.h"

const char* ssid = "L89";
const char* password = "01158";

void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected: " + WiFi.localIP().toString());
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  setupMQTT();
  analogContinuousSetWidth(12);  // setuAnalog continous reading 0-4095 on 3.3 V
  analogContinuousSetAtten(ADC_11db);
  analogContinuous(adc_pins, adc_pins_count, CONVERSIONS_PER_PIN, 20000, &adcComplete);
  analogContinuousStart();
  // setup input digital pins as pullup they will be 1 if not connected to ground G
  for (int i = 0; i < digi_pins_count; i++) { pinMode(digi_pins[i], INPUT_PULLUP); }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) setup_wifi();
  if (!client.connected()) reconnect();
  //client.loop();
  publishMessage("ROBO", readAnalogs(), true);
  Serial.println(iS[4]);
  delay(iS[4]);
}

