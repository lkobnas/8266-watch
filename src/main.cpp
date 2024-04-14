#include <Arduino.h>
#include "PageController.h"

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  int result = myFunction(2, 3);
  Serial.println(result);
  PageController pageController;
  pageController.nextPage();

}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}