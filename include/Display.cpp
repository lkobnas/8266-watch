#include "Display.h"


Display::Display(int address) {
    display = Adafruit_SSD1306(128, 64, &Wire);
    display.begin(SSD1306_SWITCHCAPVCC, address);
}

Display::~Display() {}

void Display::clearDisplay() {
    display.clearDisplay();
}

void Display::setCursor(int x, int y) {
    display.setCursor(x, y);
}

void Display::setTextSize(int size) {
    display.setTextSize(size);
}

void Display::setTextColor(int color) {
    display.setTextColor(color);
}

void Display::drawBitmap(int x, int y, const unsigned char bitmap[], int w, int h, int color) {
    display.drawBitmap(x, y, bitmap, w, h, color);
}

void Display::drawRect(int x, int y, int w, int h, int color) {
    display.drawRect(x, y, w, h, color);
}

void Display::print(std::string text) {
    display.print(text.c_str());
}

void Display::println(std::string text) {
    display.println(text.c_str());
}

void Display::println() {
    display.println();
}

void Display::displayScreen() {
    display.display();
}


