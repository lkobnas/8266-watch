#include <Arduino.h>
#include "PageController.h"
#include "Display.h"

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //init

  Display* display = new Display(0x3C);
  PageController* pageController = new PageController(display);
  pageController->nextPage();

}

void loop() {
  // put your main code here, to run repeatedly:
  PageController* pageController = PageController::getInstance(display);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}