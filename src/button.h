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