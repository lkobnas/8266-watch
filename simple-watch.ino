/*Simple Watch
  Developed by San



  Change Log
  added blink when setup

*/
//I2C 
#include <Wire.h>

//DS3231
#include <DS3231.h>
DS3231 Clock;
bool Century=false,h12=true,pm;
char week[7][20] = {"SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY"};
char mon[12][4] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
int clockCenterX=63;
int clockCenterY=31;


//MAX30102
#include "MAX30105.h" //sparkfun MAX3010X library
#include "heartRate.h"
MAX30105 particleSensor;

//SSD1306
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#define OLED_ADDR   0x3C
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

#define R_BUTTON D7
#define L_BUTTON D6
#define SETUP_BUTTON D5

//WiFi
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>

const char* ssid = "";                                                        //WiFi Name
const char* password = "";                                               //WiFi Password

// Your Domain name with URL path or IP address with path
String openWeatherMapApiKey = "";               //Open Weather Map API Key

// Replace with your country code and city
String city = "";
String countryCode = "";  

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;
String jsonBuffer;

//#define MAX30105 //if you have Sparkfun's MAX30105 breakout board , try #define MAX30105 

int test = 0;
byte temp_sec, temp_min, temp_hour, temp_date, alarm_hour, alarm_min, alarm_sec;
int temp_DoW, temp_month;

long time_now=0, time_last=0;
long Wifi_timeout = 1000*60;//*5;
long Wifi_timelast = 0; 

String weather, description;

//setup mode
bool setup_mode = false;
int setup_hold = -1;
bool setup_lock = false;
int setup_pos = -2;

//alarm
bool alarm = false;
bool alarm_set = false;

// 'sunny_day', 16x16px
const unsigned char myBitmapsunny_day [] PROGMEM = {
  0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x10, 0x08, 0x03, 0xc0, 0x04, 0x20, 0x08, 0x10, 0x48, 0x12, 
  0x48, 0x12, 0x08, 0x10, 0x04, 0x20, 0x03, 0xc0, 0x10, 0x08, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00
};
// 'Moon', 16x16px
const unsigned char  myBitmapMoon [] PROGMEM = {
  0x00, 0x00, 0x02, 0x00, 0x0e, 0x00, 0x14, 0x00, 0x24, 0x00, 0x24, 0x00, 0x24, 0x00, 0x42, 0x00, 
  0x42, 0x00, 0x21, 0x02, 0x20, 0xfe, 0x30, 0x04, 0x18, 0x08, 0x0c, 0x30, 0x03, 0xc0, 0x00, 0x00
};
// 'rainy_day', 16x16px
const unsigned char myBitmaprainy_day [] PROGMEM = {
  0x03, 0x80, 0x0c, 0x60, 0x08, 0x30, 0x10, 0x1c, 0x10, 0x02, 0x60, 0x03, 0x40, 0x01, 0x80, 0x03, 
  0x40, 0x02, 0x7f, 0xfc, 0x0f, 0xf0, 0x12, 0x00, 0x36, 0x48, 0x00, 0x00, 0x12, 0x40, 0x24, 0x40
};
// 'top_arrow', 24x24px
const unsigned char  myBitmapnoun_top_arrow [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x7e, 0x00, 0x00, 
  0xff, 0x00, 0x01, 0xff, 0x80, 0x03, 0xff, 0xc0, 0x03, 0x99, 0xc0, 0x03, 0x18, 0xc0, 0x00, 0x18, 
  0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 
  0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'down_arrow', 24x24px
const unsigned char  myBitmapnoun_down_arrow [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 
  0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 
  0x00, 0x03, 0x18, 0xc0, 0x03, 0x99, 0xc0, 0x01, 0xff, 0x80, 0x00, 0xff, 0x00, 0x00, 0x7e, 0x00, 
  0x00, 0x3c, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// Temperature_bitmap, 32x32px
const unsigned char Temperature_bitmap [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x18, 0xc0, 0x00, 0x00, 0x10, 0x40, 0x00, 
  0x00, 0x10, 0x4e, 0x00, 0x00, 0x10, 0x40, 0x00, 0x00, 0x10, 0x40, 0x00, 0x00, 0x10, 0x40, 0x00, 
  0x00, 0x10, 0x4c, 0x00, 0x00, 0x12, 0x40, 0x00, 0x00, 0x12, 0x40, 0x00, 0x00, 0x12, 0x4e, 0x00, 
  0x00, 0x12, 0x40, 0x00, 0x00, 0x12, 0x40, 0x00, 0x00, 0x12, 0x40, 0x00, 0x00, 0x12, 0x40, 0x00, 
  0x00, 0x12, 0x40, 0x00, 0x00, 0x32, 0x60, 0x00, 0x00, 0x67, 0x30, 0x00, 0x00, 0x4f, 0x90, 0x00, 
  0x00, 0x5f, 0xd0, 0x00, 0x00, 0x5f, 0xd0, 0x00, 0x00, 0x4f, 0x90, 0x00, 0x00, 0x6f, 0xb0, 0x00, 
  0x00, 0x20, 0x20, 0x00, 0x00, 0x18, 0xc0, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'no-wifi', 32x32px
const unsigned char  myBitmapno_wifi [] PROGMEM = {
  0x07, 0xff, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xfc, 0x78, 0x00, 0x00, 0x1e, 
  0x7c, 0x00, 0x00, 0x0e, 0xee, 0x00, 0x00, 0x07, 0xe7, 0x00, 0x00, 0x07, 0xe3, 0x00, 0x00, 0x07, 
  0xe0, 0x0f, 0xf0, 0x07, 0xe0, 0x7f, 0xfe, 0x07, 0xe0, 0xfe, 0x7f, 0x07, 0xe3, 0xe0, 0x07, 0xc7, 
  0xe7, 0x80, 0x01, 0xc7, 0xe7, 0x0f, 0xf0, 0xe7, 0xe0, 0x3f, 0xfc, 0x07, 0xe0, 0x7c, 0x3e, 0x07, 
  0xe0, 0xf0, 0x0f, 0x07, 0xe0, 0x41, 0x82, 0x07, 0xe0, 0x0f, 0xe0, 0x07, 0xe0, 0x1f, 0xf0, 0x07, 
  0xe0, 0x1c, 0x30, 0x07, 0xe0, 0x00, 0x00, 0x07, 0xe0, 0x01, 0x83, 0x07, 0xe0, 0x01, 0x83, 0x87, 
  0xe0, 0x01, 0x81, 0xc7, 0xe0, 0x00, 0x00, 0xe7, 0xe0, 0x00, 0x00, 0x77, 0x70, 0x00, 0x00, 0x3e, 
  0x78, 0x00, 0x00, 0x1e, 0x3f, 0x00, 0x00, 0xfc, 0x1f, 0xff, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xe0
};
// 'wifi', 32x32px
const unsigned char  myBitmapwifi [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x1f, 0xf8, 0x00, 0x00, 0xff, 0xff, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xff, 0xf0, 
  0x1f, 0xc0, 0x03, 0xf8, 0x7f, 0x00, 0x00, 0xfe, 0xfc, 0x07, 0xe0, 0x3f, 0xf8, 0x3f, 0xfc, 0x1f, 
  0xf0, 0xff, 0xff, 0x0f, 0x03, 0xff, 0xff, 0xc0, 0x07, 0xf8, 0x1f, 0xe0, 0x07, 0xc0, 0x03, 0xe0, 
  0x07, 0x83, 0xc1, 0xe0, 0x02, 0x1f, 0xf8, 0x40, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x7f, 0xfe, 0x00, 
  0x00, 0x78, 0x1e, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 
  0x00, 0x03, 0xc0, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'cloud', 16x16px
const unsigned char myBitmapcloud [] PROGMEM = {
  0x00, 0x00, 0x02, 0x00, 0x07, 0x00, 0x0d, 0xf0, 0x18, 0xf8, 0x30, 0xcc, 0x30, 0x84, 0x61, 0x86, 
  0x61, 0x86, 0x60, 0x86, 0x20, 0xfc, 0x30, 0x78, 0x18, 0xe0, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00
};
// 'humidity', 16x16px
const unsigned char myBitmaphumidity [] PROGMEM = {
  0x00, 0x00, 0x02, 0x00, 0x07, 0x00, 0x0d, 0xf0, 0x18, 0xf8, 0x30, 0xcc, 0x30, 0x84, 0x61, 0x86, 
  0x61, 0x86, 0x60, 0x86, 0x20, 0xfc, 0x30, 0x78, 0x18, 0xe0, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00
};
// 'wind', 16x16px
const unsigned char myBitmapwind [] PROGMEM = {
  0x07, 0x00, 0x0f, 0x80, 0x08, 0x80, 0x00, 0x9c, 0x01, 0xb6, 0x7f, 0x22, 0x00, 0x02, 0x7f, 0xfe, 
  0x7f, 0xf8, 0x00, 0x00, 0x7f, 0xfc, 0x00, 0x06, 0x00, 0x02, 0x00, 0x22, 0x00, 0x3e, 0x00, 0x1c
};
// 'cloudy', 32x32px
const unsigned char myBitmapcloudy [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x31, 0x80, 0x00, 0x00, 0x23, 0xe0, 0x00, 
  0x03, 0xcc, 0x18, 0x00, 0x0c, 0x58, 0x0c, 0x00, 0x08, 0x30, 0x04, 0x00, 0x10, 0x20, 0x02, 0x00, 
  0x10, 0x20, 0x02, 0x00, 0x30, 0x20, 0x03, 0xc0, 0x43, 0xe0, 0x01, 0xb8, 0x84, 0x00, 0x00, 0x0c, 
  0x88, 0x00, 0x00, 0x06, 0x98, 0x00, 0x00, 0x02, 0x70, 0x00, 0x00, 0x01, 0x60, 0x00, 0x00, 0x01, 
  0xc0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x02, 
  0x40, 0x00, 0x00, 0x04, 0x60, 0x00, 0x00, 0x08, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'thunder', 32x32px
const unsigned char myBitmapthunder [] PROGMEM = {
  0x00, 0x3f, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x01, 0xe1, 0xe0, 0x00, 0x01, 0x80, 0x60, 0x00, 
  0x03, 0x00, 0x3f, 0x00, 0x03, 0x00, 0x3f, 0x80, 0x03, 0x00, 0x11, 0xc0, 0x07, 0x00, 0x00, 0x60, 
  0x1e, 0x00, 0x00, 0x60, 0x38, 0x00, 0x00, 0x60, 0x70, 0x00, 0x00, 0x78, 0x60, 0x00, 0x00, 0x3c, 
  0xc0, 0x01, 0x80, 0x0e, 0xc0, 0x01, 0x80, 0x07, 0xc0, 0x03, 0x80, 0x03, 0xc0, 0x03, 0x80, 0x03, 
  0x60, 0x07, 0x80, 0x03, 0x60, 0x0f, 0x80, 0x07, 0x38, 0x0d, 0x80, 0x0e, 0x1f, 0xfd, 0xff, 0xfc, 
  0x0f, 0xf9, 0xff, 0xf8, 0x00, 0x38, 0x0c, 0x00, 0x00, 0x30, 0x1c, 0x00, 0x00, 0x7f, 0x98, 0x00, 
  0x00, 0x7f, 0xb8, 0x00, 0x00, 0x01, 0xb0, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x01, 0xe0, 0x00, 
  0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00
};
// 'sunny', 32x32px
const unsigned char myBitmapsunny [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 
  0x00, 0x01, 0x80, 0x00, 0x06, 0x00, 0x00, 0x60, 0x07, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x00, 0xc0, 
  0x00, 0x81, 0x81, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x30, 0x0c, 0x00, 
  0x00, 0x60, 0x06, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x40, 0x02, 0x00, 0x78, 0xc0, 0x03, 0x1e, 
  0x78, 0xc0, 0x03, 0x1e, 0x00, 0x40, 0x02, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x60, 0x06, 0x00, 
  0x00, 0x30, 0x0c, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x81, 0x81, 0x00, 
  0x03, 0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0xe0, 0x06, 0x00, 0x00, 0x60, 0x00, 0x01, 0x80, 0x00, 
  0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'clearmoon', 32x32px
const unsigned char myBitmapclearmoon [] PROGMEM = {
  0x00, 0x0f, 0x80, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0xff, 0x04, 0x0c, 0x03, 0xce, 0x06, 0x04, 
  0x07, 0x1c, 0x0e, 0x00, 0x0e, 0x18, 0x1f, 0x80, 0x1c, 0x30, 0x7b, 0xc0, 0x38, 0x30, 0x3f, 0x80, 
  0x30, 0x30, 0x0e, 0x00, 0x70, 0x20, 0x06, 0x00, 0x60, 0x20, 0x06, 0x03, 0x60, 0x60, 0x00, 0x00, 
  0xc0, 0x20, 0x00, 0x00, 0xc0, 0x30, 0x00, 0x00, 0xc0, 0x30, 0x00, 0x00, 0xc0, 0x30, 0x00, 0x00, 
  0xc0, 0x18, 0x00, 0x00, 0xc0, 0x18, 0x00, 0x00, 0xc0, 0x0c, 0x00, 0x00, 0xc0, 0x0e, 0x00, 0x00, 
  0x60, 0x07, 0x00, 0x06, 0x60, 0x03, 0xc0, 0x1e, 0x70, 0x00, 0xfd, 0xfe, 0x30, 0x00, 0x3f, 0xfc, 
  0x38, 0x00, 0x00, 0x18, 0x1c, 0x00, 0x00, 0x38, 0x0e, 0x00, 0x00, 0x70, 0x07, 0x00, 0x00, 0xe0, 
  0x03, 0xc0, 0x01, 0xc0, 0x01, 0xf0, 0x0f, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x0f, 0xf0, 0x00
};
// 'rainy', 32x32px
const unsigned char myBitmaprainy [] PROGMEM = {
  0x00, 0x3f, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x01, 0xe1, 0xe0, 0x00, 0x01, 0x80, 0x60, 0x00, 
  0x03, 0x00, 0x3f, 0x00, 0x03, 0x00, 0x3f, 0x80, 0x03, 0x00, 0x11, 0xc0, 0x07, 0x00, 0x00, 0x60, 
  0x1e, 0x00, 0x00, 0x60, 0x38, 0x00, 0x00, 0x60, 0x70, 0x00, 0x00, 0x78, 0x60, 0x00, 0x00, 0x3c, 
  0xc0, 0x00, 0x00, 0x0e, 0xc0, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 
  0x60, 0x00, 0x00, 0x03, 0x60, 0x00, 0x00, 0x07, 0x38, 0x00, 0x00, 0x0e, 0x1f, 0xff, 0xff, 0xfc, 
  0x0f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x18, 0x0c, 0x06, 
  0x30, 0x18, 0x0c, 0x06, 0x70, 0x38, 0x1c, 0x0e, 0x62, 0x31, 0x18, 0x8c, 0x66, 0x33, 0x19, 0x8c, 
  0x06, 0x03, 0x01, 0x80, 0x0c, 0x06, 0x03, 0x00, 0x0c, 0x06, 0x03, 0x00, 0x08, 0x04, 0x02, 0x00
};
// 'mist', 32x32px
const unsigned char myBitmapmist [] PROGMEM = {
  0x00, 0x3f, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x01, 0xe1, 0xe0, 0x00, 0x01, 0x80, 0x60, 0x00, 
  0x03, 0x00, 0x3f, 0x00, 0x03, 0x00, 0x3f, 0x80, 0x03, 0x00, 0x11, 0xc0, 0x07, 0x00, 0x00, 0x60, 
  0x1e, 0x00, 0x00, 0x60, 0x38, 0x00, 0x00, 0x60, 0x70, 0x00, 0x00, 0x78, 0xe0, 0x00, 0x00, 0x3c, 
  0xc0, 0x00, 0x00, 0x0e, 0xc0, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 
  0xe0, 0x00, 0x00, 0x03, 0x60, 0x00, 0x00, 0x07, 0x38, 0x00, 0x00, 0x0e, 0x1f, 0xff, 0xff, 0xfc, 
  0x0f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xfc, 0x3f, 0x0f, 0xfc, 0xff, 0xff, 0xff, 
  0x1c, 0x07, 0xe1, 0xf8, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc7, 0xc7, 0x7f, 0xff, 0xe7, 0xe2, 
  0x00, 0x00, 0x00, 0x00, 0xf0, 0xf8, 0x3f, 0x0f, 0xff, 0xf8, 0xff, 0xff, 0x1f, 0x80, 0xe1, 0xf8
};
// 'drizzle', 32x32px
const unsigned char myBitmapdrizzle [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x01, 0x80, 0x08, 
  0x00, 0x00, 0x80, 0x10, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x30, 0xc0, 0x00, 0x1f, 0xe0, 0x60, 
  0x00, 0x30, 0x70, 0x20, 0x00, 0x60, 0x18, 0x27, 0x00, 0xc0, 0x08, 0x20, 0x1f, 0x80, 0x0c, 0x20, 
  0x30, 0x00, 0x04, 0x60, 0x60, 0x00, 0x06, 0xc0, 0xc0, 0x00, 0x07, 0x80, 0xc0, 0x00, 0x06, 0x18, 
  0x80, 0x00, 0x04, 0x08, 0x80, 0x00, 0x04, 0x00, 0xc0, 0x00, 0x0c, 0x00, 0x40, 0x00, 0x18, 0x00, 
  0x30, 0x00, 0x30, 0x00, 0x1f, 0xff, 0xe0, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x06, 0x00, 0x80, 0x00, 0x06, 0x01, 0xc0, 0x00, 0x0f, 0x01, 0x40, 0x00, 0x0f, 0x31, 0x60, 0x00, 
  0x06, 0x39, 0xc0, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00
};

const int period = 1;
bool lock = false;
enum setting{
  CLEAR, //BLACK: 0
  SET    //WHITE: 1
};
long count =-1;
int beat =-1;
bool RESET = false;

#define USEFIFO
#define BUZZER D8
#define LED D3
int freq = 2000;
int resolution = 8;

//Default temp value


void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing...");
  pinMode(LED,OUTPUT);
  pinMode(BUZZER,OUTPUT);
  pinMode(R_BUTTON, INPUT);
  pinMode(L_BUTTON, INPUT);
  pinMode(SETUP_BUTTON, INPUT);
  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30102 was not found. Please check wiring/power/solder jumper at MH-ET LIVE MAX30102 board. ");
    while (1);
  }

  //Setup to sense a nice looking saw tooth on the plotter
  byte ledBrightness = 0x7F; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  //Options: 1 = IR only, 2 = Red + IR on MH-ET LIVE MAX30102 board
  int sampleRate = 400; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 16384; //Options: 2048, 4096, 8192, 16384
  // Set up the wanted parameters
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  particleSensor.shutDown();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
  Serial.println(F("SSD1306 allocation failed"));
  for (;;)
    ; // Don't proceed, loop forever
  }
  //Wifi setup
  Wifi_setup();

  display.clearDisplay();
  display.display();
  delay(2000); // Pause for 2 seconds
}
void Wifi_setup(){
  WiFi.begin(ssid, password);
}

void heartbeat_setup(){
  particleSensor.wakeUp();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setRotation(0);
  display.setCursor(5,10);
  display.print("BPM %SpO2");
  display.display();
  drawBeat(0,SET);  
}

double avered = 0; double aveir = 0;
double sumirrms = 0;
double sumredrms = 0;
int i = 0;
int Num = 100;//calculate SpO2 by this sampling interval

bool fingerState = false;
int prev_Ebpm = 0;
int prev_SpO2 = 80;
double ESpO2 = 95.0;//initial value of estimated SpO2
double FSpO2 = 0.7; //filter factor for estimated SpO2
double frate = 0.95; //low pass filter for IR/red LED value to eliminate AC component
#define TIMETOBOOT 3000 // wait for this time(msec) to output SpO2
#define SCALE 88.0 //adjust to display heart beat and SpO2 in the same scale
#define SAMPLING 5 //if you want to see heart beat more precisely , set SAMPLING to 1
#define FINGER_ON 95000 // if red signal is lower than this , it indicates your finger is not on the sensor
#define MINIMUM_SPO2 80.0

int interface = 0;
bool input_lock = false;
int page_now = 0;
bool wifi_status = false;
JSONVar myObject;


void loop()                                                             //// MAIN PROGRAM
{ 
   checkAlarm();                                                   
   read_input();
   //Serial.print("Interface: ");
   //Serial.println(interface);
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------   
//製定界面 (提示：switch(interface){ )
   switch(interface){
    case 0:
      setupHold();
      clock_main();
      break;
    case 1:
      heartbeat_main();
      break;
    case 2:
      temp_main();  
      Serial.println(wifi_status);
      break;
    case 3:
      weather_main();
      break;
    case 4:
      setupHold();
      alarm_main();
      break;
    case 5:
      drawClockDisplay();
      drawRealClock();
      break;
   }
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
   //interface reference
   //heartbeat_main();
   //clock_main();
}
void read_input(){                                              //Detect if red or yellow button is pressed
  //if(digitalRead(RESET_BUTTON)==1)
  if(digitalRead(L_BUTTON)==0 && digitalRead(R_BUTTON)==0){     //release input_lock
    input_lock = false;
  }
  if(alarm == true && (digitalRead(SETUP_BUTTON)==1 || digitalRead(R_BUTTON)==1 || digitalRead(L_BUTTON)==1)){            //reset alarm by pressing any button
    alarm_set = false;
    alarm = false;
    delay(250);
  }
  if(input_lock == false && setup_mode == false){               //Initialize
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//當觸碰到右邊按鍵時

    if(digitalRead(R_BUTTON)==1){
      if(interface==0){
        heartbeat_setup();
        interface = 1;     
      }else if(interface==1){
        particleSensor.shutDown();
        Wifi_check();
        temp_setup();     
        interface = 2;
      }else if(interface==2){
        weather_setup();
        interface = 3;
      }else if(interface==3){ 
        alarm_setup();    
        interface = 4;
      }else if(interface==4){
        interface = 5;
      }else if(interface==5){
        interface = 0;
      }
 //-------------------------------------------------------------------------------------------------------------------------------------------------------------------
      
      input_lock = true;
      delay(250);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//當觸碰到左邊按鍵時
      
    }else if(digitalRead(L_BUTTON)==1){
      if(interface==5){
        alarm_setup();
        interface =4;
      }else if(interface==4){
        Wifi_check();
        weather_setup();
        interface =3;
      }else if(interface==3){
        Wifi_check();
        temp_setup();    
        interface = 2;
      }else if(interface ==2){
        heartbeat_setup();
        interface = 1;    
      }else if(interface ==1){
        particleSensor.shutDown();
        interface = 0;      
      }else if(interface ==0){
        interface = 5;
      }
      input_lock = true;
      delay(250);  
    }
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
  }
}

int alarm_buzzer_count = 0;
int sound_pattern = 0;
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//製定鬧鐘 (例子：void xxx(){    ,提示：function name: checkAlarm )
void checkAlarm(){
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
  if(alarm_set == true){
    if(Clock.getHour(h12, pm)==alarm_hour && Clock.getMinute()==alarm_min && Clock.getSecond()==alarm_sec){
      alarm = true;
    }
  }
  if(alarm == true){
    //tone(BUZZER, 1000);
    alarm_buzzer_count++;
    alarm_sound();
  }else{
    alarm_buzzer_count = 0;
    sound_pattern = 0;
    noTone(BUZZER);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//製定鈴聲 (提示：function name: alarm_sound )
void alarm_sound(){
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
  if(sound_pattern<4){
    if(alarm_buzzer_count<5){
      tone(BUZZER,3000);
    }else if(alarm_buzzer_count<10){
      noTone(BUZZER);
    }else if(alarm_buzzer_count<15){
      tone(BUZZER,3000);
    }else if(alarm_buzzer_count<30){
      noTone(BUZZER);
    }else{
      sound_pattern++;
      alarm_buzzer_count=0;
    }
  }else{
    if(alarm_buzzer_count<2){
      tone(BUZZER,3000);
    }else if(alarm_buzzer_count<4){
      noTone(BUZZER);
    }else if(alarm_buzzer_count<6){
      tone(BUZZER,3000);
    }else if(alarm_buzzer_count<8){
      noTone(BUZZER);
    }else if(alarm_buzzer_count<10){
      tone(BUZZER,3000); 
    }else if(alarm_buzzer_count<12){
      noTone(BUZZER);  
    }else if(alarm_buzzer_count<14){
      tone(BUZZER,3000); 
    }else if(alarm_buzzer_count<30){
      noTone(BUZZER);        
    }else{
      alarm_buzzer_count=0;
    }    
  }
}
void setupHold(){
  if(digitalRead(SETUP_BUTTON)==1){
    setup_hold++;
  }else{
    setup_hold = -1;
    setup_lock = false;
  }
  if(setup_mode == true && setup_lock == false){
    if(interface==4){
        if(setup_hold>1&&setup_hold<3){
          if(setup_pos==3){
            setup_pos = 0;
            delay(250);
          }else if(setup_pos==0){
            setup_pos = 1;
            delay(250);
          }else if(setup_pos==1){
            setup_pos = 2;
            delay(250);
          }else if(setup_pos==2){
            setup_pos = 3;
            delay(250);
          }
        }
    }else if(interface ==0){
       if(setup_hold>1&&setup_hold<3){
        if(setup_pos==5){
          setup_pos = 0;
          delay(250);
        }else if(setup_pos==0){
          setup_pos = 1;
          delay(250);
        }else if(setup_pos==1){
          setup_pos = 2;
          delay(250);
        }else if(setup_pos==2){
          setup_pos = 3;
          delay(250);
        }else if(setup_pos==3){
          setup_pos = 4;
          delay(250);
        }else if(setup_pos==4){
          setup_pos = 5;
          delay(250);
        }
      }
    }
    
  }
  if(setup_hold>60 && setup_lock == false){       //hold for 3 seconds to enter or exit setting mode
    //alarm_set = false;
    setup_lock = true;
    setup_mode = !setup_mode;
    if(setup_mode){
      setup_pos = -1;
    }else{
      setup_pos = -2;
    }
  }

  if(setup_mode == true){                      //Enter alarm setting mode
    if(interface==4){
      alarm_setting();
    }else if(interface ==0){
      clock_setting();
    }
  }
}

void alarm_setting(){                                     //setting function called when setting mode is true
  if(setup_pos == -1){
    alarm_set = false;
    setup_pos = 0;
  }
  if(digitalRead(R_BUTTON)==1 && input_lock==false){
    input_lock = true;

    switch(setup_pos){
      case 0:
        if(alarm_hour<23)
        alarm_hour++;
        else
        alarm_hour= 0;
      break;
      case 1:
        if(alarm_min<59)
        alarm_min++;
        else
        alarm_min= 0;
      break;
      case 2:
        if(alarm_sec<59)
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//增加秒數 (提示：將變數(alarm_sec)增加 1 )
      alarm_sec++;
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
        else
        alarm_sec = 0;
      break;      
      case 3:      
        if(alarm_set == true){
          setup_mode = false;
          setup_pos = -1;
        }else{
          alarm_set = true;
        }
      break;
    }
  }
  if(digitalRead(L_BUTTON)==1 && input_lock==false){
    input_lock = true;
    switch(setup_pos){
      case 0:
        if(alarm_hour>0)
        alarm_hour--;
        else
        alarm_hour = 23;
      break;
      case 1:
        if(alarm_min>0)
        alarm_min--;
        else
        alarm_min = 59;
      break;
      case 2:
        if(alarm_sec>0)
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//減少秒數  (提示：將變數(alarm_sec)減少 1 )
        alarm_sec--;
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
        else
        alarm_sec = 59;
      break;    
      case 3:
        alarm_set = false;
      break;
    }    
  }  
}
void alarm_setup(){                                       //Get pre-set alarm time
  if(alarm_set==false){
    alarm_hour = Clock.getHour(h12, pm);
    alarm_min = Clock.getMinute();
    alarm_sec = Clock.getSecond();
  }
}

void alarm_main(){                                      //Draw alarm page
  display.clearDisplay();
    byte dhour, dmin, dsec;
    dhour = alarm_hour;
    dmin = alarm_min;
    dsec = alarm_sec;
     
    display.setCursor(0,0);
    display.setTextSize(1);
    display.print("        Alarm");
    display.println();
    display.println();
    //display.println();
    display.print(" ");
    display.setTextSize(2);
    display.print(" ");

    time_now = millis();
    if(setup_mode==false || time_now-time_last < 300 ){      //normal mode
      if(dhour<10){
        display.print("0");
      }  
      display.print(dhour);
      display.print(":");   
      if(dmin<10){
        display.print("0");
      }
      display.print(dmin);
      display.print(":");
      if(dsec<10){
        display.print("0");
      }
      display.print(dsec);
      display.setCursor(40,48);
      display.setTextSize(1);
      if(alarm_set)
      display.print("Alarm Set");
      else
      display.print("Alarm Off");
    }else{                          //setup mode
      if(setup_pos==0){
        display.print("  ");
      }else{
        if(dhour<10){
          display.print("0");
        }  
        display.print(dhour);
      }

        display.print(":");   

      if(setup_pos==1){
        display.print("  ");
      }else{
        if(dmin<10){
          display.print("0");
        }
        display.print(dmin);
      }

        display.print(":");
      
      if(setup_pos==2){
        display.print("  ");
      }else{
        if(dsec<10){
          display.print("0");
        }
        display.print(dsec);
      }
    
        display.setCursor(40,48);
        display.setTextSize(1);
        if(setup_pos==3){
          display.print("         ");
        }else{
          if(alarm_set)
          display.print("Alarm Set");
          else
          display.print("Alarm Off");
        }
      
      if(time_now-time_last>600){
        time_last = time_now;
      }
    }
  display.display();
}

void Wifi_connecting(){         //show WiFi connecting
   display.clearDisplay();
   display.setTextSize(1);
   display.setCursor(0,20);
   display.print("WiFi Connecting...");
   display.display();
}

void Wifi_not_connected(){    //show WiFi not connected logo
  delay(3000);
  display.clearDisplay();
  display.drawBitmap(48,16,myBitmapno_wifi,32,32,WHITE);
  display.display();
  delay(2000);
}
void Wifi_connected(){        //show Wifi connected logo
   display.clearDisplay();
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//顯示wifi已連接的圖示 (提示：變數名稱：myBitmapwifi)
   display.drawBitmap(48,16,myBitmapwifi,32,32,WHITE);
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
   display.display();
   delay(200);
}

void Wifi_check(){            //Check if WiFi connection is valid
    time_now = millis();
    if(time_now - Wifi_timelast > Wifi_timeout || wifi_status == false){

      Wifi_connecting();
      if(WiFi.status()== WL_CONNECTED){
        Wifi_connected();
        String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + openWeatherMapApiKey;     
        jsonBuffer = httpGETRequest(serverPath.c_str());
        Serial.println(jsonBuffer);
        myObject = JSON.parse(jsonBuffer);
        Serial.println(myObject);
        /*
        if(!isDigit((int)myObject["main"]["temp"]))
        {
          wifi_status = false;
        }
        */
        wifi_status = true;   
        // JSON.typeof(jsonVar) can be used to get the type of the var
        if (JSON.typeof(myObject) == "undefined") {
          Serial.println("Parsing input failed!");
          wifi_status = false;
          return;
        }
      }else{
        wifi_status = false;
        Serial.println("WiFi Disconnected");
        Wifi_not_connected();
      }
      
      Wifi_timelast = millis();
    }
}

void temp_setup(){                    //Draw Temperature logo
  display.clearDisplay();
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//顯示溫度圖示 (提示：display.drawBitmap(坐標X, 坐標Y, 名稱, 長度, 闊度, 顏色(WHITE))  ) 
//(提示：坐標X為8, 坐標Y為6, 名稱為Temperature_bitmap, 長度為32, 闊度為32)
  display.drawBitmap(8,6,Temperature_bitmap,32,32,WHITE);
//(提示：坐標X為0, 坐標Y為40, myBitmapnoun_top_arrow, 長度為24, 闊度為24)
  display.drawBitmap(0,40,myBitmapnoun_top_arrow,24,24,WHITE);
//(提示：坐標X為64, 坐標Y為40, myBitmapnoun_down_arrow, 長度為24, 闊度為24)
  display.drawBitmap(64,40,myBitmapnoun_down_arrow,24,24,WHITE);
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
  display.display();
}
void temp_main(){                   //Draw and read temperature in temperature page
  if(wifi_status==true){
    display.setCursor(40,14);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.print(((double)myObject["main"]["temp"]-273),1);
    display.print(" C");
    display.drawRect(92,14,4,4,WHITE);
    display.setCursor(28,44);
    if((double)myObject["main"]["temp_max"]>(int)myObject["main"]["temp_max"]){
      display.print((int)myObject["main"]["temp_max"]-273+1);   //Round up
    }else{
      display.print((int)myObject["main"]["temp_max"]-273);
    }    
    display.drawRect(52,44,4,4,WHITE);
    display.setCursor(92,44);
    display.print((int)myObject["main"]["temp_min"]-273);  
    display.drawRect(116,44,4,4,WHITE);
  }else{
    display.setTextSize(2);
    display.setCursor(52,14);   
    display.print("---");
    display.setCursor(28,44);   
    display.print("--");
    display.setCursor(92,44);   
    display.print("--");
  }
  display.display();
}

String httpGETRequest(const char* serverName) {     //helper function for WiFi check
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {                         //httpResponseCode > 0, no error
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}


void heartbeat_main(){                          // main function for reading heartrate and calculate SpO2
  
  uint32_t ir, red , green;
  double fred, fir;
  double SpO2 = 0; //raw SpO2 before low pass filtered
  time_now = millis();

#ifdef USEFIFO
  particleSensor.check(); //Check the sensor, read up to 3 samples

  while (particleSensor.available()) {//do we have new data
#ifdef MAX30105
   red = particleSensor.getFIFORed(); //Sparkfun's MAX30105
    ir = particleSensor.getFIFOIR();  //Sparkfun's MAX30105
#else
    red = particleSensor.getFIFOIR(); //why getFOFOIR output Red data by MAX30102 on MH-ET LIVE breakout board
    ir = particleSensor.getFIFORed(); //why getFIFORed output IR data by MAX30102 on MH-ET LIVE breakout board
#endif
    i++;
    fred = (double)red;
    fir = (double)ir;
    avered = avered * frate + (double)red * (1.0 - frate);//average red level by low pass filter
    aveir = aveir * frate + (double)ir * (1.0 - frate); //average IR level by low pass filter
    sumredrms += (fred - avered) * (fred - avered); //square sum of alternate component of red level
    sumirrms += (fir - aveir) * (fir - aveir);//square sum of alternate component of IR level
    if ((i % SAMPLING) == 0) {//slow down graph plotting speed for arduino Serial plotter by thin out
      if ( millis() > TIMETOBOOT) {
        float ir_forGraph = (2.0 * fir - aveir) / aveir * SCALE;
        float red_forGraph = (2.0 * fred - avered) / avered * SCALE;
        int Ebpm = HRM_estimator(fir, aveir); //Ebpm is estimated BPM
        //trancation for Serial plotter's autoscaling
        if ( ir_forGraph > 100.0) ir_forGraph = 100.0;
        if ( ir_forGraph < 80.0) ir_forGraph = 80.0;
        if ( red_forGraph > 100.0 ) red_forGraph = 100.0;
        if ( red_forGraph < 80.0 ) red_forGraph = 80.0;
        //        Serial.print(red); Serial.print(","); Serial.print(ir);Serial.print(".");
        if (ir < FINGER_ON) {
          ESpO2 = MINIMUM_SPO2; //indicator for finger detached
          placeFINGER();
          fingerState = false;
          beat = 1;
          count=-1;
          lock = false;
          drawBeat(5,CLEAR);
          drawBeat(4,CLEAR);
          drawBeat(3,CLEAR);
          drawBeat(2,CLEAR);
          drawBeat(1,CLEAR);
        }
        if (Ebpm< 50) Ebpm = 50;
        if(aveir < FINGER_ON && count < 0){
          //tone(BUZZER, 2000);
        }
        if(ESpO2>100) ESpO2 = 100;
        if(ir > FINGER_ON) {
          if(fingerState == false) {      //check finger state change
            display.fillRect(5,34,111,14,BLACK); //clear screen portion
            fingerState = true;
            display.setCursor(8,34);
            display.setTextSize(2);
            display.print((int)Ebpm);  //show temporary BPM value
            display.setCursor(64,34);
            display.print((int)ESpO2);  //show temporary SpO2 value
            display.print("%"); //display the % symbol
          }
          drawBPM((int)Ebpm);     //display BPM values
          drawSPO2((int)ESpO2);   //display SpO2 values
        }else{
          //digitalWrite(BUZZER,LOW);
        }
        //Serial.print(Ebpm);
        
        Serial.print(",");Serial.print(ir_forGraph); // to display pulse wave at the same time with SpO2 data
        Serial.print(","); Serial.print(red_forGraph); // to display pulse wave at the same time with SpO2 data
        Serial.print(",");
        Serial.print(ESpO2); //low pass filtered SpO2
        Serial.print(","); Serial.print(85.0); //reference SpO2 line
        Serial.print(","); Serial.print(90.0); //warning SpO2 line
        Serial.print(","); Serial.print(95.0); //safe SpO2 line
        Serial.print(","); Serial.println(100.0); //max SpO2 line
        
      }
    }
    if ((i % Num) == 0) {
      double R = (sqrt(sumredrms) / avered) / (sqrt(sumirrms) / aveir);
      // Serial.println(R);
      SpO2 = -23.3 * (R - 0.4) + 100; //http://ww1.microchip.com/downloads/jp/AppNotes/00001525B_JP.pdf
      ESpO2 = FSpO2 * ESpO2 + (1.0 - FSpO2) * SpO2;//low pass filter
      //  Serial.print(SpO2);Serial.print(",");Serial.println(ESpO2);
      sumredrms = 0.0; sumirrms = 0.0; i = 0;
      break;
    }
    particleSensor.nextSample(); //We're finished with this sample so move to next sample
    //Serial.println(SpO2);
  }
#else

  while (1) {//do we have new data
#ifdef MAX30105
   red = particleSensor.getRed();  //Sparkfun's MAX30105
    ir = particleSensor.getIR();  //Sparkfun's MAX30105
#else
    red = particleSensor.getIR(); //why getFOFOIR outputs Red data by MAX30102 on MH-ET LIVE breakout board
    ir = particleSensor.getRed(); //why getFIFORed outputs IR data by MAX30102 on MH-ET LIVE breakout board
#endif
    i++;
    fred = (double)red;
    fir = (double)ir;
    avered = avered * frate + (double)red * (1.0 - frate);//average red level by low pass filter
    aveir = aveir * frate + (double)ir * (1.0 - frate); //average IR level by low pass filter
    sumredrms += (fred - avered) * (fred - avered); //square sum of alternate component of red level
    sumirrms += (fir - aveir) * (fir - aveir);//square sum of alternate component of IR level
    if ((i % SAMPLING) == 0) {//slow down graph plotting speed for arduino IDE toos menu by thin out
      //#if 0
      if ( millis() > TIMETOBOOT) {
        float ir_forGraph = (2.0 * fir - aveir) / aveir * SCALE;
        float red_forGraph = (2.0 * fred - avered) / avered * SCALE;
        //trancation for Serial plotter's autoscaling
        if ( ir_forGraph > 100.0) ir_forGraph = 100.0;
        if ( ir_forGraph < 80.0) ir_forGraph = 80.0;
        if ( red_forGraph > 100.0 ) red_forGraph = 100.0;
        if ( red_forGraph < 80.0 ) red_forGraph = 80.0;
        //        Serial.print(red); Serial.print(","); Serial.print(ir);Serial.print(".");
        if (ir < FINGER_ON) ESpO2 = MINIMUM_SPO2; //indicator for finger detached
        if(ESpO2>100) ESpO2 = 100;
        
        Serial.print((2.0 * fir - aveir) / aveir * SCALE); // to display pulse wave at the same time with SpO2 data
        Serial.print(","); Serial.print((2.0 * fred - avered) / avered * SCALE); // to display pulse wave at the same time with SpO2 data
        Serial.print(","); Serial.print(ESpO2); //low pass filtered SpO2
        Serial.print(","); Serial.print(85.0); //
        Serial.print(","); Serial.print(90.0); //warning SpO2 line
        Serial.print(","); Serial.print(95.0); //safe SpO2 line
        Serial.print(","); Serial.println(100.0); //max SpO2 line
        
        //#endif
      }
    }
    if ((i % Num) == 0) {
      double R = (sqrt(sumredrms) / avered) / (sqrt(sumirrms) / aveir);
      // Serial.println(R);
      SpO2 = -23.3 * (R - 0.4) + 100; //http://ww1.microchip.com/downloads/jp/AppNotes/00001525B_JP.pdf
      ESpO2 = FSpO2 * ESpO2 + (1.0 - FSpO2) * SpO2;
      //  Serial.print(SpO2);Serial.print(",");Serial.println(ESpO2);
      sumredrms = 0.0; sumirrms = 0.0; i = 0;
      break;
    }
    particleSensor.nextSample(); //We're finished with this sample so move to next sample
    //Serial.println(SpO2);
  }
#endif

heartbeatAnimation();
display.display();
}
//
// Heart Rate Monitor by interval of zero crossing at falling edge
// max 180bpm - min 45bpm
#define FINGER_ON 50000 // if ir signal is lower than this , it indicates your finger is not on the sensor
#define LED_PERIOD 100 // light up LED for this period in msec when zero crossing is found for filtered IR signal
#define MAX_BPS 180
#define MIN_BPS 45
double HRM_estimator( double fir , double aveIr)                                    //Function for reading heartbeat and calculating average heartrate
{
  static double fbpmrate = 0.95; // low pass filter coefficient for HRM in bpm
  static uint32_t crosstime = 0; //falling edge , zero crossing time in msec
  static uint32_t crosstime_prev = 0;//previous falling edge , zero crossing time in msec
  static double bpm = 60.0;
  static double ebpm = 60.0;
  static double eir = 0.0; //estimated lowpass filtered IR signal to find falling edge without notch
  static double firrate = 0.85; //IR filter coefficient to remove notch , should be smaller than fRate
  static double eir_prev = 0.0;  
  
  // Heart Rate Monitor by finding falling edge
  eir = eir * firrate + fir * (1.0 - firrate); //estimated IR : low pass filtered IR signal
  if ( ((eir - aveIr) * (eir_prev - aveIr) < 0 ) && ((eir - aveIr) < 0.0)) { //find zero cross at falling edge
    crosstime = millis();//system time in msec of falling edge
    if(aveIr>FINGER_ON){
      if(lock==false){
         count=0;
         lock =true;
      }else{
        RESET = true;
      }
    }
    //Serial.print(crosstime); Serial.print(","); Serial.println(crosstime_prev);
    if ( ((crosstime - crosstime_prev ) > (60 * 1000 / MAX_BPS)) && ((crosstime - crosstime_prev ) < (60 * 1000 / MIN_BPS)) ) {
      bpm = 60.0 * 1000.0 / (double)(crosstime - crosstime_prev) ; //get bpm
      //   Serial.println("crossed");
      ebpm = ebpm * fbpmrate + (1.0 - fbpmrate) * bpm;//estimated bpm by low pass filtered
#ifdef LED_SOUND_INDICATOR
      if (aveIr > FINGER_ON) {
        digitalWrite(LEDPORT, HIGH);
        tone(BLIPSOUND - (100.0 - eSpO2) * 10.0); //when SpO2=80% BLIPSOUND drops 200Hz to indicate anormaly
      }
#endif
    } else {
      //Serial.println("faild to find falling edge");
    }
    crosstime_prev = crosstime;
  }
  eir_prev = eir;
#ifdef LED_SOUND_INDICATOR
  if (millis() > (crosstime + LED_PERIOD)) {
    if ( aveIr > FINGER_ON ) {
      digitalWrite(LEDPORT, LOW);
      noTone();
    }
  }
#endif
  return (ebpm);
}

void placeFINGER(){                                                         //Draw place finger text
  //display place finger request
  display.fillRect(5,34,111,14,BLACK); //clear screen portion
  display.setCursor(5,34);
  display.setTextSize(1);
  //-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//顯示place your finger (提示：display.print("    "))
  display.print("place your finger");
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
}

void HR_SpO2_splashScreen() {                                             //not used
  display.setCursor(35,10);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Heart Rate");
  display.setCursor(64,20);
  display.println("&");
  display.setCursor(55,30);
  display.println("SpO2");
  display.setCursor(50,50);
  display.println("begin>");
}

void drawBPM(int Ebpm){                                                  //Read user's heartbeat
  //display.setCursor();
  if(prev_Ebpm != Ebpm) { //validation
    display.fillRect(8,34,34,14,BLACK); //clear screen portion for BPM value update
    display.setCursor(8,34);
    display.setTextSize(2);
    display.print(Ebpm);  //display BPM values
  }
  prev_Ebpm = Ebpm;
}

void drawSPO2(int SpO2){                                                //Draw SpO2 value on screen
  if(prev_SpO2 != SpO2) { //validaton
    display.fillRect(64,34,47,14,BLACK);  //clear screen portion for SpO2 value update
    display.setCursor(64,34);
    display.setTextSize(2);
    display.print(SpO2);  // display SpO2 values
    display.print("%"); //display the % symbol
  }
  prev_SpO2 = SpO2;
}

void drawBeat(int stage, setting set){                                          //Helper function for hearbeatAnimation(), drawing rectangle here
  int x=120, y=55-stage*9, color;
  if(set==CLEAR){
    color = 0;
  }else{
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//顯示顏色 (提示：將變數(color)設定為 1)
    color = 1;
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
  }
  for(int a=0;a<8;a++){
    for(int b=0;b<8;b++){
      display.drawPixel(x+a,y+b,color);
    }
  }
}

void heartbeatAnimation(){                                                      //Draw the heartbeat bar on the right according to user's heartbeat
    
    if(fingerState==true&&count>=0 && (time_now-time_last > period)){
     // Serial.print("count: ");
     // Serial.println(count);
      count++;
    update_count();
    //check_reset();
    time_last = time_now;
    }
}

void update_count(){                                                          //Helper function for hearbeatAnimation 
  switch(count){
    case 1:
       drawBeat(1,SET);
       digitalWrite(LED,HIGH);
       tone(BUZZER, 2000);
    break;
    case 2:
       drawBeat(2,SET);
    break;
    case 3:
       //noTone(BUZZER);
       drawBeat(3,SET);
    break;
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//顯示上升 (提示：drawBeat(階段,模式) , 模式為 SET)
//       (提示： 緊記 case 完結時必須加 break;)
    case 4:
       drawBeat(4,SET);
    break;
    case 5:
       drawBeat(5,SET);
    break;
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
    case 6:
       drawBeat(5,SET);
    break;
    case 7:
       drawBeat(5,SET);
    break;
    case 8:
       drawBeat(5,CLEAR);
    break;
    case 9:
       drawBeat(4,CLEAR);
    break;
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//顯示下降 (提示：drawBeat(階段,模式) , 模式為 CLEAR)
//-         (提示： 緊記 case 完結時必須加 break;)
    case 10:
       drawBeat(3,CLEAR);
    break;
    case 11:
       drawBeat(2,CLEAR);
    break;
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------

    case 12:
       drawBeat(1,CLEAR);
       digitalWrite(LED,LOW);
    beat = 1;
    count=-1;
    lock = false;
    break;
  }
}
void clock_setting(){
  Serial.println("Entered clock setting");
   if(setup_pos==-1){                         //initialize temporary variables for clock setting
    temp_hour = Clock.getHour(h12, pm);
    temp_min = Clock.getMinute();
    temp_sec = Clock.getSecond();
    temp_date = Clock.getDate();
    temp_month = Clock.getMonth(Century)-1;
    temp_DoW = Clock.getDoW()-1;
    setup_pos = 0;
   }
   if(digitalRead(R_BUTTON)==1 && input_lock==false){
    input_lock = true;
    switch(setup_pos){
      case 0:
        if(temp_DoW<6)
          temp_DoW++;
        else
          temp_DoW= 0;
        Clock.setDoW(temp_DoW+1);
      break;
      case 1:               //month
        if(temp_hour<23)
          temp_hour++;
        else
          temp_hour= 0;
        Clock.setHour(temp_hour);
      break;
      case 2:
        if(temp_min<59)
          temp_min++;
        else
          temp_min= 0;
        Clock.setMinute(temp_min);
      break;
      case 3:
        temp_sec = 0;
        Clock.setSecond(temp_sec);
      break;   
      case 4:
        if(Clock.getMonth(Century)==2){        
          if(temp_date<28)
            temp_date++;
          else
            temp_date=0;    
        }else if(Clock.getMonth(Century)==4 ||Clock.getMonth(Century)==6 ||Clock.getMonth(Century)==9 ||Clock.getMonth(Century)==11){
          if(temp_date<30)
            temp_date++;
          else
            temp_date=0;    
        }else{
          if(temp_date<31)
            temp_date++;
          else
            temp_date=0;              
        }
        Clock.setDate(temp_date);
      break;
      case 5:
        if(temp_month<11)
          temp_month++;
        else
          temp_month=0;
      Clock.setMonth(temp_month+1);
      break;   
    }
  }
  if(digitalRead(L_BUTTON)==1 && input_lock==false){
    input_lock = true;
    switch(setup_pos){
      case 0:
        if(temp_DoW>0)
          temp_DoW--;
        else
          temp_DoW = 6;
        Clock.setDoW(temp_DoW+1);
      break;
      case 1:
        if(temp_hour>0)
          temp_hour--;
        else
          temp_hour = 23;
        Clock.setHour(temp_hour);
      break;
      case 2:
        if(temp_min>0)
          temp_min--;
        else
          temp_min = 59;
        Clock.setMinute(temp_min);
      break;
      case 3:
        if(temp_sec>0)
          temp_sec--;
        else
          temp_sec = 59;
        Clock.setSecond(temp_sec);
      break;    
      case 4:
        if(Clock.getMonth(Century)==2){        
          if(temp_date>0)
            temp_date--;
          else
            temp_date=28;    
        }else if(Clock.getMonth(Century)==4 ||Clock.getMonth(Century)==6 ||Clock.getMonth(Century)==9 ||Clock.getMonth(Century)==11){
          if(temp_date>0)
            temp_date--;
          else
            temp_date=30;    
        }else{
          if(temp_date>0)
            temp_date--;
          else
            temp_date=31;              
        }
        Clock.setDate(temp_date);
      break;
      case 5:
        if(temp_month>0)
          temp_month--;
        else
          temp_month=11;
      Clock.setMonth(temp_month+1);
      break;  
    }    
  }  
}


void clock_main() {                                            //Draw digital clock in main page

    byte chour, cmin, csec, cdate, cmonth, cDoW;
    chour = Clock.getHour(h12, pm);
    cmin = Clock.getMinute();
    csec = Clock.getSecond();
    cdate = Clock.getDate();
    cmonth = Clock.getMonth(Century)-1;
    cDoW = Clock.getDoW()-1;


    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    if(Clock.getHour(h12,pm)>=6 && Clock.getHour(h12,pm)<18){
      display.drawBitmap(111, 3, myBitmapsunny_day, 16, 16, WHITE);
    }else{
      display.drawBitmap(111, 3, myBitmapMoon, 16, 16, WHITE);
    }
    
    display.setCursor(0,0);
    display.println();
    display.print(Clock.getTemperature(),0);
    display.drawRect(14,8,3,3,WHITE);
    display.print(" C");

    time_now = millis();
    if(setup_mode==false || time_now-time_last < 300 ){            //normal mode
      printSpace_DoW();                
      display.print(week[cDoW]);
      display.println();
      display.println();
      display.print(" ");
      display.setTextSize(2);
      display.print(" ");

      if(chour<10){
        display.print("0");
      }  
      display.print(chour);
      display.print(":");

      if(cmin<10){
        display.print("0");
      }
      display.print(cmin);
      display.print(":");

      if(csec<10){
        display.print("0");
      }
      display.print(csec);
      display.setTextSize(2);
      display.println();
      display.setTextSize(1);
      display.print("     ");
      display.setTextSize(2);

      if(cdate<10){
        display.print("0");
      }
      display.print(cdate);
      display.print(" "); 
      display.print(mon[cmonth]);

    }else{                            //setup mode

      printSpace_DoW();
      if(setup_pos ==0){
        display.print("         ");
      }else{
        display.print(week[cDoW]);
      }                
      display.println();
      display.println();
      display.print(" ");
      display.setTextSize(2);
      display.print(" ");
      if(setup_pos ==1){
        display.print("  ");
      }else{
        if(chour<10){
          display.print("0");
        }  
        display.print(chour);
      }
      display.print(":");

      if(setup_pos ==2){
        display.print("  ");
      }else{
        if(cmin<10){
          display.print("0");
        }
        display.print(cmin);
      }
      display.print(":");
      if(setup_pos ==3){     
        display.print("  ");
      }else{
        if(csec<10){
          display.print("0");
        }
        display.print(csec);
      }

      display.setTextSize(2);
      display.println();
      display.setTextSize(1);
      display.print("     ");
      display.setTextSize(2);

      if(setup_pos==4){
        display.print("  ");
      }else{
        if(cdate<10){
          display.print("0");
        }
        display.print(cdate);
      }
      display.print(" "); 
      if(setup_pos==5){
        display.print("  ");
      }else{
        display.print(mon[cmonth]);  
      }  
      
      if(time_now-time_last>600){
        time_last = time_now;
      }
    }

    display.println();
    display.setTextSize(1);
    display.println();
    display.print("        ");
    display.display(); 
 
}


void printSpace_DoW(){                          //indentation for printing week
  
  //Serial.println(Clock.getDoW()-1);
  switch(Clock.getDoW()-1){
    case 0:
      display.print("    ");
      break;
    case 1:
      display.print("    ");
      break;
    case 2:
      display.print("   ");
    break;
    case 3:
      display.print("   ");
    break;
    case 4:
      display.print("   ");
    break;
    case 5:
      display.print("    ");
    break;
    case 6:
      display.print("   ");
    break;
  }
}


//Read clock display
void drawRealClock(){                                   //Draw analog clock
  
  drawSec(Clock.getSecond());
  drawMin(Clock.getMinute());
  drawHour(Clock.getHour(h12, pm), Clock.getMinute());
  display.display();
}

void drawClockDisplay()                               //Draw analog clock outline
{
  display.clearDisplay();
  // Clear screen
  //display.clearDisplay();
  
  // Draw Clockface
  for (int i=0; i<2; i++)
  {
    display.drawCircle(clockCenterX, clockCenterY, 31-i,WHITE);
  }
  for (int i=0; i<3; i++)
  {
    display.drawCircle(clockCenterX, clockCenterY, i,WHITE);
  }
  
  // Draw a small mark for every hour
  for (int i=0; i<12; i++)
  {
    drawMark(i);
  }  
  //display.display();
}

void drawMark(int h)
{
  float x1, y1, x2, y2;
  
  h=h*30;
  h=h+270;
  
  x1=29*cos(h*0.0175);
  y1=29*sin(h*0.0175);
  x2=26*cos(h*0.0175);
  y2=26*sin(h*0.0175);
  
  display.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY,WHITE);
}

void drawSec(int s)
{
  float x1, y1, x2, y2;

  s=s*6;
  s=s+270;
  
  x1=29*cos(s*0.0175);
  y1=29*sin(s*0.0175);
  x2=26*cos(s*0.0175);
  y2=26*sin(s*0.0175);
  /*
  if ((s % 5) == 0)
    display.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY,WHITE);
  else
    display.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY,WHITE);
  */
  display.drawLine(x1+clockCenterX, y1+clockCenterY, clockCenterX, clockCenterY,WHITE);
}

void drawMin(int m)
{
  float x1, y1, x2, y2, x3, y3, x4, y4;

  m=m*6;
  m=m+270;
  
  x1=25*cos(m*0.0175);
  y1=25*sin(m*0.0175);
  x2=3*cos(m*0.0175);
  y2=3*sin(m*0.0175);
  x3=10*cos((m+8)*0.0175);
  y3=10*sin((m+8)*0.0175);
  x4=10*cos((m-8)*0.0175);
  y4=10*sin((m-8)*0.0175);
  
  display.drawLine(x1+clockCenterX, y1+clockCenterY, x3+clockCenterX, y3+clockCenterY,WHITE);
  display.drawLine(x3+clockCenterX, y3+clockCenterY, x2+clockCenterX, y2+clockCenterY,WHITE);
  display.drawLine(x2+clockCenterX, y2+clockCenterY, x4+clockCenterX, y4+clockCenterY,WHITE);
  display.drawLine(x4+clockCenterX, y4+clockCenterY, x1+clockCenterX, y1+clockCenterY,WHITE);
}

void drawHour(int h, int m)
{
  float x1, y1, x2, y2, x3, y3, x4, y4;

  h=(h*30)+(m/2);
  h=h+270;
  
  x1=20*cos(h*0.0175);
  y1=20*sin(h*0.0175);
  x2=3*cos(h*0.0175);
  y2=3*sin(h*0.0175);
  x3=8*cos((h+12)*0.0175);
  y3=8*sin((h+12)*0.0175);
  x4=8*cos((h-12)*0.0175);
  y4=8*sin((h-12)*0.0175);
  
  display.drawLine(x1+clockCenterX, y1+clockCenterY, x3+clockCenterX, y3+clockCenterY,WHITE);
  display.drawLine(x3+clockCenterX, y3+clockCenterY, x2+clockCenterX, y2+clockCenterY,WHITE);
  display.drawLine(x2+clockCenterX, y2+clockCenterY, x4+clockCenterX, y4+clockCenterY,WHITE);
  display.drawLine(x4+clockCenterX, y4+clockCenterY, x1+clockCenterX, y1+clockCenterY,WHITE);
}

void weather_setup(){                                             //Get weather data from JSON 
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//當收不到wifi時 (提示：  檢查邊數(wifi_status)是否為 false  ,使用 if(){  )
  if(wifi_status==false){
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
    weather = "---";
    description = "No Internet";
    return;
  }
  String JSON_string = JSON.stringify(myObject);
  int weatherStart=JSON_string.indexOf("\"main\":\"");
  int weatherEnd = JSON_string.indexOf("\"",weatherStart+9); 
  weather = JSON_string.substring(weatherStart+8,weatherEnd);

  int desStart = JSON_string.indexOf("\"description\":\"");
  int desEnd = JSON_string.indexOf("\"",desStart+17);
  description = JSON_string.substring(desStart+15, desEnd);
  
  Serial.print("Start: ");Serial.println(weatherStart);
  Serial.print("End: ");Serial.println(weatherEnd);

}

void weather_main(){                                                              //Draw weather info
  display.clearDisplay();
  
  //char weather[] = myObject["weather"]["main"];
  //char description[] = myObject["weather"]["description"];
  //weather = JSON.stringify(myObject["weather"]["main"]);
  //description = JSON.stringify(myObject["weather"]["description"]);
  //Serial.print("String Obj: ");Serial.println(JSON_string);
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//當收到wifi (提示：  檢查邊數(wifi_status)是否為 true  ,使用 if(){  )
  if(wifi_status==true){
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
    
    Serial.print("Weather: ");Serial.println(weather);
    Serial.print("Description: ");Serial.println(description);
    
    if(weather=="Clear"){
        if(Clock.getHour(h12,pm)>=6 && Clock.getHour(h12,pm)<18){
          display.drawBitmap(48,0,myBitmapsunny,32,32,WHITE);
        }else{
          display.drawBitmap(48,0,myBitmapclearmoon,32,32,WHITE);
        }
    }else if(weather=="Thunderstorm"){
        display.drawBitmap(48,0,myBitmapthunder,32,32,WHITE);
    }else if(weather=="Drizzle"){  
        display.drawBitmap(48,0,myBitmapdrizzle,32,32,WHITE);
    }else if(weather=="Rain"){
        display.drawBitmap(48,0,myBitmaprainy,32,32,WHITE);
    }else if(weather=="Atmosphere"){
        display.drawBitmap(48,0,myBitmapmist,32,32,WHITE);
    }else{   //default is "Clouds" 
        display.drawBitmap(48,0,myBitmapcloudy,32,32,WHITE); 
    }
    display.setTextSize(1);
    display.setCursor(64-(description.length()/2)*6,34);
    display.print(description);
    display.drawBitmap(0,48,myBitmapwind,16,16,WHITE);
    display.drawBitmap(64+8,48,myBitmaphumidity,16,16,WHITE);
    display.setCursor(20,52);
    display.print((double)myObject["wind"]["speed"],1);
    display.print(" m/s");
    display.setCursor(64+20+10,52);
    display.print((int)myObject["main"]["humidity"]);
    display.print(" %");
  }else{    
    display.drawBitmap(48,0,myBitmapno_wifi,32,32,WHITE);           //When no internet
    display.setTextSize(1);
    display.setCursor(64-(description.length()/2)*6,34);
    display.print(description);
    display.drawBitmap(0,48,myBitmapwind,16,16,WHITE);
    display.drawBitmap(64+8,48,myBitmaphumidity,16,16,WHITE);
    display.setCursor(20+8,52);
    display.print("---");
    display.setCursor(64+20+10+8,52);
    display.print("---");
  }
  
  display.display();
}
