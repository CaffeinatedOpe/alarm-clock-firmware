int buttonState = 0;
int buttonPin = 0;

void buttonSetup(int buttonPin){
  buttonPin = buttonPin;
  pinMode(buttonPin, INPUT_PULLUP); // initialize pushbutton pin with internal pull-up
}

void buttonLoop() {
  buttonState = digitalRead(buttonPin); // read the state of the pushbutton value
}

bool getButtonState() {
    buttonState = digitalRead(buttonPin);
    return buttonState == LOW;
}