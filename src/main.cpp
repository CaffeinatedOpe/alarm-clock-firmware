#include "components/audio.h"
#include <stdio.h>
#include "components/wifiCreator.h"
#include "components/timeTelling.h"
#include "components/buttons/buttons.h"
// #include "ledRings.h"
#include "displayManager.h"

#include <iostream>
#include <vector>

using namespace std;

//don't ask
Buttons buttons = Buttons();

typedef enum
{
	BUTTON_PRESSED,
	BUTTON_NOT_PRESSED,
	TIME_UPDATE,
	IS_TIME,
	IS_NOT_TIME
} Status;

void updateWifi() {
	if (wifiState == UPDATETIME) {
		manualTimeSetup(receivedHours, receivedMinutes, receivedSeconds);
		wifiState = READY;
	}
}

vector<void(*)()> loopFunctions = {};

void processButtons()
{
	buttons.getButtonEvents();
	for (Buttons::BUTTONEVENTS i : buttons.buttonEvents)
	{
		switch (i)
		{
		case Buttons::LEFT_PRESS:
			Serial.println("left press");
			if (audioStatus==SILENT){
				startAudio();
				loopFunctions.push_back(playAudioLoop);
			}
			buttons.buttonEvents.erase(buttons.buttonEvents.begin());
			break;
		case Buttons::RIGHT_PRESS:
			Serial.println("right press");
			toggleAudioState();
			if (audioStatus==SILENT){ //this will be post-state swap
				loopFunctions.erase(find(loopFunctions.begin(), loopFunctions.end(), playAudioLoop));
			} else {
				loopFunctions.push_back(playAudioLoop);
			}
			buttons.buttonEvents.erase(buttons.buttonEvents.begin());
			break;
		case Buttons::LEFT_RELEASE:
			Serial.println("left release");
			buttons.buttonEvents.erase(buttons.buttonEvents.begin());
			break;
		case Buttons::RIGHT_RELEASE:
			Serial.println("right release");
			buttons.buttonEvents.erase(buttons.buttonEvents.begin());
			break;
		}
	}
}

//put things in here that only need to run every loop *sometimes*

void setup()
{
	Serial.begin(115200);
	audioSetup();
	wifiSetup();
	//  autoTimeSetup();
	manualTimeSetup(7, 30, 0);

	// ledsetup();
	initScreen();
	buttons.init();
	loopFunctions.push_back(processButtons); 
	loopFunctions.push_back(updateWifi);
	// writeString("testing");
}

void loop()
{
	wifiLoop();
	// manualTimeLoop();
	writeTime(getMinutes(), getHours());
	for (int i = 0; i < loopFunctions.size(); i++) {	
		loopFunctions[i]();
	}
	/*switch(getManualAlarmTimeCompare("07:30:30")){
		 case IS_TIME:
				Serial.println("Time is Time");
				switch(buttonL.getState())
					 case BUTTON_PRESSED:
							Serial.println("Button is Pressed");
							break;
					 case BUTTON_NOT_PRESSED:
							Serial.println("Button is not Pressed");
							break;

				break;
	}*/
}