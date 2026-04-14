#include <Arduino.h>
#include <iostream>
#include <vector>
#include <stdio.h>
class SimonSays
{

public:
	int difficulty = 3;
	int delay = 500; // ms

	void (*ringControlFunc)(bool, bool);
	void (*finishFunction)();
	void setButtonPressL();
	void setButtonPressR();
	void simonInit();
	void simonloop(bool currentButtonState);
	bool takeUserInput;

private:
	bool ringState = false;
	int step = 0;
	std::vector<bool> steps = {};
	unsigned long timeSinceChange;
};