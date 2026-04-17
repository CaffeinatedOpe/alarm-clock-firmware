#include "components/audio.h"
#include <stdio.h>
#include "components/timeTelling.h"
#include "components/buttons/buttons.h"
#include "components/rings/rings.h"
#include "components/display/display.h"
#include "Simon_Says/simonSays.h"

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
SimonSays simon = SimonSays();

bool militaryTime = false;
bool simonSaysActive = true;
void (*buttonLFunc)();
void (*buttonRFunc)();

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

void setRingState(bool state, bool side)
{
	Serial.println("setting ring state");
	if (!side)
	{
		if (state)
		{
			ringL.fillColor();
		}
		else
		{
			ringL.blank();
		}
		ringL.refresh();
	}
	else
	{
		if (state)
		{
			ringR.fillColor();
		}
		else
		{
			ringR.blank();
		}
		ringR.refresh();
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

void noop()
{
}

vector<Time> alarms = {};

// this alarm will NEVER TRIGGER with these values
// cause, y'know, time
Time nextAlarm(25, 61);

int currentAlarmIndex = -1;

void readAlarms()
{
	File alarmsFile;

	if (SD.exists("/alarms.txt"))
	{
		alarmsFile = SD.open("/alarms.txt", FILE_READ);
		while (alarmsFile.available())
		{
			int hour = alarmsFile.readStringUntil('\n').toInt();
			int minutes = alarmsFile.readStringUntil('\n').toInt();
			alarms.push_back(Time(hour, minutes));
			Serial.println("added time:");
			Serial.print(hour);
			Serial.println(minutes);
		}
		alarmsFile.close();
	}
}

void writeAlarms()
{
	File alarmsFile;
	if (SD.exists("/alarms.txt"))
	{
		SD.remove("/alarms.txt");
	}
	Serial.println("writing file");
	alarmsFile = SD.open("/alarms.txt", FILE_WRITE);

	for (int i = 0; i < alarms.size(); i++)
	{
		alarmsFile.println(alarms[i].hours);
		alarmsFile.println(alarms[i].minutes);

		Serial.println(alarms[i].hours);
		Serial.println(alarms[i].minutes);
	}

	alarmsFile.close();
}

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
	writeAlarms();
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

void stopAlarm()
{
	Serial.println("stopping alarms");
	loopFunctions.erase(find(loopFunctions.begin(), loopFunctions.end(), ringBlinks));
	alarmStatus = SILENCED;
	stopAudio();
	loopFunctions.erase(find(loopFunctions.begin(), loopFunctions.end(), playAudioLoop));
	buttonLFunc = noop;
	buttonRFunc = noop;
}

void simonLoop()
{
	simon.simonloop();
}
void simonL()
{
	simon.setButtonPressL();
}
void simonR()
{
	simon.setButtonPressR();
}
void simonEnd()
{
	loopFunctions.erase(find(loopFunctions.begin(), loopFunctions.end(), simonLoop));
	stopAlarm();
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
				loopFunctions.push_back(playAudioLoop);
				loopFunctions.push_back(loopPrevention);
				alarmStatus = SOUNDING_ALARM;
				getNextAlarm();
				if (simonSaysActive)
				{
					buttonLFunc = simonL;
					buttonRFunc = simonR;
					simon.simonInit();
					loopFunctions.push_back(simonLoop);
				}
				else
				{
					buttonLFunc = stopAlarm;
					buttonRFunc = stopAlarm;
					loopFunctions.push_back(ringBlinks);
				}
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
void writeConfig()
{
	File configFile;
	if (SD.exists("/config.txt"))
	{
		SD.remove("/config.txt");
	}
	Serial.println("writing file");
	configFile = SD.open("/config.txt", FILE_WRITE);

	configFile.println(display.brightness);

	configFile.println(display.numberR);
	configFile.println(display.numberG);
	configFile.println(display.numberB);

	configFile.println(display.dotR);
	configFile.println(display.dotG);
	configFile.println(display.dotB);

	configFile.println(ringL.ringR);
	configFile.println(ringL.ringG);
	configFile.println(ringL.ringB);

	if (simonSaysActive)
	{
		configFile.println("1");
	}
	else
	{
		configFile.println("0");
	}
	configFile.println();

	configFile.close();
}

void initConfig()
{
	File configFile;
	if (SD.exists("/config.txt"))
	{
		SD.remove("/config.txt");
	}
	Serial.println("writing file");
	configFile = SD.open("/config.txt", FILE_WRITE);

	configFile.println(75);

	configFile.println(255);
	configFile.println(255);
	configFile.println(0);

	configFile.println(255);
	configFile.println(255);
	configFile.println(255);

	configFile.println(0);
	configFile.println(0);
	configFile.println(255);

	configFile.println(0.5);
	configFile.println(1);

	configFile.close();
}

void readConfig()
{
	File configFile;

	if (!SD.exists("/config.txt"))
	{
		initConfig();
	}
	configFile = SD.open("/config.txt", FILE_READ);
	Serial.println("reading file");

	display.brightness = configFile.readStringUntil('\n').toInt();
	display.numberR = configFile.readStringUntil('\n').toInt();
	display.numberG = configFile.readStringUntil('\n').toInt();
	display.numberB = configFile.readStringUntil('\n').toInt();
	display.dotR = configFile.readStringUntil('\n').toInt();
	display.dotG = configFile.readStringUntil('\n').toInt();
	display.dotB = configFile.readStringUntil('\n').toInt();
	ringL.ringR = configFile.readStringUntil('\n').toInt();
	ringL.ringG = configFile.readStringUntil('\n').toInt();
	ringL.ringB = configFile.readStringUntil('\n').toInt();
	ringR.ringR = ringL.ringR;
	ringR.ringG = ringL.ringG;
	ringR.ringB = ringL.ringB;
	audioVolume = configFile.readStringUntil('\n').toFloat();
	if (simonSaysActive = configFile.readStringUntil('\n') == "1")
	{
		simonSaysActive = true;
	}
	else
	{
		simonSaysActive = false;
	}

	display.setColor();
	ringL.setColor();
	ringR.setColor();
	display.writeTime(getMinutes(), getHours(), militaryTime);
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

	server.on(
			"/", HTTP_GET,
			[](AsyncWebServerRequest *request)
			{
				request->send(200, "text/html", index_html);
			});
	server.on(
			"/hotspot-detect.html", HTTP_GET,
			[](AsyncWebServerRequest *request)
			{
				request->send(200, "text/html", index_html);
			});
	server.on("/changeTime", HTTP_GET, [](AsyncWebServerRequest *request)
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
			request->send(200, "text/plain", "OK"); });
	server.on("/updateScreenBrightness", HTTP_GET, [](AsyncWebServerRequest *request)
						{
		String inputBrightness;
			if (request->hasParam("value"))
			{
				inputBrightness = request->getParam("value")->value();
				display.setBrightness(inputBrightness.toInt());
				display.refreshDisplay();
			}
			request->send(200, "text/plain", "OK"); });
	server.on("/updateRingBrightness", HTTP_GET, [](AsyncWebServerRequest *request)
						{
				String inputBrightness;
			if (request->hasParam("value"))
			{
				inputBrightness = request->getParam("value")->value();
				ringL.setBrightness(inputBrightness.toInt());
				ringR.setBrightness(inputBrightness.toInt());
			}
			request->send(200, "text/plain", "OK"); });
	server.on("/updateNumberColor", HTTP_GET, [](AsyncWebServerRequest *request)
						{
				String inputR;
				String inputG;
				String inputB;
				if (request->hasParam("r"), request->hasParam("g"), request->hasParam("b"))
				{
				inputR = request->getParam("r")->value();
				inputG = request->getParam("g")->value();
				inputB = request->getParam("b")->value();
				display.numberR = inputR.toInt();
				display.numberG = inputG.toInt();
				display.numberB = inputB.toInt();
				display.setColor();
				display.writeTime(getMinutes(), getHours(), militaryTime);
				display.refreshDisplay();
			}
			request->send(200, "text/plain", "OK"); });
	server.on("/updateDotColor", HTTP_GET, [](AsyncWebServerRequest *request)
						{
							String inputR;
			String inputG;
			String inputB;
			if (request->hasParam("r"), request->hasParam("g"), request->hasParam("b"))
			{
				inputR = request->getParam("r")->value();
				inputG = request->getParam("g")->value();
				inputB = request->getParam("b")->value();
				display.dotR = inputR.toInt();
				display.dotG = inputG.toInt();
				display.dotB = inputB.toInt(); 
				display.setDotColor();
				display.writeTime(getMinutes(), getHours(), militaryTime);
			}
			request->send(200, "text/plain", "OK"); });
	server.on("/updateRingColor", HTTP_GET, [](AsyncWebServerRequest *request)
						{
				String inputR;
			String inputG;
			String inputB;
			if (request->hasParam("r"), request->hasParam("g"), request->hasParam("b"))
			{
				inputR = request->getParam("r")->value();
				inputG = request->getParam("g")->value();
				inputB = request->getParam("b")->value();
				ringL.ringR = inputR.toInt();
				ringL.ringG = inputG.toInt();
				ringL.ringB = inputB.toInt(); 
				ringR.ringR = inputR.toInt();
				ringR.ringG = inputG.toInt();
				ringR.ringB = inputB.toInt();
				ringL.setColor();
				ringR.setColor();
				ringL.refresh();
				ringR.refresh();
			}
			request->send(200, "text/plain", "OK"); });
	server.on("/updateVolume", HTTP_GET, [](AsyncWebServerRequest *request)
						{
				String inputVolume;
			if (request->hasParam("volume"))
			{
				inputVolume = request->getParam("volume")->value();
				i2s.setVolume((inputVolume.toInt()/100));
				Serial.print("set volume to");
				Serial.println((inputVolume.toInt()/100));
			}
			request->send(200, "text/plain", "OK"); });
	server.on("/alarms", HTTP_GET, [](AsyncWebServerRequest *request)
						{ request->send(200, "text/html", alarms_html, alarmListProcessor); });
	server.on("/addAlarm", HTTP_GET, [](AsyncWebServerRequest *request)
						{
							String inputHours;
							String inputMinutes;
							if (request->hasParam("hours"), request->hasParam("minutes"))
							{
								inputHours = request->getParam("hours")->value();
								inputMinutes = request->getParam("minutes")->value();
								insertAlarm(inputHours.toInt(), inputMinutes.toInt());
							}
							request->send(200, "text/plain", "OK"); });
	server.on("/deleteAlarm", HTTP_GET, [](AsyncWebServerRequest *request)
						{
								String index;
			if (request->hasParam("index"))
			{
				index = request->getParam("index")->value();
				alarms.erase(alarms.begin() + index.toInt());
			}
			getCurrentAlarmIndex();
			request->send(200, "text/plain", "OK"); });
	server.on(
			"/", HTTP_POST,
			[](AsyncWebServerRequest *request)
			{
				if (request->getResponse())
				{
					// 400 File not available for writing
					Serial.println("error");
					return;
				}

				if (!SD.exists("/sounds/alarm.mp3"))
				{
					Serial.println("nothing uploaded");
					return request->send(400, "text/plain", "Nothing uploaded");
				}

				// sends back the uploaded file
				request->send(SD, "/sounds/alarm.mp3", "text/plain");
			},
			[](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
			{
				Serial.printf("Upload[%s]: start=%u, len=%u, final=%d\n", filename.c_str(), index, len, final);

				if (!index)
				{
					request->_tempFile = SD.open("/sounds/alarm.mp3", "w");

					if (!request->_tempFile)
					{
						request->send(400, "text/plain", "File not available for writing");
						return;
					}
				}
				if (len)
				{
					request->_tempFile.write(data, len);
				}
				if (final)
				{
					request->_tempFile.close();
				}
			});
	server.on("/saveSettings", HTTP_GET, [](AsyncWebServerRequest *request)
						{
							writeConfig();
							request->send(200, "text/plain", "OK"); });
	server.on("/resetSettings", HTTP_GET, [](AsyncWebServerRequest *request)
						{
							initConfig();
							request->send(200, "text/plain", "OK"); });
	server.on("/simonOn", HTTP_GET, [](AsyncWebServerRequest *request)
						{
							simonSaysActive = true;
							writeConfig();
							request->send(200, "text/plain", "OK"); });
	server.on("/simonOff", HTTP_GET, [](AsyncWebServerRequest *request)
						{
							simonSaysActive = false;
							writeConfig();
							request->send(200, "text/plain", "OK"); });
	dnsServer.start(53, "*", WiFi.softAPIP());
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
			buttonLFunc();
			buttons.buttonEvents.erase(buttons.buttonEvents.begin());
			break;
		case Buttons::RIGHT_PRESS:
			Serial.println("right press");
			buttonRFunc();
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

	readConfig();
	readAlarms();

	loopFunctions.push_back(processButtons);
	loopFunctions.push_back(updateTimeDisplay);
	loopFunctions.push_back(alarmLoop);

	simon.ringControlFunc = setRingState;
	simon.finishFunction = stopAlarm;
}

void loop()
{
	for (int i = 0; i < loopFunctions.size(); i++)
	{
		loopFunctions[i]();
	}
	dnsServer.processNextRequest();
}