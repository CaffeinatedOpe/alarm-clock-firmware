/// @file    Pride2015.ino
/// @brief   Animated, ever-changing rainbows.
/// @example Pride2015.ino
#include "FastLED.h"
#include "characters.h"

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

CRGB color = CRGB(255,0,255);

//buffer 0x0 is the top left pixel
CRGB buffer[WIDTH][HEIGHT];

CRGB screenLeds[SCREEN_NUM_LEDS];

void screenLedsetup() {
  delay(1); // 1 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,SCREEN_DATA_PIN,COLOR_ORDER>(screenLeds, SCREEN_NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void blankScreen() {
	CRGB off = CRGB(0,0,0);
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			buffer[x][y] = off;
		}
	}
  for (int i = 0; i < SCREEN_NUM_LEDS; i++){
    screenLeds[i] = off;
  }
  FastLED.show();
}

// digits are in [y][x], while buffer is in [x][y]
// offset is the number of pixels from the right of the screen, or it at least should be.
void writeNum(int digit, int offset) {
	//characters are 6x8
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 7; x++) {
			if(digits[digit][y][x]) {
				buffer[x + offset][y] = color;
			}
			else{
				buffer[x + offset][y] = CRGB(0,0,0);
			}
		}
	}
}

// led matrix starts at top right, data path runs vertically
void refreshDisplay() { 
  bool flip = true;
  int index = 0;
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      if (flip) {
        screenLeds[index] = buffer[WIDTH - x][y];
      }
      else {
        screenLeds[index] = buffer[WIDTH - x][HEIGHT - y];
      }
			index++;
    }
    flip = !flip;
  }
  FastLED.show();
}