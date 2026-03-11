int buttonState = 0;
int buttonPin = 5;

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
		buttonState = !digitalRead(pin);
		return buttonState;
	}
};