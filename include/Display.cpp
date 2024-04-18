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

    void setTextColor(int color) {
        display.setTextColor(color);
    }

    void drawBitmap(int x, int y, const unsigned char bitmap[], int w, int h, int color) {
        display.drawBitmap(x, y, bitmap, w, h, color);
    }

    void drawRect(int x, int y, int w, int h, int color) {
        display.drawRect(x, y, w, h, color);
    }

    void print(std::string text) {
        display.print(text.c_str());
    }

    void println(std::string text) {
        display.println(text.c_str());
    }

    void println() {
        display.println();
    }

    void display() {
        display.display();
    }

};
