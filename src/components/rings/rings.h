#include <FastLED.h>
#include <Arduino.h>
class Rings
{
public:
	void init();
	void blank();
	void clearBuffer();
	void refreshDisplay();

	void setColor(int, int, int);
	void setBrightness(int);
	void test();

private:
	CRGB ringLedsL[12];
	CRGB color;
};