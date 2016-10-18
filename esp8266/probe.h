#ifndef __PROBE_H__
#define __PROBE_H__

#include <ArduinoJson.h>

struct Probe
{
	Probe()
	{
		pin = 0;
		nominalResistance = 0;
		nominalTemperatureInC = 0;
		b = 0;
		resistor = 0;
	}

	Probe(String name, int pin, long nominalResistance, long nominalTemperatureInC, long b, long resistor) :
		name(name), pin(pin), nominalResistance(nominalResistance), nominalTemperatureInC(nominalTemperatureInC), b(b), resistor(resistor)
	{
		adc = 0;
		ohm = 0.0;
		temperatureInC = 0;
	}
	String name;
	int pin;
	long nominalResistance;
	long nominalTemperatureInC;
	long b;
	long resistor;

	float adc;
	float ohm;
	float temperatureInC;

	void toJson(char *buf, int len)
	{
		StaticJsonBuffer<200> jsonBuffer;
		JsonObject &root = jsonBuffer.createObject();
		root["name"] = name;
		root["adc"] = adc;
		root["ohm"] = ohm;
		root["temperatureInC"] = temperatureInC;
		root.printTo(buf, len);
	}

	void toJson(Print &s)
	{
		StaticJsonBuffer<200> jsonBuffer;
		JsonObject &root = jsonBuffer.createObject();
		root["name"] = name;
		root["adc"] = adc;
		root["ohm"] = ohm;
		root["temperatureInC"] = temperatureInC;
		root.printTo(s);
		s.println();
	}

	static Probe fromJson(char *json)
	{
		StaticJsonBuffer<200> jsonBuffer;
		JsonObject &root = jsonBuffer.parseObject(json);
		Probe p;
		if (!root.success()) {
			return p;
		}
		const char *name = root["name"];
		float adc = root["adc"];
		float ohm = root["ohm"];
		float temperatureInC = root["temperatureInC"];
		p.name = String(name);
		p.adc = adc;
		p.ohm = ohm;
		p.temperatureInC = temperatureInC;
		return p;
	}
};

#endif
