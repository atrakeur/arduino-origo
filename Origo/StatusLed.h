// StatusLed.h

#ifndef _STATUSLED_h
#define _STATUSLED_h

#include "Origo.h"
#include "Brain.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class StatusLed : public AbstractBrain {
private:
	bool isOn;
	int blinkFreq;
public:
	char* getName();
	void initialize();
	int tick();
};

#endif

