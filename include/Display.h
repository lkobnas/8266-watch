#ifndef DISPLAY_H
#define DISPLAY_H

#include "Adafruit_SSD1306.h"
#include <string>

class Display {
public:
        Display(int address);
        ~Display();

        // Add your public member function declarations here
        void clearDisplay();

        void setCursor(int x, int y);

        void setTextSize(int size);

        void setTextColor(int color);

        void drawBitmap(int x, int y, const unsigned char bitmap[], int w, int h, int color);

        void drawRect(int x, int y, int w, int h, int color);

        void print(std::string text);

        void println(std::string text);

        void println();

        void displayScreen();

        private:
            // Add your private member variables and functions here
            Adafruit_SSD1306 display;
};

#endif // DISPLAY_H