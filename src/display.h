/// @file    Pride2015.ino
/// @brief   Animated, ever-changing rainbows.
/// @example Pride2015.ino
#include "FastLED.h"

// Pride2015
// Animated, ever-changing rainbows.
// by Mark Kriegsman

#define DATA_PIN    19
//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    12
#define BRIGHTNESS 255

#define WIDTH 32
#define HEIGHT 8

CRGB leds[NUM_LEDS];

void ledsetup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
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

void updateDisplay(uint8_t buffer[HEIGHT][WIDTH]) { 
  bool flip = true;
  int index = 0;
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      if (flip) {
        leds[index + HEIGHT - y] = buffer[WIDTH][HEIGHT];
      }
      else {
        leds[index + y] = buffer[WIDTH][HEIGHT];
      }
    }
    flip = !flip;
  }
}