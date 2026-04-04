#include "components/audio.h"
#include <stdio.h>
#include "components/timeTelling.h"
#include "components/buttons/buttons.h"
// #include "ledRings.h"
#include "components/display/display.h"

#include <iostream>
#include <vector>

#include "htmlData.h"

using namespace std;

// don't ask
Buttons buttons = Buttons();
Display display = Display();

typedef enum
{
	BUTTON_PRESSED,
	BUTTON_NOT_PRESSED,
	TIME_UPDATE,
	IS_TIME,
	IS_NOT_TIME
} Status;

#include <WiFi.h>
#include <AsyncTCP.h>					 //https://github.com/me-no-dev/AsyncTCP using the latest dev version from @me-no-dev
#include <ESPAsyncWebServer.h> //https://github.com/me-no-dev/ESPAsyncWebServer using the latest dev version from @me-no-dev
#include <DNSServer.h>

int receivedSeconds;
int receivedMinutes;
int receivedHours;

typedef enum
{
	READY,
	UPDATETIME,
	UPDATEVOLUME,
	UPDATECOLOR,
	UPDATEDOTCOLOR
} WIFISTATE;

int receivedScreenColor[3];
int receivedScreenDotColor[3];

WIFISTATE wifiState = READY;

class CaptiveRequestHandler : public AsyncWebHandler
{
public:
	bool canHandle(__unused AsyncWebServerRequest *request) const override
	{
		return true;
	}

	void handleRequest(AsyncWebServerRequest *request)
	{
		Serial.println("Connection detected");
		Serial.println(request->url());
		if (request->method() == HTTP_GET && request->url() == "/")
		{
			request->send(200, "text/html", index_html);
		}
		else if (request->method() == HTTP_GET && request->url() == "/hotspot-detect.html")
		{
			request->send(200, "text/html", index_html);
		}
		else if (request->method() == HTTP_GET && request->url() == "/changeTime")
		{
			String inputSeconds;
			String inputMinutes;
			String inputHours;
			if (request->hasParam("seconds") && request->hasParam("minutes") && request->hasParam("hours"))
			{
				inputSeconds = request->getParam("seconds")->value();
				inputMinutes = request->getParam("minutes")->value();
				inputHours = request->getParam("hours")->value();
				wifiState = UPDATETIME;
				Serial.println(inputSeconds.toInt());
				Serial.println(inputMinutes.toInt());
				Serial.println(inputHours.toInt());
				manualTimeSetup(inputHours.toInt(), inputMinutes.toInt(), inputSeconds.toInt());
				Serial.println(getMinutes());
				Serial.println(getHours());
			}
			request->send(200, "text/plain", "OK");
		}
		else if (request->method() == HTTP_GET && request->url() == "/updateScreenBrightness")
		{
			String inputBrightness;
			if (request->hasParam("value"))
			{
				inputBrightness = request->getParam("value")->value();
				display.setBrightness(inputBrightness.toInt());
			}
			request->send(200, "text/plain", "OK");
		}
		else if (request->method() == HTTP_GET && request->url() == "/updateRingBrightness")
		{
			String inputBrightness;
			if (request->hasParam("value"))
			{
				inputBrightness = request->getParam("value")->value();
				// display.setBrightness(inputBrightness.toInt())
			}
			request->send(200, "text/plain", "OK");
		}
		else if (request->method() == HTTP_GET && request->url() == "/updateNumberColor")
		{
			String inputR;
			String inputG;
			String inputB;
			if (request->hasParam("r"), request->hasParam("g"), request->hasParam("b"))
			{
				inputR = request->getParam("r")->value();
				inputG = request->getParam("g")->value();
				inputB = request->getParam("b")->value();
				display.setColor(inputR.toInt(), inputG.toInt(), inputB.toInt());
			}
			request->send(200, "text/plain", "OK");
		}
		else if (request->method() == HTTP_GET && request->url() == "/updateDotColor")
		{
			String inputR;
			String inputG;
			String inputB;
			if (request->hasParam("r"), request->hasParam("g"), request->hasParam("b"))
			{
				inputR = request->getParam("r")->value();
				inputG = request->getParam("g")->value();
				inputB = request->getParam("b")->value();
				display.setDotColor(inputR.toInt(), inputG.toInt(), inputB.toInt());
			}
			request->send(200, "text/plain", "OK");
		}
		else
		{
			request->send(418, "text/html", "<h1>don't get your hopes up, i'm just an alarm clock</h1>");
		}
	}
};

DNSServer dnsServer;
AsyncWebServer server(80);

void wifiSetup()
{
	Serial.println();

	if (!WiFi.softAP("Miss Minutes"))
	{
		Serial.println("Soft AP creation failed.");
		while (1)
			;
	}

	dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
	dnsServer.start(53, "*", WiFi.softAPIP());
	server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);
	server.begin();
}

void updateWifi()
{
	if (wifiState == UPDATETIME)
	{
		manualTimeSetup(receivedHours, receivedMinutes, receivedSeconds);
		wifiState = READY;
	}
}

vector<void (*)()> loopFunctions = {};

void processButtons()
{
	buttons.getButtonEvents();
	for (Buttons::BUTTONEVENTS i : buttons.buttonEvents)
	{
		switch (i)
		{
		case Buttons::LEFT_PRESS:
			Serial.println("left press");
			if (audioStatus == SILENT)
			{
				startAudio();
				loopFunctions.push_back(playAudioLoop);
			}
			buttons.buttonEvents.erase(buttons.buttonEvents.begin());
			break;
		case Buttons::RIGHT_PRESS:
			Serial.println("right press");
			toggleAudioState();
			if (audioStatus == SILENT)
			{ // this will be post-state swap
				loopFunctions.erase(find(loopFunctions.begin(), loopFunctions.end(), playAudioLoop));
			}
			else
			{
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

unsigned long displayTimeMillis; // used to lower the frequency of screen updates
int oldminutes;
void updateTime()
{
	unsigned long currentmillis = millis();
	if (currentmillis - displayTimeMillis >= 1000) // updates every second, can be easily adjusted.
	{ 
		if (oldminutes != getMinutes())
		{
			display.writeTime(getMinutes(), getHours());
			oldminutes = getMinutes();
		}
		displayTimeMillis = currentmillis;
	}
}

// put things in here that only need to run every loop *sometimes*

void setup()
{
	Serial.begin(115200);
	audioSetup();
	wifiSetup();
	//  autoTimeSetup();
	manualTimeSetup(7, 30, 0);

	// ledsetup();
	buttons.init();
	display.init();
	loopFunctions.push_back(processButtons);
	loopFunctions.push_back(updateTime);
	// writeString("testing");
}

void loop()
{
	// manualTimeLoop();
	for (int i = 0; i < loopFunctions.size(); i++)
	{
		loopFunctions[i]();
	}
	dnsServer.processNextRequest();
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