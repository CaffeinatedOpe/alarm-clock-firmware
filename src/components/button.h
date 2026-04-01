class Button
{
private:
	int pin;

public:
	void init(int pinInput)
	{
		pin = pinInput;
		pinMode(pin, INPUT_PULLUP);
	}

	bool getState()
	{
		return !digitalRead(pin);
	}
};

typedef enum{
	LEFT,
	RIGHT,
	BOTH,
	NONE
} BUTTONSTATE;

#define button_l 22
#define button_r 5

void button_init() {
	pinMode(button_l, INPUT_PULLUP);
	pinMode(button_r, INPUT_PULLUP);
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
	else if (!digitalRead(button_l)){
		return RIGHT;
	}
	else{
		return NONE;
	}
	return NONE; //just to get rid of a warning squiggle
}