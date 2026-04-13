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
	void writeTime(int, int, bool);
	void writeString(String);

	void setColor();
	void setDotColor();
	void setBrightness(int);
	typedef enum
	{
		EIGHTBYEIGHT,
		SIXBYEIGHT,
		FOURBYSEVEN
	} FontOptions;
	FontOptions font = SIXBYEIGHT;

	int brightness;

	int numberR;
	int numberG;
	int numberB;
	int dotR;
	int dotG;
	int dotB;

private:
	void addTimeSeparator(int);
	CRGB buffer[32][8];
	CRGB screenLeds[32 * 8];
	CRGB color;
	CRGB dotColor;
};