//this file handles hardware interaction and decision making logic

//Libraries
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Pin Configurations Macros
#define SOIL_PIN 34
#define LED_PIN 26
#define DHT_PIN 25          // choose your pin where DHT22 is connected
#define DHT_TYPE DHT22

// Thresholds
int SOIL_THRESHOLD = 2000;
int TEMP_THRESHOLD = 30;
int HUM_THRESHOLD  = 70;

// Globals
DHT dht(DHT_PIN, DHT_TYPE);
float temperature = 0, humidity = 0;
int soilMoisture = 0;
int ledState = 0;

// Function prototypes
void initSensors();
void readSensors();
void controlLogic(LiquidCrystal_I2C &lcd);

// Initialize sensors (DHT + LED)
void initSensors() {
  dht.begin();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("DHT22 + Soil Sensor initialized");
}

// Read temperature, humidity, soilMoisture and set globals
void readSensors() {
  temperature = dht.readTemperature();
  humidity    = dht.readHumidity();
  soilMoisture = analogRead(SOIL_PIN);

  // Basic check for NaN (sometimes DHT fails)
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temp: "); Serial.println(temperature);
  Serial.print("Humidity: "); Serial.println(humidity);
  Serial.print("Soil: "); Serial.println(soilMoisture);
}