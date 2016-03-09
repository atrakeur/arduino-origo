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
	pinMode(13, OUTPUT);
	setStatus(&status[STATUS_BOOTING]);
}

void StatusLed::setStatus(const short * cycle)
{
	statusCycle = cycle;
	cycleState = 0;
}

void StatusLed::checkState()
{
	if (Origo::instance().hasWifi) {
		setStatus(&status[STATUS_GOT_WIFI]);
	}
	if (Origo::instance().isOnline) {
		setStatus(&status[STATUS_CONNECTED]);
	}
}

int StatusLed::tick()
{
	//Toggle the led
	isOn = !isOn;
	if (isOn) {
		digitalWrite(13, HIGH);
	}
	else {
		digitalWrite(13, LOW);
	}

	//Loop through states, go back to zero when encountering a delay of zero
	cycleState++;
	if (statusCycle[cycleState] == 0) {
		checkState();
		cycleState = 0;
	}

	//Wait before toggling again
	return statusCycle[cycleState];
}
