#include "littleGuy.h"

void LittleGuy::guyInit()
{
	happiness = 5;
	guyWorkingBodyColor[0] = guyBodyColor[0];
	guyWorkingBodyColor[1] = guyBodyColor[1];
	guyWorkingBodyColor[2] = guyBodyColor[2];
	guyWorkingOutlineColor[0] = guyOutlineColor[0];
	guyWorkingOutlineColor[1] = guyOutlineColor[1];
	guyWorkingOutlineColor[2] = guyOutlineColor[2];
}

void LittleGuy::guySad()
{
	happiness -= 1;
	if (happiness <= 1)
	{
		happiness = 1;
	}
}

void LittleGuy::guyHappy()
{
	if (happiness < 5)
	{
		happiness += 1;
	}
}

void LittleGuy::drawGuy()
{
	guyWorkingBodyColor[0] = guyBodyColor[0];
	guyWorkingBodyColor[1] = guyBodyColor[1];
	guyWorkingBodyColor[2] = guyBodyColor[2];
	Serial.println(guyWorkingBodyColor[0]);
	Serial.println(guyWorkingBodyColor[1]);
	Serial.println(guyWorkingBodyColor[2]);
	guyWorkingOutlineColor[0] = guyOutlineColor[0];
	guyWorkingOutlineColor[1] = guyOutlineColor[1];
	guyWorkingOutlineColor[2] = guyOutlineColor[2];
	switch (happiness)
	{

	case 5:
		renderGuy(SMILE, HALF);
		break;
	case 4:
		renderGuy(FULLFLAT, FULL);
		break;
	case 3:
		renderGuy(SHORTFLAT, HALF);
		break;
	case 2:
		renderGuy(FULLFLAT, ANGRY);
		break;
	case 1:
		guyWorkingBodyColor[0] = 255;
		guyWorkingBodyColor[1] = 255;
		guyWorkingBodyColor[2] = 255;
		guyWorkingOutlineColor[0] = 255;
		guyWorkingOutlineColor[1] = 255;
		guyWorkingOutlineColor[2] = 255;
		renderGuy(FULLFLAT, FULL);
		break;
	}
}

void LittleGuy::renderGuy(MOUTHSTATE mouth, EYESTATE eye)
{
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (guyBody[y][x] == 1)
			{
				guyBuffer[x][y][0] = guyWorkingBodyColor[0];
				guyBuffer[x][y][1] = guyWorkingBodyColor[1];
				guyBuffer[x][y][2] = guyWorkingBodyColor[2];
				guyBuffer[x][y][3] = 1;
			}
			else if (guyOutline[y][x] == 1)
			{
				guyBuffer[x][y][0] = guyWorkingOutlineColor[0];
				guyBuffer[x][y][1] = guyWorkingOutlineColor[1];
				guyBuffer[x][y][2] = guyWorkingOutlineColor[2];
				guyBuffer[x][y][3] = 1;
			}
			else
			{
				guyBuffer[x][y][3] = 0;
			}
		}
	}
	switch (mouth)
	{
	case SHORTFLAT:
		guyBuffer[3][7][0] = 0;
		guyBuffer[3][7][1] = 0;
		guyBuffer[3][7][2] = 0;
		guyBuffer[3][7][3] = 1;
		guyBuffer[4][7][0] = 0;
		guyBuffer[4][7][1] = 0;
		guyBuffer[4][7][2] = 0;
		guyBuffer[4][7][3] = 1;
		break;
	case FULLFLAT:
		guyBuffer[2][7][0] = 0;
		guyBuffer[2][7][1] = 0;
		guyBuffer[2][7][2] = 0;
		guyBuffer[2][7][3] = 1;
		guyBuffer[3][7][0] = 0;
		guyBuffer[3][7][1] = 0;
		guyBuffer[3][7][2] = 0;
		guyBuffer[3][7][3] = 1;
		guyBuffer[4][7][0] = 0;
		guyBuffer[4][7][1] = 0;
		guyBuffer[4][7][2] = 0;
		guyBuffer[4][7][3] = 1;
		break;
	case SMILE:
		guyBuffer[2][7][0] = 0;
		guyBuffer[2][7][1] = 0;
		guyBuffer[2][7][2] = 0;
		guyBuffer[2][7][3] = 1;
		guyBuffer[3][7][0] = 0;
		guyBuffer[3][7][1] = 0;
		guyBuffer[3][7][2] = 0;
		guyBuffer[3][7][3] = 1;
		guyBuffer[4][7][0] = 0;
		guyBuffer[4][7][1] = 0;
		guyBuffer[4][7][2] = 0;
		guyBuffer[4][7][3] = 1;
		guyBuffer[5][6][0] = 0;
		guyBuffer[5][6][1] = 0;
		guyBuffer[5][6][2] = 0;
		guyBuffer[5][6][3] = 1;
		break;
	}
	switch (eye)
	{
	case FULL:
		guyBuffer[1][4][0] = 0;
		guyBuffer[1][4][1] = 0;
		guyBuffer[1][4][2] = 0;
		guyBuffer[1][4][3] = 1;
		guyBuffer[1][5][0] = 0;
		guyBuffer[1][5][1] = 0;
		guyBuffer[1][5][2] = 0;
		guyBuffer[1][5][3] = 1;
		guyBuffer[4][4][0] = 0;
		guyBuffer[4][4][1] = 0;
		guyBuffer[4][4][2] = 0;
		guyBuffer[4][4][3] = 1;
		guyBuffer[4][5][0] = 0;
		guyBuffer[4][5][1] = 0;
		guyBuffer[4][5][2] = 0;
		guyBuffer[4][5][3] = 1;
		guyBuffer[2][4][0] = 255;
		guyBuffer[2][4][1] = 255;
		guyBuffer[2][4][2] = 255;
		guyBuffer[2][4][3] = 1;
		guyBuffer[2][5][0] = 255;
		guyBuffer[2][5][1] = 255;
		guyBuffer[2][5][2] = 255;
		guyBuffer[2][5][3] = 1;
		guyBuffer[5][4][0] = 255;
		guyBuffer[5][4][1] = 255;
		guyBuffer[5][4][2] = 255;
		guyBuffer[5][4][3] = 1;
		guyBuffer[5][5][0] = 255;
		guyBuffer[5][5][1] = 255;
		guyBuffer[5][5][2] = 255;
		guyBuffer[5][5][3] = 1;
		break;
	case HALF:
		guyBuffer[1][4][0] = guyAccentColor[0];
		guyBuffer[1][4][1] = guyAccentColor[1];
		guyBuffer[1][4][2] = guyAccentColor[2];
		guyBuffer[1][4][3] = 1;
		guyBuffer[1][5][0] = 0;
		guyBuffer[1][5][1] = 0;
		guyBuffer[1][5][2] = 0;
		guyBuffer[1][5][3] = 1;
		guyBuffer[4][4][0] = guyAccentColor[0];
		guyBuffer[4][4][1] = guyAccentColor[1];
		guyBuffer[4][4][2] = guyAccentColor[2];
		guyBuffer[4][4][3] = 1;
		guyBuffer[4][5][0] = 0;
		guyBuffer[4][5][1] = 0;
		guyBuffer[4][5][2] = 0;
		guyBuffer[4][5][3] = 1;
		guyBuffer[2][4][0] = guyAccentColor[0];
		guyBuffer[2][4][1] = guyAccentColor[1];
		guyBuffer[2][4][2] = guyAccentColor[2];
		guyBuffer[2][4][3] = 1;
		guyBuffer[2][5][0] = 255;
		guyBuffer[2][5][1] = 255;
		guyBuffer[2][5][2] = 255;
		guyBuffer[2][5][3] = 1;
		guyBuffer[5][4][0] = guyAccentColor[0];
		guyBuffer[5][4][1] = guyAccentColor[1];
		guyBuffer[5][4][2] = guyAccentColor[2];
		guyBuffer[5][4][3] = 1;
		guyBuffer[5][5][0] = 255;
		guyBuffer[5][5][1] = 255;
		guyBuffer[5][5][2] = 255;
		guyBuffer[5][5][3] = 1;
		break;
	case ANGRY:
		guyBuffer[1][4][0] = guyAccentColor[0];
		guyBuffer[1][4][1] = guyAccentColor[1];
		guyBuffer[1][4][2] = guyAccentColor[2];
		guyBuffer[1][4][3] = 1;
		guyBuffer[1][5][0] = 0;
		guyBuffer[1][5][1] = 0;
		guyBuffer[1][5][2] = 0;
		guyBuffer[1][5][3] = 1;
		guyBuffer[4][4][0] = guyAccentColor[0];
		guyBuffer[4][4][1] = guyAccentColor[1];
		guyBuffer[4][4][2] = guyAccentColor[2];
		guyBuffer[4][4][3] = 1;
		guyBuffer[4][5][0] = 0;
		guyBuffer[4][5][1] = 0;
		guyBuffer[4][5][2] = 0;
		guyBuffer[4][5][3] = 1;
		guyBuffer[2][4][0] = guyAccentColor[0];
		guyBuffer[2][4][1] = guyAccentColor[1];
		guyBuffer[2][4][2] = guyAccentColor[2];
		guyBuffer[2][4][3] = 1;
		guyBuffer[2][5][0] = 255;
		guyBuffer[2][5][1] = 255;
		guyBuffer[2][5][2] = 255;
		guyBuffer[2][5][3] = 1;
		guyBuffer[5][4][0] = guyAccentColor[0];
		guyBuffer[5][4][1] = guyAccentColor[1];
		guyBuffer[5][4][2] = guyAccentColor[2];
		guyBuffer[5][4][3] = 1;
		guyBuffer[5][5][0] = 255;
		guyBuffer[5][5][1] = 255;
		guyBuffer[5][5][2] = 255;
		guyBuffer[5][5][3] = 1;
		guyBuffer[1][3][0] = guyAccentColor[0];
		guyBuffer[1][3][1] = guyAccentColor[1];
		guyBuffer[1][3][2] = guyAccentColor[2];
		guyBuffer[1][3][3] = 1;
		guyBuffer[5][3][0] = guyAccentColor[0];
		guyBuffer[5][3][1] = guyAccentColor[1];
		guyBuffer[5][3][2] = guyAccentColor[2];
		guyBuffer[5][3][3] = 1;
		break;
	}
	guyBuffer[1][0][0] = guyAccentColor[0];
	guyBuffer[1][0][1] = guyAccentColor[1];
	guyBuffer[1][0][2] = guyAccentColor[2];
	guyBuffer[1][0][3] = 1;
	guyBuffer[7][0][0] = guyAccentColor[0];
	guyBuffer[7][0][1] = guyAccentColor[1];
	guyBuffer[7][0][2] = guyAccentColor[2];
	guyBuffer[7][0][3] = 1;
}