#include <iostream>

class PageController {
private:
    int currentPage;
    int totalPages = 1;

public:
    PageController() {
        currentPage = 1;
    }
    
    ~PageController() {}

    void nextPage() {
        currentPage++;
        if (currentPage > totalPages) {
            currentPage = 1;
        }
        // Call a function to display the current page
        displayPage(currentPage);
    }

    void previousPage() {
        currentPage--;
        if (currentPage < 1) {
            currentPage = totalPages;
        }
        // Call a function to display the current page
        displayPage(currentPage);
    }

    void displayPage(int page) {
        // Implement this function to display the given page
        // You can create separate classes for each page and call their display functions here
        // For now, you can simply print the page number
        std::cout << "Displaying Page " << page << std::endl;
    }
};