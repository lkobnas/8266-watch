#include "MainWatch.h"


const char daysOfTheWeekText[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char monthText[12][4] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};


MainWatch::MainWatch(Display* display) : display(display) {
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }
    if (rtc.lostPower()) {
        Serial.println("RTC lost power, let's set the time!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}
void MainWatch::getInformation() {
    getTemperature();
    getDateTime();
}

void MainWatch::displayScreen()
{
    display->clearDisplay();
    display->setTextSize(1);
    display->setTextColor(WHITE);
    if(checkDay(hour)){
        display->drawBitmap(111, 3, myBitmapsunny_day, 16, 16, 1);
    }else{
        display->drawBitmap(111, 3, myBitmapMoon, 16, 16, 1);
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
    display->displayScreen();   
}

int MainWatch::getPageNumber() {
    return 0;
}

void MainWatch::getTemperature() {
    // Code to retrieve weather information
    temperature = rtc.getTemperature();
}

void MainWatch::getDateTime() {
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

bool MainWatch::checkDay(uint8_t hour){
    if(hour >= 6 && hour < 18){
        return true;
    } 
    return false;
}

std::string MainWatch::fillZero(int number) {
    std::string s = std::to_string(number);
    if (number < 10) {
        s = "0" + s;
    }
    return s;
}

std::string MainWatch::fillSpaceForDowDisplay(int number) {
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




