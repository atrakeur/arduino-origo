// 
// 
// 

#include "Origo.h"
#include "Brain.h"
#include "StatusLed.h"


char * StatusLed::getName()
{
	return "Status Led";
}

void StatusLed::initialize()
{
	blinkFreq = 1000;
	pinMode(13, OUTPUT);
}

int StatusLed::tick()
{
	isOn = !isOn;

	if (isOn) {
		digitalWrite(13, HIGH);
	}
	else {
		digitalWrite(13, LOW);
	}

	return blinkFreq;
}
