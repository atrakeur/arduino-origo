// SensingBrain.h

#include "Brain.h"
#include <NewPing.h>

#ifndef _SENSINGBRAIN_h
#define _SENSINGBRAIN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define SENSING_MAX_SENSORS 4
#define SENSING_SAMPLE_COUNTS 4
#define SENSING_SAMPLE_INTERVAL 50

#define SENSING_OK 0
#define SENSING_ERROR_NOSLOT -1

typedef struct Sensor {
	bool enabled;
	
	int pinTrig;
	int pinEcho;

	long* result;
	long results[SENSING_SAMPLE_COUNTS];
	int nextSampleAt;
} Sensor;

class SensingBrain : public AbstractBrain {
private:
	int sensorCount;
	int nextSensor;
	Sensor sensors[SENSING_MAX_SENSORS];
public:
	SensingBrain();
	char* getName();
	void initialize();
	int tick();

	int addSensor(int pinTrig, int pinEcho, long* result);
	int removeSensor(int pinTrig, int pinEcho);
protected:
	Sensor* getFreeSensor();
	int getSensorResult(Sensor* sensor);
	void setSensorResult(Sensor* sensor, long result);
	void measure(Sensor* sensor);
};

#endif

