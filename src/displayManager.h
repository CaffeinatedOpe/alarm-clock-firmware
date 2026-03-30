#include "display.h"

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
	char text[4];
	int index = 0;
	sprintf(text, "%d", hour * 100 + min);
	int width = 6;
	int offset = 0;
	switch (font)
	{
	case EIGHTBYEIGHT:
		width = 8;
		break;
	case SIXBYEIGHT:
		width = 6;
		offset = 4;
		break;
	case FOURBYSEVEN:
		width = 6;
		offset = 4;
		font = SIXBYEIGHT;
		break;
	}
	if (hour < 10)
	{
		writeChar((int)text[0], index + width);
		writeChar((int)text[1], index + 2 * width);
		writeChar((int)text[2], index + 3 * width);
	}
	else
	{
		writeChar((int)text[0], 7);
		writeChar((int)text[1], 15);
		writeChar((int)text[2], 23);
	}
	addTimeSeparator();
	refreshDisplay();
}