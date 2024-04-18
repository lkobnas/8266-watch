#include <iostream>
#include <string>
#include <RTClib.h>
#include "MainWatch.h"
#include "Bitmap.h"

const char daysOfTheWeekText[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char monthText[12][4] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};

class MainWatch : public Page{
private:
    RTC_DS3231 rtc;
    Display* display;
    DateTime now;
    uint16_t year;
    uint8_t month, day, hour, minute, second;
    uint8_t daysOfTheWeek;
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
        daysOfTheWeek = now.dayOfTheWeek();
        
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

    std::string fillSpaceForDowDisplay(int number) {
        switch (number)
        {
            case 1:
            case 2:
            case 6:
                return "    ";
            default:
                return "   ";
        }
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
    void getInformation() {
        getTemperature();
        getDateTime();

    }
    
    void display()
    {
        display->clearDisplay();
        display->setTextSize(1);
        display->setTextColor(WHITE);
        if(checkDay(hour)){
            display->drawBitmap(111, 3, myBitmapsunny_day, 16, 16, WHITE);
        }else{
            display->drawBitmap(111, 3, myBitmapMoon, 16, 16, WHITE);
        }

        fillSpaceForDowDisplay(daysOfTheWeek);
        display->print(daysOfTheWeekText[daysOfTheWeek-1]);
        display->println();
        display->println();
        display->print(" ");
        display->setTextSize(2);
        display->print(" ");

        display->print(fillZero(hour));
        display->print(":");
        display->print(fillZero(minute));
        display->print(":");
        display->print(fillZero(second));
        
        display->setTextSize(2);
        display->println();
        display->setTextSize(1);
        display->print("     ");
        display->setTextSize(2);

        display->print(fillZero(day));
        display->print(" "); 
        display->print(monthText[month-1]);     
        display->display();   
    }

    int getPageNumber() {
        return 0;
    }



};