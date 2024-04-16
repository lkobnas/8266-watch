#include "Display.h"

class Display {
private:
    Adafruit_SSD1306 display;

public:
    Display(int address) {
        Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);
        display.begin(SSD1306_SWITCHCAPVCC, address);
    }

    ~Display() {}

    void clearDisplay() {
        display.clearDisplay();
    }

    void setCursor(int x, int y) {
        display.setCursor(x, y);
    }

    void setTextSize(int size) {
        display.setTextSize(size);
    }

    void display() {
        display.clearDisplay();
        //Code here


        display.display();
    }

};
