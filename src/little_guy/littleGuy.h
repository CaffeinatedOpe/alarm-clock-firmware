#include <Arduino.h>
class LittleGuy
{
public:
	void guyHappy();
	void guySad();
	void guyInit();
	void renderGuy();
	void drawGuy();
	int guyBodyColor[3] = {255, 50, 50};
	int guyOutlineColor[3] = {250, 0, 0};
	int guyAccentColor[3] = {0, 0, 255};

	typedef enum
	{
		HAPPY,
		NEUTRAL,
		ANNOYED,
		TIRED
	} EMOTIONS;

	typedef enum
	{
		FULL,
		HALF,
		ANGRY,
		NONE
	} EYESTATE;

	typedef enum
	{
		SMILE,
		FULLFLAT,
		SHORTFLAT,
		NONE
	} MOUTHSTATE;

private:
	int happiness = 5;

	// buffer is [x][y][red, green, blue, alpha]
	int guyBuffer[8][8][4];
	int guyBody[8][8] = {
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 1, 1, 1, 1, 1, 0},
			{0, 1, 1, 1, 1, 1, 1, 0},
			{0, 1, 1, 1, 1, 1, 1, 0},
			{0, 1, 1, 1, 1, 1, 1, 0},
			{0, 1, 1, 1, 1, 1, 1, 0}};
	int guyOutline[8][8] = {
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 1, 0, 0, 0, 0, 0, 1},
			{0, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 1}};
	void renderGuy(MOUTHSTATE, EYESTATE);
};