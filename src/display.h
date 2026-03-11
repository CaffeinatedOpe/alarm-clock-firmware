#include "LedController.hpp"
/*
 Pin assignments:
 DIN pin is connected to Arduino pin 11
 CLK pin is connected to Arduino pin 13
 CS pin is connected to Arduino pin 10
 We have only 1 MAX7219 module (cascaded devices)
*/
LedController lc = LedController(11,13,10,1); // [11 = DIN, 13 = CLK, 10 = CS, 1 = number of matrices]

int MATRIX_PIN = 17;

ByteBlock digits[10] = {
  {
    B00000000,
    B00011000,
    B00100100,
    B01000010,
    B01000010,
    B00100100,
    B00011000,
    B00000000
  }, {
    B00000000,
    B00011100,
    B00101100,
    B01001100,
    B00001100,
    B00001100,
    B00001100,
    B00000000
  }, {
    B00000000,
    B00111000,
    B01101100,
    B00011000,
    B00110000,
    B01100000,
    B01111110,
    B00000000
  }, {
    B00000000,
    B00111100,
    B01100110,
    B00001100,
    B00000110,
    B01100110,
    B00111100,
    B00000000
  }, {
    B00000000,
    B01100000,
    B01100000,
    B01101000,
    B01111110,
    B00001000,
    B00001000,
    B00000000
  }, {
    B00000000,
    B01111110,
    B01100000,
    B01111000,
    B00000110,
    B01100110,
    B00111100,
    B00000000
  }, {
    B00000000,
    B00001100,
    B00111000,
    B01100000,
    B01111100,
    B01100110,
    B00111100,
    B00000000
  }, {
    B00000000,
    B01111110,
    B00000110,
    B00001100,
    B00011000,
    B00110000,
    B01100000,
    B00000000
  }, {
    B00000000,
    B00111100,
    B00100100,
    B00011000,
    B01100110,
    B01000010,
    B00111100,
    B00000000
  }, {
    B00000000,
    B00111100,
    B01100110,
    B00111110,
    B00000110,
    B00011100,
    B00110000,
    B00000000
  }
};

void dispalySetup() {
  
  pinMode(13, OUTPUT);
    
  for(unsigned int i = 0; i < 10; i++){
    lc.rotate180(digits[i],&digits[i]);
  }
}

void setLEDs (unsigned int number) {

  unsigned int places[4];

  for(unsigned int i = 0;i < 4;i++){
    unsigned int divisor = 1;
    for(unsigned int j=0;j < i;j++){
      divisor *= 10;
    }

    places[3-i] = number/divisor % 10;
    lc.displayOnSegment(3-i,digits[places[3-i]]);
  }

}

void switchLED(){

  static bool LEDON = false;
  if(LEDON){
    digitalWrite(13, LOW);
  }else{
    digitalWrite(13, HIGH);
  }
  LEDON = !LEDON;
}

void displayLoop() { 

  lc.clearMatrix();
  
  for (unsigned int i = 0; i<10000; i++) {
    delay(500);
    switchLED();
    setLEDs(i);
  }
}