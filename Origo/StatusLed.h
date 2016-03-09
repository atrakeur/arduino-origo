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

#define STATUS_BOOTING   0
#define STATUS_GOT_WIFI  3
#define STATUS_CONNECTED 6

class StatusLed : public AbstractBrain {
private:
	const short status[9] = {
		250, 250, 0,		//Booting various sub modules
		500, 500, 0,		//Got wifi, but not online
		1000, 1000, 0		//Online (got wifi + MQTT)
	};

	bool isOn;
	
	const short* statusCycle;	//Which cycle to do
	int cycleState;		//Index in the cycle
public:
	char* getName();
	void initialize();
	void setStatus(const short* cycle);
	void checkState();
	int tick();
};

#endif

