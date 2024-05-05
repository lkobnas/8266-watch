#include "PageController.h"


PageController::PageController(byte displayAddress){
    display = new Display(displayAddress);
    currentPage = new MainWatch(display);
}

PageController::~PageController() {
    delete currentPage;
}


void PageController::displayCurrentPage() {
    currentPage->displayScreen();
}

void PageController::init(){
    rtc.begin();
}

void PageController::nextPage() {
    int nextpage = (currentPage->getPageNumber() % totalPages)+ 1;
    displayPage(nextpage);
}

void PageController::previousPage() {
    int previouspage = (currentPage->getPageNumber() - 1) % totalPages;
    displayPage(previouspage);
}

void PageController::displayPage(int page) {
    // Implement this function to display the given page
    delete currentPage;
    switch (page)
    {
    case 0:
        currentPage = new MainWatch(display);
        break;
    case 1:
        currentPage = new MainWatch(display);
        break;
    
    default:
        currentPage = new MainWatch(display);
    }
    currentPage->displayScreen();
}