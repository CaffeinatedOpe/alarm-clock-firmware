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

	void setColor();
	void setBrightness(int);
	void test();
	void refresh();

	int ringR;
	int ringG;
	int ringB;

private:
	CRGB leds[12];
	CRGB color;
};