#include "components/display.h"

void initScreen()
{
	screenLedsetup();
}

void writeString(String text)
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

void writeTime(int min, int hour)
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