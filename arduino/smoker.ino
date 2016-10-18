#include <DS3232RTC.h>

#include "Probe.h"

void setup(void)
{
	Serial.begin(115200);
}

void temperature(Probe *probe)
{
	const int NUMSAMPLES = 5;

	long totalADC = 0;
	for (int i = 0; i < NUMSAMPLES; i++) {
		totalADC += analogRead(probe->pin);
		delay(10);
	}

	float average = (float) totalADC / NUMSAMPLES;
	probe->adc = average;

	// convert the value to resistance
	average = 1023 / average - 1;
	average = probe->resistor / average;
	probe->ohm = average;

	float steinhart = average / probe->nominalResistance;     // (R/Ro)
	steinhart = log(steinhart);                  // ln(R/Ro)
	steinhart /= probe->b;                   // 1/B * ln(R/Ro)
	steinhart += 1.0 / (probe->nominalTemperatureInC + 273.15); // + (1/To)
	steinhart = 1.0 / steinhart;                 // Invert
	steinhart -= 273.15;                         // convert to C

	probe->temperatureInC = steinhart;
}

Probe pit("Pit", A0, 1000000, 25, 4800, 1000000);
Probe food("Food", A1, 43000, 25, 4000, 51000);
Probe ambient("Ambient", 0, 0, 0, 0, 0);

void loop(void) {
	temperature(&pit);
	temperature(&food);
	ambient.temperatureInC = RTC.temperature() / 4.0;

	pit.toJson(Serial);
	food.toJson(Serial);
	ambient.toJson(Serial);

	delay(2000);
}
