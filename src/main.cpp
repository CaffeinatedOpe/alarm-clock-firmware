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

typedef enum {
    BUTTON_PRESSED, 
    TIME_UPDATE,
    IS_TIME,
    IS_NOT_TIME
   } Status;

void setup(){
   audioSetup(9000);
   wifiSetup();
   buttonSetup(5);
   //autoTimeSetup();
   manualTimeSetup(7, 30, 0);
   Status status[] = { BUTTON_PRESSED, TIME_UPDATE, IS_TIME, IS_NOT_TIME};
}

void loop(){
   wifiLoop();
   manualTimeLoop();
   switch(getManualAlarmTime("07:30:30")){
      case IS_TIME:
         //audioLoop();
         Serial.println("Button is not pressed");
         break;
   }
   // if(getManualAlarmTime("07:31:00")){
   //    Serial.println("Time is time");
   //    while(!getButtonState()){
   //       //audioLoop();
   //       Serial.println("Button is not pressed");
   //    }
   //    Serial.println("Button is pressed");
   // }
}