#include <Arduino.h>
class LittleGuy
{
public:
	void guyHappy();
	void guySad();
	void guyInit();
	void renderGuy();
	int guyBodyColor[3] = {255, 50, 50};
	int guyOutlineColor[3] = {250, 0, 0};

private:
	int happiness = 5;
	int guyBuffer[32][8][4];
	int guyBody[8][8] = {

	};
	int guyOutline[8][8] = {
		
	};
	void renderGuy();
};