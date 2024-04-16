#include <iostream>
#include <string>
#include <RTClib.h>
#include "MainWatch.h"

class MainWatch : public Page{
private:
    RTC_DS3231 rtc;
    Display display;

    void getWeather() {
        // Code to retrieve weather information
        std::string weatherInfo = "Sunny";
        std::cout << "Weather: " << weatherInfo << std::endl;
    }

    void getTime() {
        // Code to retrieve current time
        std::string currentTime = "12:00 PM";
        std::cout << "Time: " << currentTime << std::endl;
    }

    void getDate() {
        // Code to retrieve current date
        std::string currentDate = "2022-01-01";
        std::cout << "Date: " << currentDate << std::endl;
    }


public:
    MainWatch() : display(display) {

    }
    void displayInformation() {
        getWeather();
        getTime();
        getDate();
    }
    
    void display()
    {
        //display.displayInformation();
    }

    int getPageNumber() {
        return 0;
    }



};