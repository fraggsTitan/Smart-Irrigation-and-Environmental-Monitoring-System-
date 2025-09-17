//this file handles hardware interaction and decision making logic

//Libraries
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <LiquidCrystal_I2C.h>

// Pin Configurations Macros
#define SOIL_PIN 34
#define LED_PIN 26

// Thresholds
int SOIL_THRESHOLD =2000;
int  TEMP_THRESHOLD =30;
int  HUM_THRESHOLD =70;

// Globals
Adafruit_BME280 bme;
float temperature = 0, humidity = 0;
int soilMoisture = 0;
int ledState = 0;

// Function prototypes
void initSensors();
void readSensors();
void controlLogic(LiquidCrystal_I2C &lcd);

//checks for BME and either successfully turns it on or sends error
void initSensors() {
  if (!bme.begin(0x76)) {
    Serial.println("Could not find BME280 sensor!");
    while (1);
  }
pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}
//reads temperature,humidity,soilMoisture and accordingly sets the global vars while also printing value to console
void readSensors() {
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  soilMoisture = analogRead(SOIL_PIN);

  Serial.print("Temp: "); Serial.println(temperature);
  Serial.print("Humidity: "); Serial.println(humidity);
  Serial.print("Soil: "); Serial.println(soilMoisture);
}
