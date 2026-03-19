/// @file    Pride2015.ino
/// @brief   Animated, ever-changing rainbows.
/// @example Pride2015.ino
#include "FastLED.h"

// Pride2015
// Animated, ever-changing rainbows.
// by Mark Kriegsman

#define SCREEN_DATA_PIN    18
//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define SCREEN_NUM_LEDS  256
#define BRIGHTNESS 255

#define WIDTH 32
#define HEIGHT 8

CRGB color = CRGB(255,255,255);

CRGB screenLeds[SCREEN_NUM_LEDS];

void screenLedsetup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,SCREEN_DATA_PIN,COLOR_ORDER>(screenLeds, SCREEN_NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void blankScreen() {
  for (int i = 0; i < SCREEN_NUM_LEDS; i++){
    screenLeds[i] = color;
  }
  FastLED.show();
}

void updateDisplay(uint8_t buffer[HEIGHT][WIDTH]) { 
  bool flip = true;
  int index = 0;
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      if (flip) {
        screenLeds[index + HEIGHT - y] = buffer[WIDTH][HEIGHT];
      }
      else {
        screenLeds[index + y] = buffer[WIDTH][HEIGHT];
      }
    }
    flip = !flip;
  }
  FastLED.show();
}