#ifndef MAINWATCH_H
#define MAINWATCH_H

#include <iostream>
#include <string>
#include <RTClib.h>
#include "Page.h"
#include "Display.h"
#include "Bitmap.h"

class MainWatch : public Page{

    public:
        MainWatch(Display* d);

        void getInformation();
        void displayScreen();
        int getPageNumber() override;

    private:
        Display* d
        RTC_DS3231 rtc;
        DateTime now;
        uint16_t year;
        uint8_t month, day, hour, minute, second;
        uint8_t daysOfTheWeek;
        float temperature;

        void getTemperature();

        void getDateTime();
        bool checkDay(uint8_t hour);
        std::string fillZero(int number);
        std::string fillSpaceForDowDisplay(int number);

};

#endif // MAINWATCH_H