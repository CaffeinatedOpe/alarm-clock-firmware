#include <WiFi.h>
#include <AsyncTCP.h>					 //https://github.com/me-no-dev/AsyncTCP using the latest dev version from @me-no-dev
#include <ESPAsyncWebServer.h> //https://github.com/me-no-dev/ESPAsyncWebServer using the latest dev version from @me-no-dev
#include <DNSServer.h>

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
			request->send(200, "text/html", "<h1>there should be a website here, but that's your problem</h1>");
		} else if (request->method() == HTTP_GET && request->url() == "/hotspot-detect.html") {
			request->send(200, "text/html", "<h1>there should be a website here, but that's your problem</h1>");
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
	Serial.begin(115200);
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