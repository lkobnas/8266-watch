#include <SD.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "PageController.h"
#include "sensor/Button.h"

PageController* pageController;
Button* prevButton;
Button* nextButton;
Button* setupButton;

void readConfigFile();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  readConfigFile();

  prevButton->init();
  nextButton->init();
  setupButton->init();

  pageController = new PageController(0x3C);

}

void loop() {

  pageController->displayCurrentPage();
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

void readConfigFile()
{
  // Read config file
  File configFile = SD.open("config.json");
  if (!configFile) {
    Serial.println("Failed to open config.json file.");
    return;
  }
  
  // Parse the JSON object
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, configFile);

  if (error) {
    Serial.print("Failed to parse config.json file: ");
    Serial.println(error.c_str());
    return;
  }

  String prev_button = doc["prev_button"];
  String next_button = doc["next_button"];
  String setup_button = doc["setup_button"];
  String oled_address = doc["oled_address"];
  String wifi_ssid = doc["wifi_ssid"];
  String wifi_password = doc["wifi_password"];
  String openweather_api_key = doc["openweather_api_key"];
  String openweather_city = doc["openweather_city"];
  String openweather_country_code = doc["openweather_country_code"];

  prevButton = new Button(prev_button.toInt());
  nextButton = new Button(next_button.toInt());
  setupButton = new Button(setup_button.toInt());
  

}
