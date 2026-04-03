#include <WiFi.h>
#include <AsyncTCP.h>					 //https://github.com/me-no-dev/AsyncTCP using the latest dev version from @me-no-dev
#include <ESPAsyncWebServer.h> //https://github.com/me-no-dev/ESPAsyncWebServer using the latest dev version from @me-no-dev
#include <DNSServer.h>

int receivedSeconds;
int receivedMinutes;
int receivedHours;

typedef enum{
	READY,
	UPDATETIME,
	UPDATEVOLUME,
	UPDATECOLOR,
	UPDATEDOTCOLOR
} WIFISTATE;

int receivedScreenColor[3];
int receivedScreenDotColor[3];

WIFISTATE wifiState = READY;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
    input:checked+.slider {background-color: #b30000}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>ESP Web Server</h2>
  %BUTTONPLACEHOLDER%
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?output="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?output="+element.id+"&state=0", true); }
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

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
		} else if (request->method() == HTTP_GET && request->url() == "/hotspot-detect.html") {
			request->send(200, "text/html", index_html);
		} else if (request->method() == HTTP_GET && request->url() == "/changeTime") {
				String inputSeconds;
				String inputMinutes;
				String inputHours;
  		  if (request->hasParam("seconds") && request->hasParam("minutes") && request->hasParam("hours")) {
  		    inputSeconds = request->getParam("seconds")->value();
					inputMinutes = request->getParam("minutes")->value();
					inputHours = request->getParam("hours")->value();
					wifiState = UPDATETIME;
					receivedSeconds = inputSeconds.toInt();
					receivedMinutes = inputMinutes.toInt();
					receivedHours = inputHours.toInt();
  		  }
  		  else {
  		    inputSeconds = "";
					inputMinutes = "";
					inputHours = "";
  		  }
  		  Serial.print("Got time: ");
  		  Serial.print(inputSeconds);
				Serial.print(inputMinutes);
				Serial.print(inputHours);
  		  request->send(200, "text/plain", "OK");
		} else if (request->method() == HTTP_GET && request->url() == "/updateScreenBrightnes") {
				String inputBrightness;
  		  if (request->hasParam("value")) {
  		    inputBrightness = request->getParam("values")->value();
					//screenBrightness = inputBrightness.toInt();
  		  }
  		  request->send(200, "text/plain", "OK");
		} else if (request->method() == HTTP_GET && request->url() == "/updateRingBrightnes") {
				String inputBrightness;
  		  if (request->hasParam("value")) {
  		    inputBrightness = request->getParam("values")->value();
					//screenBrightness = inputBrightness.toInt();
  		  }
  		  request->send(200, "text/plain", "OK");
		} else if (request->method() == HTTP_GET && request->url() == "/updateNumberColor") {
				String inputR;
				String inputG;
				String inputB;
  		  if (request->hasParam("r"), request->hasParam("g"), request->hasParam("b")) {
  		    inputR = request->getParam("r")->value();
					inputG = request->getParam("g")->value();
					inputB = request->getParam("b")->value();
					receivedScreenColor[0] = inputR.toInt();
					receivedScreenColor[1] = inputG.toInt();
					receivedScreenColor[2] = inputB.toInt();
					wifiState = UPDATECOLOR;
  		  }
  		  request->send(200, "text/plain", "OK");
		} else if (request->method() == HTTP_GET && request->url() == "/updateDotColor") {
				String inputR;
				String inputG;
				String inputB;
  		  if (request->hasParam("r"), request->hasParam("g"), request->hasParam("b")) {
  		    inputR = request->getParam("r")->value();
					inputG = request->getParam("g")->value();
					inputB = request->getParam("b")->value();
					receivedScreenDotColor[0] = inputR.toInt();
					receivedScreenDotColor[1] = inputG.toInt();
					receivedScreenDotColor[2] = inputB.toInt();
					wifiState = UPDATEDOTCOLOR;
  		  }
  		  request->send(200, "text/plain", "OK");
		} else
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
  dnsServer.setTTL(300);
	dnsServer.start(53, "*", WiFi.softAPIP());
	server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);
	server.begin();
}

void wifiLoop()
{
	dnsServer.processNextRequest();
}