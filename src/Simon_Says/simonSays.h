#include "components/buttons/buttons.h"

bool buttonState;

void simonsetup(){

}

void simonloop(bool currentButtonState){
    currentButtonState = buttonState;
}