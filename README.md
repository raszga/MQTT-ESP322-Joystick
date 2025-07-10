This Arduino-based IoT system uses an ESP32 to connect seamlessly to a Wi-Fi network and publish sensor data via MQTT.
It's designed for real-time monitoring of analog and digital sensors, making it ideal for robotics, smart environments, and remote diagnostics.
The code features continuous 12-bit analog readings, intelligent Wi-Fi reconnection logic, and modular components for maintainability.
Digital inputs are set up with internal pull-ups, making them resilient and noise-tolerant. 
Whether you're collecting environmental data, tracking motion, or logging system diagnostics, this project provides a robust foundation.
# 🤖 IoT Robo Sensor with MQTT & Analog Reading (Arduino ESP32)

A lightweight IoT solution built with ESP32 that connects to Wi-Fi, reads analog and digital sensors, and publishes data over MQTT. Perfect for robotics, automation, and edge sensor data collection.

---

## 🧰 Features

- 📡 **MQTT Communication**: Seamless data publishing to brokers
- 🔌 **Wi-Fi Setup**: Auto-reconnect for resilient networking
- 📊 **Analog & Digital Sensor Input**: Continuous 12-bit resolution readings
- ⚙️ **Modular Code**: Clean separation of concerns with custom headers

---

## 🛠️ Hardware Requirements

| Component     | Description                      |
|--------------|----------------------------------|
| ESP32 Board   | Microcontroller with Wi-Fi       |
| Analog Sensors| Any sensor outputting 0-3.3V     |
| Digital Sensors| Pull-up configured input pins   |
| Wi-Fi Network | Access credentials required      |

---
