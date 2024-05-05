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

    // Public member functions
    void displayCurrentPage();
    void init();
    void nextPage();
    void previousPage();


private:
    // Private member variables
    Page* currentPage;
    const int totalPages = 6;
    
    // Sensors and Actuators
    Display* display;
    RTC_DS3231 rtc;

    // Private member functions
    void displayPage(int page);
    
    
};


#endif // PAGECONTROLLER_H