#include "Probe.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <WiFiManager.h>

//#define DEBUG Serial
#define DEBUG Serial1 // for production

static MDNSResponder mdns;
static ESP8266WebServer www(80);

Probe pit;
Probe food;
Probe ambient;

void setup()
{
	SPIFFS.begin();

	Serial.begin(57600);
	Serial1.begin(115200);

	WiFiManager wifiManager;
	wifiManager.autoConnect("smoker");

	if (mdns.begin("smoker", WiFi.localIP())) {
		DEBUG.println("mDNS responder active");
	}

	www.on("/probe/pit", [] {
		char buf[200];
		pit.toJson(buf, sizeof(buf));
		www.send(200, "application/json", buf); 
	});
	www.on("/probe/food", [] {
		char buf[200];
		food.toJson(buf, sizeof(buf));
		www.send(200, "application/json", buf); 
	});

	www.serveStatic("", SPIFFS, "");

	www.begin();
	DEBUG.println("www active");

	pit.name = "Pit";
	food.name = "Food";
	ambient.name = "Ambient";

	pit.temperatureInC = food.temperatureInC = ambient.temperatureInC = -1;
}

void loop()
{
	if (Serial.available() > 0) {
		static char buffer[200];
		int read = Serial.readBytesUntil('\n', buffer, 200);
		if (read > 0) {
			Probe p = Probe::fromJson(buffer);
			if (p.name == "Pit") {
				pit = p;
			}
			else if (p.name == "Food") {
				food = p;
			}
			else if (p.name == "Ambient") {
				ambient = p;
			}
			p.toJson(DEBUG);
		}
	}

	www.handleClient();
}
