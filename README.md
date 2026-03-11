# 🌱 Smart Irrigation and Environmental Monitoring System

**An intelligent automated plant irrigation system powered by ESP32**

---

## 📋 Project Overview

This project implements a comprehensive **automated plant irrigation system** using an ESP32 microcontroller. It monitors environmental parameters (soil moisture, temperature, humidity) in real-time and intelligently controls a water pump. The system integrates cloud connectivity via **ThingSpeak**, provides local API control, and features live LCD display for immediate feedback.

**Perfect for**: Indoor gardens, potted plants, small greenhouses, and IoT learning projects.

---

## ✨ Features

- ✅ **Soil Moisture Monitoring** - Real-time analog soil moisture reading
- ✅ **Environmental Sensing** - Temperature & humidity via BME280 sensor
- ✅ **Live LCD Display** - 16x2 I2C LCD shows readings in real-time
- ✅ **Intelligent Pump Control** - Activates water pump based on moisture and environmental conditions
- ✅ **Cloud Integration** - Sends sensor data to ThingSpeak for remote monitoring
- ✅ **Local API Control** - Adjust thresholds remotely via JSON POST requests
- ✅ **Safety Timeout** - Automatic pump shutdown after 2 minutes to prevent overwatering

---

## 🛠️ Hardware Components

| # | Component | Specification | Notes |
|---|-----------|---------------|-------|
| 1 | **ESP32 Microcontroller** | 32-bit dual-core | Main processor |
| 2 | **BME280 Sensor** | I2C interface | Temperature, humidity, pressure |
| 3 | **Soil Moisture Sensor** | Analog output (0-4095) | GPIO 34 |
| 4 | **Water Pump / LED** | GPIO 26 | LED in prototype, relay for real pump |
| 5 | **LCD Display** | 16x2 I2C | Address: 0x27 |
| 6 | **Breadboard & Wires** | - | For connections |
| 7 | **Power Supply** | 5V USB or Battery | For ESP32 and peripherals |

---

## 📦 Software Requirements

### Required Tools
- **Arduino IDE** (v1.8.x or later)

### Required Libraries
