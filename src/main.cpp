/**
 * @file player-sd-audiokit.ino
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/examples-audiokit/player-sd-audiokit/README.md
 * Make sure that the pins are set to off, on, on, off, off
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
#include "audio.h"
#include <stdio.h>
#include "wifiCreator.h"
#include "timeTelling.h"
#include "button.h"

int L_BUTTON_PIN = 0;
int R_BUTTON_PIN = 5;

Button buttonL;
Button buttonR;

typedef enum {
    BUTTON_PRESSED,
    BUTTON_NOT_PRESSED, 
    TIME_UPDATE,
    IS_TIME,
    IS_NOT_TIME
   } Status;

void setup(){
   audioSetup();
   wifiSetup();
   buttonL.init(L_BUTTON_PIN);
	 buttonR.init(R_BUTTON_PIN);
   //autoTimeSetup();
   manualTimeSetup(7, 30, 0);
   std::vector<Status> status = {BUTTON_PRESSED, BUTTON_NOT_PRESSED, TIME_UPDATE, IS_TIME, IS_NOT_TIME};
}

void loop(){
   wifiLoop();
   manualTimeLoop();
   switch(getManualAlarmTime("07:30:30")){
      case IS_TIME:
         Serial.println("Time is Time");
         switch(buttonL.getState())
            case BUTTON_PRESSED:
               Serial.println("Button is Pressed");
               break;
            case BUTTON_NOT_PRESSED:
               //audioLoop();
               Serial.println("Button is not Pressed");
               break;
         
         break;
   }
}