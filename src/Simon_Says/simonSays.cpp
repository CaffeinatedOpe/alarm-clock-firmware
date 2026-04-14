#include <Arduino.h>
#include "simonSays.h"

void SimonSays::setButtonPressL()
{
	if (takeUserInput && step < difficulty * 2) // check if we're in the range of taking user input
	{
		if (!steps[step - difficulty])
		{
			step++; // moves on
							// good noodle
		}
		else
		{
			// bad noodle
			// err or something
		}
	}
}

void SimonSays::setButtonPressR()
{
	if (takeUserInput && step < difficulty * 2) // check if we're in the range of taking user input
	{
		if (steps[step - difficulty])
		{
			step++; // moves on
							// good noodle
		}
		else
		{
			// bad noodle
			// err or something
		}
	}
}

void SimonSays::simonInit()
{
	step = 0;
	ringState = false;
	steps = {};

	for (int i = 0; i <= difficulty; i++)
	{
		int val = random() % 2;
		if (val == 0)
		{
			steps.push_back(true);
		}
		else
		{
			steps.push_back(false);
		}
	}
}

void SimonSays::simonloop(bool currentButtonState)
{
	unsigned long currentmillis = millis();
	int diff = currentmillis - timeSinceChange;
	if (step < difficulty)
	{
		if (diff > delay)
		{
			if (currentButtonState == false)
			{
				ringControlFunc(true, steps[step]);
				currentButtonState == true;
			}
			else
			{
				ringControlFunc(false, steps[step]);
				step++;
				currentButtonState = false;
			}
			timeSinceChange = currentmillis;
		}
	}
	if (step == difficulty * 2)
	{
		step = 0;
		finishFunction();
	}
}