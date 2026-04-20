#include "littleGuy.h"

void LittleGuy::guyInit()
{
	happiness = 5;
}

void LittleGuy::guySad()
{
	happiness -= 1;
}

void LittleGuy::guyHappy()
{
	if (happiness < 5)
	{
		happiness += 1;
	}
}

void LittleGuy::drawGuy(){
	switch (happiness) {

		case  5:
		renderGuy(SMILE, FULL);
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
				guyBuffer[x][y][0] = guyBodyColor[0];
				guyBuffer[x][y][1] = guyBodyColor[1];
				guyBuffer[x][y][2] = guyBodyColor[2];
				guyBuffer[x][y][3] = 1;
			}
			else if (guyOutline[y][x] == 1)
			{
				guyBuffer[x][y][0] = guyOutlineColor[0];
				guyBuffer[x][y][1] = guyOutlineColor[1];
				guyBuffer[x][y][2] = guyOutlineColor[2];
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
}