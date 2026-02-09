# smart-health-monitor-esp32
IoT-based health monitoring system using ESP32, MAX30102, and MLX90614 with real-time data visualization on Blynk.
# Smart Health Monitoring System (ESP32)

IoT-based health monitoring system using ESP32, MAX30102, and MLX90614 to measure heart rate, blood oxygen (SpO₂), and body temperature in real time via the Blynk mobile application.

---

## Overview
This project implements a smart health monitoring system that integrates multiple medical sensors with an ESP32 microcontroller. The system reads vital parameters and sends the data to a smartphone using the Blynk IoT platform.

The system is capable of classifying the user’s health condition automatically based on predefined thresholds.

---
## How to Run
1. Install Arduino IDE.
2. Install ESP32 board package.
3. Install required libraries:
   - MAX3010x
   - Adafruit MLX90614
   - Blynk
4. Open `Code_Health_Monitoring.ino`.
5. Set Wi-Fi and Blynk credentials.
6. Upload to ESP32.
---
## Features
- Real-time body temperature monitoring (non-contact)
- Heart rate (BPM) monitoring
- Blood oxygen level (SpO₂) monitoring
- Automatic health status classification:
  - Healthy
  - Warning
  - Sick / High Fever
- Real-time monitoring via Blynk mobile app

---

## Hardware Components
- ESP32 (main microcontroller)
- MAX30102 (heart rate and SpO₂ sensor)
- MLX90614 (non-contact temperature sensor)
- 4.7kΩ resistor
- Jumper wires

---

## Technologies Used
- Programming Language: C/C++ (Arduino)
- Development Platform: Arduino IDE
- Communication Protocol: I2C
- IoT Platform: Blynk
- Connectivity: Wi-Fi (ESP32)

---

## System Workflow
1. ESP32 initializes the sensors and Wi-Fi connection.
2. MLX90614 measures body temperature.
3. MAX30102 measures heart rate and SpO₂.
4. ESP32 processes the sensor data.
5. The system classifies the health condition.
6. Data is sent to the Blynk app for real-time monitoring.

---

## Test Results

### Normal Condition
- Temperature: 34.37°C
- Heart Rate: 102 BPM
- SpO₂: 93.64%
- Status: Healthy

### Fever Condition
- Temperature: 38.77°C
- Heart Rate: 96 BPM
- SpO₂: 86.85%
- Status: Sick

### High Fever Condition
- Temperature: 42°C
- Heart Rate: 110 BPM
- SpO₂: 94.39%
- Status: High Fever

---

## Project Documentation
### Device
<img width="617" height="345" alt="image" src="https://github.com/user-attachments/assets/2d80dec7-4e09-4744-8886-37474d972cb4" />


### Test Result
<img width="767" height="163" alt="image" src="https://github.com/user-attachments/assets/6132e463-8c45-4587-8863-18e198ca5c0a" />
<img width="760" height="160" alt="image" src="https://github.com/user-attachments/assets/a20edcb1-f2b5-41a2-b89f-338c31931bcb" />
<img width="745" height="164" alt="image" src="https://github.com/user-attachments/assets/0a5e9b2e-0be2-4b4e-93f2-a1a6893412f4" />


---

## Full Report
[Click here to open the final report](Final_report_health_monitoring.pdf)

---

## Author
- Bharata Kukuh Hariyanto
- Maulana Khairil Akbar
- Rifky Fadjar Hidayat  

**Bachelor of Computer Engineering**  
Telkom University Surabaya
