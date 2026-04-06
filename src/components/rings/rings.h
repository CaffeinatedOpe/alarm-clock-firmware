#include <FastLED.h>
#include <Arduino.h>
class Rings
{
public:
	void initL();
	void initR();
	void blank();
	void clearBuffer();
	void refreshDisplay();
	void fillColor();

	void setColor(int, int, int);
	void setBrightness(int);
	void test();
	void refresh();

private:
	CRGB leds[12];
	CRGB color;
};