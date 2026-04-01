#include <vector>
#include <iostream>
using namespace std;

typedef enum{
	LEFT,
	RIGHT,
	BOTH,
	NONE
} BUTTONSTATE;

typedef enum{
	LEFT_PRESS,
	RIGHT_PRESS,
	LEFT_RELEASE,
	RIGHT_RELEASE
} BUTTONEVENTS;

#define button_l 22
#define button_r 5

bool button_l_state = false;
bool button_r_state = false;

vector<BUTTONEVENTS> buttonEvents = {};

void button_init() {
	pinMode(button_l, INPUT_PULLUP);
	pinMode(button_r, INPUT_PULLUP);
}

void getButtonEvents() {
	if (!digitalRead(button_l) != button_l_state) {
		if (!digitalRead(button_l)) {
			buttonEvents.push_back(LEFT_PRESS);
		}
		else {
			buttonEvents.push_back(LEFT_RELEASE);
		}
		button_l_state = !digitalRead(button_l);
	}
	if (!digitalRead(button_r) != button_r_state) {
		if (!digitalRead(button_r)) {
			buttonEvents.push_back(RIGHT_PRESS);
		}
		else {
			buttonEvents.push_back(RIGHT_RELEASE);
		}
		button_r_state = !digitalRead(button_r);
	}
}

BUTTONSTATE getButtons() {
	if (!digitalRead(button_l)){
		if (!digitalRead(button_r)){
			return BOTH;
		}
		else{
			return LEFT;
		}
	}
	else if (!digitalRead(button_r)){
		return RIGHT;
	}
	else{
		return NONE;
	}
	return NONE; //just to get rid of a warning squiggle
}