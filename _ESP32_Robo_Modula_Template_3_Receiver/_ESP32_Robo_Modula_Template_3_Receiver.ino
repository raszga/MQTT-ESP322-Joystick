#include "mqtt_handler.h"
#include "hSensor.h"
#include "Servo_CR_ESP32.h"

#include <Arduino.h>
#include "ServoController.h"

// Create a ServoController instance for GPIO pin 18
ServoController myServo("SteeringServo", 25, 50, 16);



const char* ssid = "Lin889";
const char* password = "01161358";

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
  myServo.begin();  // Initialize PWM and move to default position
  delay(100);
  myServo.writeAngle(1024);  // Move to mid-range position
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) setup_wifi();
  if (!client.connected()) reconnect();

  if (sMSG.indexOf("*,") != -1) {
    sMSG.remove(0, 2);
    xintJoy = TrS(sMSG);
    //myServo.writeAngle(xintJoy._val[0]);
    myServo.slowMove(xintJoy._val[0],0,1);
    Serial.println(xintJoy._val[0]);
    sMSG = "";
  }
  client.loop();
}
