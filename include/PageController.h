#ifndef PAGECONTROLLER_H
#define PAGECONTROLLER_H

#include <iostream>
#include <RTClib.h>
#include "Page.h"
#include "Display.h"
#include "MainWatch.h"



class PageController {
public:
    PageController(Display* d);
    ~PageController();
    static PageController* getInstance(Display* d);
    // Add your public member functions here
    void nextPage();
    void previousPage();

private:
    // Add your private member variables and functions here
    int currentPage;
    int totalPages;
};


#endif // PAGECONTROLLER_H