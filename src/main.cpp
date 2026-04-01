#include "components/audio.h"
#include <stdio.h>
#include "components/wifiCreator.h"
#include "components/timeTelling.h"
#include "components/button.h"
// #include "ledRings.h"
#include "displayManager.h"

int L_BUTTON_PIN = 22;
int R_BUTTON_PIN = 5;

Button buttonL;
Button buttonR;

typedef enum
{
	BUTTON_PRESSED,
	BUTTON_NOT_PRESSED,
	TIME_UPDATE,
	IS_TIME,
	IS_NOT_TIME
} Status;

void setup()
{
	Serial.begin(115200);
	audioSetup();
	//wifiSetup();
	buttonL.init(L_BUTTON_PIN);
	buttonR.init(R_BUTTON_PIN);
	// autoTimeSetup();
	manualTimeSetup(7, 30, 0);
	std::vector<Status> status = {BUTTON_PRESSED, BUTTON_NOT_PRESSED, TIME_UPDATE, IS_TIME, IS_NOT_TIME};
	//ledsetup();
	initScreen();
	//writeString("testing");
}

void loop()
{
	wifiLoop();
	//manualTimeLoop();
	writeTime(getMinutes(), getHours());

	//audioPeriodic();
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