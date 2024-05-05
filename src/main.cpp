#include <Arduino.h>
#include "PageController.h"

PageController* pageController = new PageController(0x3C);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  //pageController->getInstance(display);
  //pageController->displayCurrentPage();
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}