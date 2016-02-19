// SensingBrain.h

#include "Brain.h"

#ifndef _SENSINGBRAIN_h
#define _SENSINGBRAIN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define SENSING_MAX_SENSORS 4
#define SENSING_SAMPLE_COUNTS 4

#define SENSING_OK 0
#define SENSING_ERROR_NOSLOT -1
#define SENSING_ERROR_NOTFOUND -2

typedef struct Sensor {
	boolean enabled;
	const char* name;
	int trigPin;
	int echoPin;

	/* Rolling average of samples */
	int nextValue;						//The next value to write
	int values[SENSING_SAMPLE_COUNTS];	//All the sample values

	/* Pointer to where to store the actual value */
	int* value;
} Sensor;

class SensingBrain : public AbstractBrain {
private:
	int sensorCount = 0;
	int previousSensor = 0;
	Sensor sensors[SENSING_MAX_SENSORS];
public:
	SensingBrain();
	char* getName();
	void initialize();
	int tick();

	int addSensor(const char* name, int trigPin, int echoPin, int* value);
	int removeSensor(const char* name);
	int getSensorValue(const char* name);

protected:
	Sensor* getSensorConfig(const char* name);
	Sensor* getFreeSensorSlot();
};

#endif

