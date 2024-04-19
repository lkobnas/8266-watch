#include <Arduino.h>
#include "PageController.h"
#include "Display.h"


Display* display;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //init
  display = new Display(0x3C);

}

void loop() {
  // put your main code here, to run repeatedly:
  PageController* pageController = PageController::getInstance(display);
  pageController->displayCurrentPage();
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}