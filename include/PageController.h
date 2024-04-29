#ifndef PAGECONTROLLER_H
#define PAGECONTROLLER_H

#include <iostream>
#include <RTClib.h>
#include "Page.h"
#include "Display.h"
#include "MainWatch.h"



class PageController {
public:
    PageController(byte displayAddress);
    ~PageController();
    static PageController* getInstance(Display* d);

    // Public member functions
    void nextPage();
    void previousPage();
    void displayCurrentPage();
    void init();

private:
    // Private member variables
    static PageController* instance;
    Page* currentPage;
    const int totalPages = 6;
    
    // Sensors and Actuators
    Display* display;
    RTC_DS3231 rtc;

    // Private member functions
    void displayPage(int page);
    
    
};


#endif // PAGECONTROLLER_H