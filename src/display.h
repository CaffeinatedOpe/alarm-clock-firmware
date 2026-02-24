#include "LedController.hpp"
/*
 Pin assignments:
 DIN pin is connected to Arduino pin 11
 CLK pin is connected to Arduino pin 13
 CS pin is connected to Arduino pin 10
 We have only 1 MAX7219 module (cascaded devices)
*/
LedController lc = LedController(11,13,10,1); // [11 = DIN, 13 = CLK, 10 = CS, 1 = number of matrices]

void dispalySetup() {
  lc.activateAllSegments();
  /* Set the brightness to a medium values */
  lc.setIntensity(8);
  /* and clear the display */
  lc.clearMatrix();
}

void displayLoop() { 
  
}