int buttonState = 0;
int buttonPin = 5;

void buttonSetup(int pinInput){
	buttonPin = pinInput;
  pinMode(buttonPin, INPUT_PULLUP);
}

bool getButtonState() {
    buttonState = !digitalRead(buttonPin);
    return buttonState;
}