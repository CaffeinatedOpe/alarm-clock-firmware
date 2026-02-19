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

void setup(){
   audioSetup(9000);
   wifiSetup();
   buttonSetup();
   //autoTimeSetup();
   manualTimeSetup(7, 30, 0);
}

void loop(){
   buttonSetup();
   wifiLoop();
   manualTimeLoop();
   if(getManualTime("07:31:00")){
      Serial.println("Time is time");
      while(!getButtonState()){
         Serial.println("Button is not Pressed");
         audioLoop();
      }
   }
   
}