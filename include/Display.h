#ifndef DISPLAY_H
#define DISPLAY_H

#include "Adafruit_SSD1306.h"

class Display {
public:
        Display(int address);
        ~Display();

        // Add your public member function declarations here
        void display();

        void clearDisplay();

        void setCursor(int x, int y);

        void setTextSize(int size);
    private:
        // Add your private member variables and functions here
        Adafruit_SSD1306 display;
};

#endif // DISPLAY_H