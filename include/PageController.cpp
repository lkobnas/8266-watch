#include <iostream>

#include "PageController.h"


class PageController {
    private:
    static PageController* instance;
        Page* currentPage;
        Display* display;
        const int totalPages = 6;

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
        
    public:
        PageController(Display* d){
            display = d;
            currentPage = new MainWatch(display);
        }

        ~PageController() {
            delete currentPage;
        }

        // Delete the copy constructor and assignment operator
        PageController(const PageController&) = delete;
        PageController& operator=(const PageController&) = delete;

        // Provide a static method to get the instance of the class
        static PageController* getInstance(Display* d) {
            if (!instance) {
                instance = new PageController(d);
            }
            return instance;
        }

        void nextPage() {
            int nextpage = (currentPage->getPageNumber() % totalPages)+ 1;
            displayPage(nextpage);
        }

        void previousPage() {
            int previouspage = (currentPage->getPageNumber() - 1) % totalPages;
            displayPage(previouspage);
        }

};