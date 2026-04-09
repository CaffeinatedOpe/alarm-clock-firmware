#include "rings.h"
#include <FastLED.h>

#define PIN_L 5
#define PIN_R 23
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define RING_NUM_LEDS 12
#define BRIGHTNESS 75

void Rings::initL(){
	delay(20); // 1 second delay for recovery

	// tell FastLED about the LED strip configuration
	FastLED.addLeds<LED_TYPE, PIN_L, COLOR_ORDER>(leds, RING_NUM_LEDS)
			.setCorrection(TypicalLEDStrip)
			.setDither(BRIGHTNESS < 255);

	// set master brightness control
	FastLED.setBrightness(BRIGHTNESS);
	color = CRGB(0, 255, 255);
}
void Rings::initR(){
	delay(20); // 1 second delay for recovery

	// tell FastLED about the LED strip configuration
	FastLED.addLeds<LED_TYPE, PIN_R, COLOR_ORDER>(leds, RING_NUM_LEDS)
			.setCorrection(TypicalLEDStrip)
			.setDither(BRIGHTNESS < 255);

	// set master brightness control
	FastLED.setBrightness(BRIGHTNESS);
	color = CRGB(0, 255, 255);
}

void Rings::blank()
{
	CRGB off = CRGB(0, 0, 0);
	for (int i = 0; i < RING_NUM_LEDS; i++)
	{
		leds[i] = off;
	}
}

void Rings::test() {
	for (int i = 0; i < RING_NUM_LEDS; i++) {
		leds[i] = CRGB(0, 255, 255);
		FastLED.show();
	}
}

void Rings::fillColor() {
	for (int i = 0; i < RING_NUM_LEDS;i++) {
		leds[i]	= color;
	}
}

void Rings::setColor(int r, int g, int b) {
	color = CRGB(r, g, b);
	ringR = r;
	ringG = g;
	ringB = b;
}

void Rings::setBrightness(int brightness) {
	FastLED.setBrightness(brightness);
}

void Rings::refresh() {
	FastLED.show();
}