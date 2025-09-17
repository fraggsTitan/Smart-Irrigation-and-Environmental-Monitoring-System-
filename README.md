<h1><b>Automated Water Irrigator with ESP32</b></h1>
<h2><b>Project Overview</b></h2>

This project implements an automated plant irrigation system using an ESP32 microcontroller. It reads environmental parameters such as soil moisture, temperature, and humidity, and controls a water pump (represented by an LED in this prototype) accordingly. The system allows remote monitoring and control via ThingSpeak and a local API endpoint. It also features a digital LCD display to show live readings.

<h3><b>Features</b></h3>h3>

 1.Reads soil moisture using a sensor (analog pin)
 2.Measures temperature and humidity using BME280
 3.Displays real-time readings on a 16x2 I2C LCD
 4.Controls an LED (represents pump) based on soil moisture and environmental conditions
 5. Sends sensor data to ThingSpeak for cloud monitoring
 6. Allows remote adjustment of thresholds (SOIL_THRESHOLD, TEMP_THRESHOLD, HUM_THRESHOLD) via POST requests in JSON format
 7. Automatic pump/LED timeout after 2 minutes to prevent overwatering

<h3><b>Hardware Used</b></h3>
| # | Component                 | Notes                           |
| - | ------------------------- | ------------------------------- |
| 1 | ESP32 Microcontroller     | Main controller                 |
| 2 | BME280 Sensor             | Temperature, humidity, pressure |
| 3 | Soil Moisture Sensor      | Analog output                   |
| 4 | LED (in place of pump)    | GPIO 26                         |
| 5 | LCD 16x2 I2C              | I2C address 0x27                |
| 6 | Breadboard & jumper wires | For connections                 |
| 7 | Power Supply              | USB or battery                  |


<h3><b>Software Requirements:</b></h3>
 1. Arduino IDE or PlatformIO
 2. Libraries:
    a. WiFi.h
    b.HTTPClient.h
    c.LiquidCrystal_I2C.h
    d.WebServer.h
    e.ArduinoJson.h
    f.Adafruit_BME280.h
    g.Adafruit_Sensor.h

<h3><b>Project Structure</b></h3>
 1. sensors.c: Handles sensor readings, pump/LED control logic, and LCD updates
 2. main.c: Connects to Wi-Fi, handles ThingSpeak POST requests, sets up local WebServer API for threshold updates

<h3><b>Setup Instructions:</b></h3>
  1.Clone the project into a folder and open it in Arduino IDE.
  2.Install all required libraries via Library Manager.
  3.Connect the hardware according to pin configuration:
     a.SOIL_PIN = 34
     b.LED_PIN = 26
     c.BME280 → I2C 
     d.LCD → I2C 
  4.Set your Wi-Fi and ThingSpeak credentials in main.c
  5.Upload the code to ESP32.
  6.Open Serial Monitor to see Wi-Fi IP address (needed for Postman).

 <h3><b>Using the API</b></h3>
  Update the thresholds from Postman/any HTTP client
  Endpoint: POST http://<ESP32_IP>/updateThresholds 
  Body Syntax: {
                "soil": 2500,
                "temp": 32,
                "hum": 75
              }
  Expected Response: 200 Thresholds Updated

<h3><b>ThingSpeak Integration</b></h3>
  Sensor Data is sent once every 20 seconds to thingspeak in the format of:
    1.field1 → temperature
    2.field2 → humidity
    3.field3 → soil moisture
    4.field4 → pump/LED state

<h3><b>LCD Display</b></h3>
  1.First row: T: <temp>C H: <humidity>%
  2.Second row: Soil: <soil_moisture>% LED:ON/OFF
  3.Shows messages for irrigation state:
    a."LED:ON Soil Low" → soil below threshold
    b."LED:ON Hot/Humid" → temp/humidity high, soil slightly above threshold
    c."LED:OFF Soil OK" → soil moisture sufficient
