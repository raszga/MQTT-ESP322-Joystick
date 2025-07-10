#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H
#include <Arduino.h>

class ServoController {
private:
  String name;
  int pin;
  int pwmFreq;
  int resolutionBits;
  int maxDuty;
  int ledcChannel;
  int currentAngle;

public:
  ServoController(String servoName, int gpioPin, int frequency = 50, int resolution = 16, int channel = 0)
    : name(servoName), pin(gpioPin), pwmFreq(frequency), resolutionBits(resolution),
      maxDuty((1 << resolution) - 1), ledcChannel(gpioPin), currentAngle(0) {}

  void begin() {
    ledcAttach(pin, pwmFreq, resolutionBits);  // Setup pin directly
    delay(10);                                 // Optional: stabilize servo
    writeAngle(0);                             // Default position}
  }

  void writeAngle(int angle) {
    angle = constrain(angle, 0, 4095);
    int duty = map(angle, 0, 4095, maxDuty * 0.025, maxDuty * 0.125);
    //Serial.print("d=");Serial.println(duty);
    ledcWrite(ledcChannel, duty);
    currentAngle = angle;
  }

  void slowMove(int targetAngle, unsigned delayPerStep = 1, unsigned stepSize = 1) {
    targetAngle = constrain(targetAngle, 0, 4095);
    while (currentAngle != targetAngle) {
      if (currentAngle < targetAngle) currentAngle += stepSize;
      else currentAngle -= stepSize;
      writeAngle(currentAngle);
      delay(delayPerStep);
    }
  }

  int fastMove(int angle) {
    writeAngle(angle);
    return currentAngle;
  }

  int getCurrentAngle() const {
    return currentAngle;
  }

  String getName() const {
    return name;
  }
};

#endif  // SERVO_CONTROLLER_H
