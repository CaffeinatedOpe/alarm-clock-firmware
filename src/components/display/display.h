#include <FastLED.h>
#include <Arduino.h>
class Display
{
public:
	void init();
	void blankScreen();
	void clearBuffer();
	void refreshDisplay();

	void writeChar(int, int);
	void writeTime(int, int);
	void writeString(String);

	void setColor(int, int, int);
	void setDotColor(int, int, int);
	int brightness;
	typedef enum
	{
		EIGHTBYEIGHT,
		SIXBYEIGHT,
		FOURBYSEVEN
	} FontOptions;
	FontOptions font = SIXBYEIGHT;

private:
	void addTimeSeparator(int);
	CRGB buffer[32][8];
	CRGB screenLeds[32 * 8];
	CRGB color;
	CRGB dotColor;
};