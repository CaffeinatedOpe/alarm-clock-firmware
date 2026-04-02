#include "components/audio.h"
#include <stdio.h>
#include "components/wifiCreator.h"
#include "components/timeTelling.h"
#include "components/button.h"
// #include "ledRings.h"
#include "displayManager.h"

#include <iostream>
#include <vector>

using namespace std;
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

void processButtons()
{
	getButtonEvents();
	for (BUTTONEVENTS i : buttonEvents)
	{
		switch (i)
		{
		case LEFT_PRESS:
			player.setActive(true);
			buttonEvents.erase(buttonEvents.begin());
			Serial.println("left press");
			break;
		case RIGHT_PRESS:
			player.next();
			Serial.println(player.audioInfo());
			buttonEvents.erase(buttonEvents.begin());
			Serial.println("right press");
			break;
		case LEFT_RELEASE:
			buttonEvents.erase(buttonEvents.begin());
			Serial.println("left release");
			break;
		case RIGHT_RELEASE:
			buttonEvents.erase(buttonEvents.begin());
			Serial.println("right release");
			break;
		}
	}
}

//put things in here that only need to run every loop *sometimes*
vector<void(*)()> loopFunctions = {audioPeriodic, processButtons, updateWifi};

void setup()
{
	Serial.begin(115200);
	audioSetup();
	wifiSetup();
	//  autoTimeSetup();
	manualTimeSetup(7, 30, 0);

	// ledsetup();
	initScreen();
	button_init();
	// writeString("testing");
	player.setActive(false);
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