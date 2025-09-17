//this file handles the sensor function  calls and establishes connection with Thingspeak and wifi

#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <WebServer.h>
#include <ArduinoJson.h>
//imports from sensors.c
extern float temperature, humidity;
extern int soilMoisture, ledState;
extern int SOIL_THRESHOLD;
extern int TEMP_THRESHOLD;
extern int HUM_THRESHOLD;
//function prototype of the functions defined in sensors.c
void initSensors();
void readSensors();
void controlLogic(LiquidCrystal_I2C &lcd);

// WiFi + ThingSpeak credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* apiKey = "YOUR_THINGSPEAK_WRITE_API_KEY";
const char* thing_server = "http://api.thingspeak.com/update";

// LCD object its I2C address is 0X27 here, change if needed, 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);
//ESP 32 acts as a server and handles requests via a API endpoint
WebServer server(80);
// handler to update thresholds
void handleUpdateThresholds() {
    //checks if the sent JSON request has all these fields
    //if body of json is empty sends a 400 error
    if (server.hasArg("plain") == false) {
        server.send(400, "text/plain", "Body missing");
        return;
    }
    //gets the json body and converts the body to a bunch of key-value pairs
    String body = server.arg("plain");
    StaticJsonDocument<200> doc;// the 20 says that this JSON is for 200 bytes, this is fine since we are effectively taking in 3 ints
    DeserializationError error = deserializeJson(doc, body);
    if (error) {
        server.send(400, "text/plain", "Invalid JSON");
        return;
    }
    //sets attributes depending on what is available in the json request
    if (doc.containsKey("soil")) SOIL_THRESHOLD = doc["soil"];
    if (doc.containsKey("temp")) TEMP_THRESHOLD = doc["temp"];
    if (doc.containsKey("hum")) HUM_THRESHOLD = doc["hum"];
    //sends success response
    server.send(200, "text/plain", "Thresholds updated");
    /* Sample HTTP request from postman
        POST
        http://<ESP32_IP>/updateThresholds
        ESP32_IP will show in serial monitor after connection to wifi
        {
          "soil": 2500,
          "temp": 32,
          "hum": 75
        }
    */
}

void setup() {
  //begins connection  on port 115200
  Serial.begin(115200);
  //initialises LCD
  lcd.init();
  //Turns on LCD light behind characters
  lcd.backlight();
  //sets cursor to first column of first row
  lcd.setCursor(0, 0);
  //initially prints this onto the screen
  lcd.print("Smart Irrigator");
  //calls initSensors from sensors.c to establish BME280 connection
  initSensors();
  //gets connection with Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  //prints . till connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());
  server.on("/updateThresholds", HTTP_POST, handleUpdateThresholds);
  server.begin();
  Serial.println(" Server is Ready to accept requests!");
}