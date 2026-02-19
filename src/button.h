const int buttonPin = 2;

int buttonState = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // initialize pushbutton pin with internal pull-up
}

void loop() {
  buttonState = digitalRead(buttonPin); // read the state of the pushbutton value
}

bool getButtonState() {
    return buttonState == LOW;
}