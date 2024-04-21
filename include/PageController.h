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
    // Add your public member functions here
    void nextPage();
    void previousPage();
    void displayCurrentPage();

private:
    // Add your private member variables and functions here
    static PageController* instance;
    Page* currentPage;
    Display* display;
    const int totalPages = 6;

    RTC_DS3231 rtc;
    void displayPage(int page);
};


#endif // PAGECONTROLLER_H