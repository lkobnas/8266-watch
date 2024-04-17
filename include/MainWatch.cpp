#include <iostream>
#include <string>
#include <RTClib.h>
#include "MainWatch.h"
#include "Bitmap.h"

const char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

class MainWatch : public Page{
private:
    RTC_DS3231 rtc;
    Display* display;
    DateTime now;
    uint16_t year;
    uint8_t month, day, hour, minute, second;
    char* daysOfTheWeek;
    float temperature;

    void getTemperature() {
        // Code to retrieve weather information
        temperature = rtc.getTemperature();
    }

    void getDateTime() {
        // Code to retrieve current time
        now = rtc.now();
        
        year = now.year();
        month = now.month();
        day = now.day();
        daysOfTheWeek = &daysOfTheWeek[now.dayOfTheWeek()];
        
        hour = now.hour();
        minute = now.minute();
        second = now.second();
    } 

    bool checkDay(uint8_t hour){
        if(hour >= 6 && hour < 18){
            return true;
        } 
        return false;
    }

    std::string fillZero(int number) {
        std::string s = std::to_string(number);
        if (number < 10) {
            s = "0" + s;
        }
        return s;
    }

public:
    MainWatch(Display* display) : display(display) {
        if (!rtc.begin()) {
            Serial.println("Couldn't find RTC");
            while (1);
        }
        if (rtc.lostPower()) {
            Serial.println("RTC lost power, let's set the time!");
            rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        }
    }
    void displayInformation() {
        getTemperature();
        getDateTime();

    }
    
    void display()
    {
        //display.displayInformation();
    }

    int getPageNumber() {
        return 0;
    }



};