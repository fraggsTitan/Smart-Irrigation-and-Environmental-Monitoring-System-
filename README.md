<h1><b>Automated Water Irrigator with ESP32</b></h1>
<h2><b>Project Overview</b></h2>

This project implements an automated plant irrigation system using an ESP32 microcontroller. It reads environmental parameters such as soil moisture, temperature, and humidity, and controls a water pump (represented by an LED in this prototype) accordingly. The system allows remote monitoring and control via ThingSpeak and a local API endpoint. It also features a digital LCD display to show live readings.<br>

<h3><b>Features</b></h3>h3>

 1.Reads soil moisture using a sensor (analog pin)<br>
 2.Measures temperature and humidity using BME280<br>
 3.Displays real-time readings on a 16x2 I2C LCD<br>
 4.Controls an LED (represents pump) based on soil moisture and environmental conditions<br>
 5. Sends sensor data to ThingSpeak for cloud monitoring<br>
 6. Allows remote adjustment of thresholds (SOIL_THRESHOLD, TEMP_THRESHOLD, HUM_THRESHOLD) via POST requests in JSON format<br>
 7. Automatic pump/LED timeout after 2 minutes to prevent overwatering<br>
<br>
<h3><b>Hardware Used</b></h3>
| # | Component                 | Notes                           |<br>
| - | ------------------------- | ------------------------------- |<br>
| 1 | ESP32 Microcontroller     | Main controller                 |<br>
| 2 | BME280 Sensor             | Temperature, humidity, pressure |<br>
| 3 | Soil Moisture Sensor      | Analog output                   |<br>
| 4 | LED (in place of pump)    | GPIO 26                         |<br>
| 5 | LCD 16x2 I2C              | I2C address 0x27                |<br>
| 6 | Breadboard & jumper wires | For connections                 |<br>
| 7 | Power Supply              | USB or battery                  |<br>


<h3><b>Software Requirements:</b></h3>
 1. Arduino IDE<br>
 2. Libraries:<br>
    a. WiFi.h<br>
    b.HTTPClient.h<br>
    c.LiquidCrystal_I2C.h<br>
    d.WebServer.h<br>
    e.ArduinoJson.h<br>
    f.Adafruit_BME280.h<br>
    g.Adafruit_Sensor.h<br>
<br>
<h3><b>Project Structure</b></h3>
 1. sensors.c: Handles sensor readings, pump/LED control logic, and LCD updates<br>
 2. main.c: Connects to Wi-Fi, handles ThingSpeak POST requests, sets up local WebServer API for threshold updates<br>
<br>
<h3><b>Setup Instructions:</b></h3>
  1.Clone the project into a folder and open it in Arduino IDE.<br>
  2.Install all required libraries via Library Manager.<br>
  3.Connect the hardware according to pin configuration:<br>
     a.SOIL_PIN = 34<br>
     b.LED_PIN = 26<br>
     c.BME280 → I2C <br>
     d.LCD → I2C <br>
  4.Set your Wi-Fi and ThingSpeak credentials in main.c<br>
  5.Upload the code to ESP32.<br>
  6.Open Serial Monitor to see Wi-Fi IP address (needed for Postman).<br>

 <h3><b>Using the API</b></h3><br>
  Update the thresholds from Postman/any HTTP client<br>
  Endpoint: POST http://<ESP32_IP>/updateThresholds <br>
  Body Syntax: {<br>
                "soil": 2500,<br>
                "temp": 32,<br>
                "hum": 75<br>
              }<br>
  Expected Response: 200 Thresholds Updated<br>
<br>
<h3><b>ThingSpeak Integration</b></h3><br>
  Sensor Data is sent once every 20 seconds to thingspeak in the format of:<br>
    1.field1 → temperature<br>
    2.field2 → humidity<br>
    3.field3 → soil moisture<br>
    4.field4 → pump/LED state<br>

<h3><b>LCD Display</b></h3><br>
  1.First row: T: <temp>C H: <humidity>%<br>
  2.Second row: Soil: <soil_moisture>% LED:ON/OFF<br>
  3.Shows messages for irrigation state:<br>
    a."LED:ON Soil Low" → soil below threshold<br>
    b."LED:ON Hot/Humid" → temp/humidity high, soil slightly above threshold<br>
    c."LED:OFF Soil OK" → soil moisture sufficient<br>
