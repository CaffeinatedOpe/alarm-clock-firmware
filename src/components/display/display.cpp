#include "display.h"
#include <FastLED.h>

#include "fonts/font_huge.h"
#include "fonts/font_large.h"
#include "fonts/font_small.h"

#define SCREEN_DATA_PIN 18
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define SCREEN_NUM_LEDS 256
#define BRIGHTNESS 75

#define WIDTH 32
#define HEIGHT 8

void Display::init(){
	delay(20); // 1 second delay for recovery

	// tell FastLED about the LED strip configuration
	FastLED.addLeds<LED_TYPE, SCREEN_DATA_PIN, COLOR_ORDER>(screenLeds, SCREEN_NUM_LEDS)
			.setCorrection(TypicalLEDStrip)
			.setDither(BRIGHTNESS < 255);

	// set master brightness control
	FastLED.setBrightness(BRIGHTNESS);
	color = CRGB(0, 255, 255);
	dotColor = CRGB(255, 225, 255);
}

void Display::blankScreen()
{
	CRGB off = CRGB(0, 0, 0);
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			buffer[x][y] = off;
		}
	}
	for (int i = 0; i < SCREEN_NUM_LEDS; i++)
	{
		screenLeds[i] = off;
	}
	FastLED.show();
}

void Display::clearBuffer()
{
	CRGB off = CRGB(0, 0, 0);
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			buffer[x][y] = off;
		}
	}
}

void Display::writeChar(int asciiCode, int offset)
{
	switch (font)
	{
	case EIGHTBYEIGHT:
		for (int y = 0; y < 8; y++)
		{
			bool bits[8];
			for (int i = 0; i != 8; i++)
			{
				bits[i] = (console_font_8x8[(asciiCode * 8) + y] & (1 << i));
			}
			for (int x = 0; x < 8; x++)
			{
				if (bits[7 - x])
				{
					buffer[x + offset][y] = color;
				}
				else
				{
					buffer[x + offset][y] = CRGB(0, 0, 0);
				}
			}
		}
		break;
	case SIXBYEIGHT:
		for (int y = 0; y < 8; y++)
		{
			bool bits[8];
			for (int i = 0; i != 8; i++)
			{
				bits[i] = (console_font_6x8[(asciiCode * 8) + y] & (1 << i));
			}
			for (int x = 0; x < 6; x++)
			{
				if (bits[7 - x])
				{
					buffer[x + offset][y+1] = color;
				}
				else
				{
					buffer[x + offset][y+1] = CRGB(0, 0, 0);
				}
			}
		}
		break;
	case FOURBYSEVEN:
		for (int y = 0; y < 7; y++)
		{
			bool bits[8];
			for (int i = 0; i != 8; i++)
			{
				bits[i] = (console_font_4x7[(asciiCode * 7) + y] & (1 << i));
			}
			for (int x = 0; x < 4; x++)
			{
				if (bits[7 - x])
				{
					buffer[x + offset][y+1] = color;
				}
				else
				{
					buffer[x + offset][y+1] = CRGB(0, 0, 0);
				}
			}
		}
		break;
	}
}

// led matrix starts at top right, data path runs vertically
void Display::refreshDisplay()
{
	bool flip = true;
	int index = 0;
	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			if (!flip)
			{
				screenLeds[index] = buffer[WIDTH - x - 1][y];
			}
			else
			{
				screenLeds[index] = buffer[WIDTH - x - 1][HEIGHT - y - 1];
			}
			index++;
		}
		flip = !flip;
	}
	FastLED.show();
}

void Display::addTimeSeparator(int height) {
	buffer[15][9-height] = dotColor;
	buffer[15][8-height] = dotColor;
	buffer[16][9-height] = dotColor;
	buffer[16][8-height] = dotColor;
	buffer[15][6] = dotColor;
	buffer[15][7] = dotColor;
	buffer[16][6] = dotColor;
	buffer[16][7] = dotColor;
}

void Display::writeString(String text)
{
	int index = 0;
	clearBuffer();
	for (int i = 0; i < text.length(); i++)
	{
		int character = (int)text[i];
		writeChar(character, index);
		switch (font)
		{
		case EIGHTBYEIGHT:
			index = index + 8;
			break;
		case SIXBYEIGHT:
			index = index + 6;
			break;
		case FOURBYSEVEN:
			index = index + 4;
			break;
		}
	}
	refreshDisplay();
}

void Display::writeTime(int min, int hour)
{
	clearBuffer();
	char text[4];
	int index = 0;
	sprintf(text, "%d", hour * 100 + min);
	int width = 8;
	int gap = 0;
	int height = 8;
	switch (font)
	{
	case EIGHTBYEIGHT:
		break;
	case SIXBYEIGHT:
		width = 6;
		index = 4;
		gap = 1;
		height = 7;
		break;
	case FOURBYSEVEN:
		width = 6;
		index = 4;
		gap = 1;
		height = 7;
		font = SIXBYEIGHT;
		break;
	}
	if (hour < 10)
	{
		writeChar((int)text[0], index + width - gap);
		writeChar((int)text[1], index + gap + (2 * width));
		writeChar((int)text[2], index + gap + (3 * width));
	}
	else
	{
		writeChar((int)text[0], index - gap);
		writeChar((int)text[1], index - gap + width);
		writeChar((int)text[2], index + gap + (2 * width));
		writeChar((int)text[3], index + gap + (3 * width));
	}
	addTimeSeparator(height);
	refreshDisplay();
}

void Display::setColor(int r, int g, int b) {
	color = CRGB(r, g, b);
}
void Display::setDotColor(int r, int g, int b) {
	dotColor = CRGB(r, g, b);
}

void Display::setBrightness(int brightness) {
	FastLED.setBrightness(brightness);
}