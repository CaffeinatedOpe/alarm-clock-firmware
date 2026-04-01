#include <FastLED.h>

#include "../fonts/font_huge.h"
#include "../fonts/font_large.h"
#include "../fonts/font_small.h"

#define SCREEN_DATA_PIN 18
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define SCREEN_NUM_LEDS 256
#define BRIGHTNESS 75

#define WIDTH 32
#define HEIGHT 8

CRGB color = CRGB(255, 0, 255);
CRGB dotColor = CRGB(255,255,255);

// buffer 0x0 is the top left pixel
CRGB buffer[WIDTH][HEIGHT];

CRGB screenLeds[SCREEN_NUM_LEDS];

typedef enum
{
	EIGHTBYEIGHT,
	SIXBYEIGHT,
	FOURBYSEVEN
} FontOptions;
FontOptions font = SIXBYEIGHT;

void screenLedsetup()
{
	delay(3); // 1 second delay for recovery

	// tell FastLED about the LED strip configuration
	FastLED.addLeds<LED_TYPE, SCREEN_DATA_PIN, COLOR_ORDER>(screenLeds, SCREEN_NUM_LEDS)
			.setCorrection(TypicalLEDStrip)
			.setDither(BRIGHTNESS < 255);

	// set master brightness control
	FastLED.setBrightness(BRIGHTNESS);
}

void blankScreen()
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

void clearBuffer()
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

void writeChar(int asciiCode, int offset)
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

void test()
{
	for (int x = 0; x < WIDTH; x++)
	{
		buffer[x][0] = CRGB(0, 255, 0);
	}
}

// led matrix starts at top right, data path runs vertically
void refreshDisplay()
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

void addTimeSeparator(int height) {
	buffer[15][9-height] = dotColor;
	buffer[15][8-height] = dotColor;
	buffer[16][9-height] = dotColor;
	buffer[16][8-height] = dotColor;
	buffer[15][6] = dotColor;
	buffer[15][7] = dotColor;
	buffer[16][6] = dotColor;
	buffer[16][7] = dotColor;
}