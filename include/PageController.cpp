#include <iostream>

#include "PageController.h"


class PageController {
private:
    Page* currentPage;
    Display* display;
    const int totalPages = 6;
    
    public:
        PageController(Display* d){
            display = d;
            currentPage = new MainWatch(display);
        }

        ~PageController() {
            delete currentPage;
        }

    void nextPage() {
        int nextpage = (currentPage->getPageNumber() % totalPages)+ 1;
        displayPage(nextpage);
    }

    void previousPage() {
        int previouspage = (currentPage->getPageNumber() - 1) % totalPages;
        displayPage(previouspage);
    }

    void displayPage(int page) {
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
        }
        currentPage->display();
    }
};