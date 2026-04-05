#include "rings.h"
#include <FastLED.h>

#define SCREEN_DATA_PIN 23
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define RING_NUM_LEDS 12
#define BRIGHTNESS 75

void Rings::init(){
	delay(20); // 1 second delay for recovery

	// tell FastLED about the LED strip configuration
	FastLED.addLeds<LED_TYPE, SCREEN_DATA_PIN, COLOR_ORDER>(ringLedsL, RING_NUM_LEDS)
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
		ringLedsL[i] = off;
	}
}

void Rings::test() {
	for (int i = 0; i < RING_NUM_LEDS; i++) {
		ringLedsL[i] = CRGB(0, 255, 255);
		FastLED.show();
	}
}

void Rings::setColor(int r, int g, int b) {
	color = CRGB(r, g, b);
}

void Rings::setBrightness(int brightness) {
	FastLED.setBrightness(brightness);
}