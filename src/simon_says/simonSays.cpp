#include <Arduino.h>
#include "simonSays.h"

void SimonSays::setButtonPressL()
{
	if (takeUserInput && (step < difficulty * 2)) // check if we're in the range of taking user input
	{
		if (!steps[step - difficulty])
		{
			Serial.println("correct");
			step++; // moves on
							// good noodle
		}
		else
		{
			step = 0;
			Serial.println("incorrect");
			// bad noodle
			// err or something
		}
	}
}

void SimonSays::setButtonPressR()
{
	if (takeUserInput && (step < difficulty * 2)) // check if we're in the range of taking user input
	{
		if (steps[step - difficulty])
		{
			Serial.println("correct");
			step++; // moves on
							// good noodle
		}
		else
		{
			step = 0;
			Serial.println("incorrect");
			// bad noodle
			// err or something
		}
	}
}

void SimonSays::simonInit()
{
	step = 0;
	Serial.println("init simon");
	ringState = false;
	takeUserInput = false;
	steps = {};

	for (int i = 0; i <= difficulty; i++)
	{
		long val = millis() % 2;
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

void SimonSays::simonloop()
{
	unsigned long currentmillis = millis();
	int diff = currentmillis - timeSinceChange;
	if (step < difficulty)
	{
		if (diff > delay)
		{
			Serial.println("loop simon");
			if (!ringState)
			{
				ringControlFunc(true, steps[step]);
				ringState = true;
			}
			else
			{
				ringControlFunc(false, steps[step]);
				Serial.println("aaaaa");
				step++;
				ringState = false;
			}
			timeSinceChange = currentmillis;
			Serial.println(step);
		}
		}
	if (step == difficulty){
		takeUserInput = true;
	}
	if (step == (difficulty * 2))
	{
		finishFunction();
		takeUserInput = false;
	}
}