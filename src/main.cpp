#include "components/audio.h"
#include <stdio.h>
#include "components/timeTelling.h"
#include "components/buttons/buttons.h"
#include "components/rings/rings.h"
// #include "ledRings.h"
#include "components/display/display.h"

#include <iostream>
#include <vector>

#include "htmlData.h"

#include <WiFi.h>
#include <AsyncTCP.h>					 //https://github.com/me-no-dev/AsyncTCP using the latest dev version from @me-no-dev
#include <ESPAsyncWebServer.h> //https://github.com/me-no-dev/ESPAsyncWebServer using the latest dev version from @me-no-dev
#include <DNSServer.h>

using namespace std;

// don't ask
Buttons buttons = Buttons();
Display display = Display();
Rings ringR = Rings();
Rings ringL = Rings();

bool militaryTime = false;

typedef enum
{
	SOUNDING_ALARM,
	IDLE,
	SILENCED
} Status;
Status alarmStatus = IDLE;

// put things in here that only need to run every loop *sometimes*
// remove when not needed, add when needed. keeps loops optimized.
vector<void (*)()> loopFunctions = {};

unsigned long displayTimeMillis; // used to lower the frequency of screen updates
int oldminutes;
void updateTimeDisplay()
{
	unsigned long currentmillis = millis();
	int diff = currentmillis - displayTimeMillis;
	if (abs(diff) >= 1000) // updates every second, can be easily adjusted.
	// abs is used for the case where the millis reading hits a max and loops over, which will happen when the device is left on
	{
		if (oldminutes != getMinutes())
		{
			display.writeTime(getMinutes(), getHours(), militaryTime);
			display.refreshDisplay();
			oldminutes = getMinutes();
		}
		displayTimeMillis = currentmillis;
	}
}

unsigned long blinktimer = 0;
bool blinkState = false;
void ringBlinks()
{
	unsigned long currentTime = millis();
	if ((currentTime - blinktimer) > 250)
	{
		blinkState = !blinkState;
		if (blinkState)
		{
			ringL.fillColor();
			ringR.fillColor();
		}
		else
		{
			ringL.blank();
			ringR.blank();
		}
		ringL.refresh();
		ringR.refresh();
		blinktimer = currentTime;
	}
}

struct Time
{
	int minutes;
	int hours;
	Time(int h, int m)
	{
		hours = h;
		minutes = m;
	}
};

vector<Time> alarms = {};

// this alarm will NEVER TRIGGER with these values
// cause, y'know, time
Time nextAlarm(25, 61);

int currentAlarmIndex = -1;

// relies on the list of alarms being sorted, which is done as entries are added
void getNextAlarm()
{
	currentAlarmIndex++;
	if (currentAlarmIndex >= alarms.size())
	{
		currentAlarmIndex = 0;
	}
	nextAlarm = alarms[currentAlarmIndex];
}

void getCurrentAlarmIndex()
{ // this correctly sets the next alarm, not just the index. Alarm entries must be sorted for this to work.
	Time currentTime = Time(getHours(), getMinutes());
	for (int i = 0; i < alarms.size(); i++)
	{
		if (alarms[i].hours > currentTime.hours)
		{
			currentAlarmIndex = i;
			nextAlarm = alarms[i];
			break;
		}
		else if (alarms[i].hours == currentTime.hours && alarms[i].minutes > currentTime.minutes)
		{
			currentAlarmIndex = i;
			nextAlarm = alarms[i];
			break;
		}
	}
}

void insertAlarm(int h, int m)
{
	if (alarms.size() != 0)
	{
		bool addToEnd = true;
		for (int i = 0; i < alarms.size(); i++)
		{
			if (h < alarms[i].hours)
			{
				alarms.insert(alarms.begin() + i, Time(h, m));
				addToEnd = false;
				break;
			}
			else if (h == alarms[i].hours && m < alarms[i].minutes)
			{
				alarms.insert(alarms.begin() + i, Time(h, m));
				addToEnd = false;
				break;
			}
		}
		if (addToEnd)
		{
			alarms.push_back(Time(h, m));
		}
	}
	else
	{
		alarms.push_back(Time(h, m));
	}
	getCurrentAlarmIndex();
}

unsigned long loopPreventionMillis; // used to lower the frequency of updates
// prevents the same alarm from firing a bunch of times in a minute, and getting started after being stopped
void loopPrevention()
{
	unsigned long currentmillis = millis();
	int diff = currentmillis - loopPreventionMillis;
	if (abs(diff) >= 5000)
	// abs is used for the case where the millis reading hits a max and loops over, which will happen when the device is left on
	{
		if (getMinutes() != nextAlarm.minutes && alarmStatus == SILENCED)
		{
			alarmStatus = IDLE;
			loopFunctions.erase(find(loopFunctions.begin(), loopFunctions.end(), loopPrevention));
			ringL.blank();
			ringR.blank();
		}
		loopPreventionMillis = currentmillis;
	}
}

unsigned long alarmTimeMillis; // used to lower the frequency of screen updates
void alarmLoop()
{
	unsigned long currentmillis = millis();
	int diff = currentmillis - alarmTimeMillis;
	if (abs(diff) >= 1000) // updates every second, can be easily adjusted.
	// abs is used for the case where the millis reading hits a max and loops over, which will happen when the device is left on
	{
		if (nextAlarm.hours == getHours())
		{
			if (nextAlarm.minutes == getMinutes() && alarmStatus == IDLE)
			{
				Serial.println("Sounding Alarm");
				startAudio();
				loopFunctions.push_back(ringBlinks);
				loopFunctions.push_back(playAudioLoop);
				loopFunctions.push_back(loopPrevention);
				alarmStatus = SOUNDING_ALARM;
				getNextAlarm();
			}
		}
		alarmTimeMillis = currentmillis;
	}
}

String alarmListProcessor(const String &var)
{
	if (var == "ALARMS")
	{
		String alarmsList = "";
		for (int i = 0; i < alarms.size(); i++)
		{
			String minutes = String(alarms[i].minutes);
			if (alarms[i].minutes <= 10)
			{
				minutes = "0" + minutes;
			}
			alarmsList += "<div><p class='time'>" + String(alarms[i].hours) + ":" + minutes + "</p><button type='button' onclick='deleteAlarm(" + String(i) + ")'>Delete</button></div>";
		}
		return alarmsList;
	}
	return String();
}

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
				manualTimeSetup(inputHours.toInt(), inputMinutes.toInt(), inputSeconds.toInt());
				getCurrentAlarmIndex();
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
				display.refreshDisplay();
			}
			request->send(200, "text/plain", "OK");
		}
		else if (request->method() == HTTP_GET && request->url() == "/updateRingBrightness")
		{
			String inputBrightness;
			if (request->hasParam("value"))
			{
				inputBrightness = request->getParam("value")->value();
				ringL.setBrightness(inputBrightness.toInt());
				ringR.setBrightness(inputBrightness.toInt());
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
				display.writeTime(getMinutes(), getHours(), militaryTime);
				display.refreshDisplay();
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
				display.writeTime(getMinutes(), getHours(), militaryTime);
			}
			request->send(200, "text/plain", "OK");
		}
		else if (request->method() == HTTP_GET && request->url() == "/updateRingColor")
		{
			String inputR;
			String inputG;
			String inputB;
			if (request->hasParam("r"), request->hasParam("g"), request->hasParam("b"))
			{
				inputR = request->getParam("r")->value();
				inputG = request->getParam("g")->value();
				inputB = request->getParam("b")->value();
				ringL.setColor(inputR.toInt(), inputG.toInt(), inputB.toInt());
				ringL.refresh();
			}
			request->send(200, "text/plain", "OK");
		}
		else if (request->method() == HTTP_GET && request->url() == "/alarms")
		{
			request->send(200, "text/html", alarms_html, alarmListProcessor);
		}
		else if (request->method() == HTTP_GET && request->url() == "/addAlarm")
		{
			String inputHours;
			String inputMinutes;
			if (request->hasParam("hours"), request->hasParam("minutes"))
			{
				inputHours = request->getParam("hours")->value();
				inputMinutes = request->getParam("minutes")->value();
				insertAlarm(inputHours.toInt(), inputMinutes.toInt());
			}
			request->send(200, "text/plain", "OK");
		}
		else if (request->method() == HTTP_GET && request->url() == "/deleteAlarm")
		{
			String index;
			if (request->hasParam("index"))
			{
				index = request->getParam("index")->value();
				alarms.erase(alarms.begin() + index.toInt());
			}
			getCurrentAlarmIndex();
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
			if (alarmStatus == SOUNDING_ALARM)
			{
				loopFunctions.erase(find(loopFunctions.begin(), loopFunctions.end(), ringBlinks));
				alarmStatus = SILENCED;
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

void setup()
{
	Serial.begin(115200);
	audioSetup();
	wifiSetup();
	manualTimeSetup(7, 30, 0);

	buttons.init();
	display.init();
	ringL.initL();
	ringR.initR();
	ringL.blank();
	ringR.blank();
	loopFunctions.push_back(processButtons);
	loopFunctions.push_back(updateTimeDisplay);
	loopFunctions.push_back(alarmLoop);

	insertAlarm(7, 31);
}

void loop()
{
	for (int i = 0; i < loopFunctions.size(); i++)
	{
		loopFunctions[i]();
	}
	dnsServer.processNextRequest();
}