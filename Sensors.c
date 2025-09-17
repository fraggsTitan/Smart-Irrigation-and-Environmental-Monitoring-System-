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

//this is all the logic that controls LCD display and also the pump control
void controlLogic(LiquidCrystal_I2C &lcd) {
    lcd.clear();//try to operate on this if you want to avoid flickering

    // Line 1: Temp + Humidity prints this data to first row of LCD
    lcd.setCursor(0, 0);
    lcd.print("T:"); lcd.print(temperature); lcd.print("C ");
    lcd.print("H:"); lcd.print(humidity); lcd.print("%");

    // Soil percent  is usually between 0-4095 and mapped to 0-100%
    int soilPercent = map(soilMoisture, 0, 4095, 0, 100);

    // static variables
    static unsigned long pumpStartTime = 0;
    static bool pumpActive = false;

    if (!pumpActive) {
        //checks if pump should be turned on or LED here
        if (soilMoisture < SOIL_THRESHOLD) {
            //this is case where moisture is too low and so it is turned on
            digitalWrite(LED_PIN, HIGH);
            ledState = 1;
            pumpStartTime = millis();
            pumpActive = true;
        } else if ((temperature > TEMP_THRESHOLD || humidity > HUM_THRESHOLD) &&
                   (soilMoisture < (int)(1.4 * SOIL_THRESHOLD))) {
            //1.4 is arbitary value but just says that if temp/himidity is high then we soil moisture should be little higher to account for that
            digitalWrite(LED_PIN, HIGH);
            ledState = 1;
            pumpStartTime = millis();
            pumpActive = true;
        }
    } else {
        if (millis() - pumpStartTime >= 120000) {//checks if time >2 mins, and stops
            digitalWrite(LED_PIN, LOW);
            ledState = 0;
            pumpActive = false;
        }
    }

    // Line 2: Soil + Pump Status// prints pump status onto the second line of the LCD display
    lcd.setCursor(0, 1);
    lcd.print("Soil:");
    lcd.print(soilPercent); lcd.print("% ");
    lcd.print(pumpActive ? "LED:ON " : "LED:OFF");
}
