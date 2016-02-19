// 
// 
// 

#include "SensingBrain.h"
#include "Origo.h"

const float SpeedOfSound = 343.2; // ~speed of sound (m/s) in air, at 20°C         
const float MicrosecondsPerMillimetre = 1000.0 / SpeedOfSound; // microseconds per millimetre
const float MicrosecondsToMillimetres = (1.0 / MicrosecondsPerMillimetre); //distance by time elapsed
const float MicrosecondsToMillimetresDistance = MicrosecondsToMillimetres / 2.0; //because the ping make twice distance to object

SensingBrain::SensingBrain()
{
	sensorCount = 0;
	nextSensor = 0;
}

char * SensingBrain::getName()
{
	return "Sensing Brain";
}

void SensingBrain::initialize()
{
	long data;
	addSensor(A8, A9, &data);
}

int SensingBrain::tick()
{
	if (sensorCount == 0) {
		return 1000;
	}

	//Do the measure
	if (sensors[nextSensor].enabled) {
		measure(&sensors[nextSensor]);
	}

	//Select the next sensor
	for (int i = 1; i < SENSING_MAX_SENSORS; i++) {
		int index = (nextSensor + i) % SENSING_MAX_SENSORS;
		if (sensors[index].enabled) {
			nextSensor = index;
		}
	}

	return SENSING_SAMPLE_INTERVAL;
}

int SensingBrain::addSensor(int pinTrig, int pinEcho, long* result)
{
	Sensor* sensorSlot = getFreeSensor();
	if (sensorSlot == nullptr) {
		return SENSING_ERROR_NOSLOT;
	}

	sensorCount++;
	sensorSlot->enabled = true;
	sensorSlot->pinTrig = pinTrig;
	sensorSlot->pinEcho = pinEcho;
	sensorSlot->result  = result;

	return SENSING_OK;
}

int SensingBrain::removeSensor(int pinTrig, int pinEcho)
{
	sensorCount--;
	for (int i = 0; i < SENSING_MAX_SENSORS; i++) {
		if (sensors[i].pinTrig == pinTrig && sensors[i].pinEcho == pinEcho) {
			sensors[i].enabled = false;
		}
	}
}

int SensingBrain::getSensorResult(Sensor * sensor)
{
	int sum = 0;

	for (int i = 0; i < SENSING_SAMPLE_COUNTS; i++) {
		if (sensor->results[i]) {
			sum += sensor->results[i];
		}
	}

	return sum / SENSING_SAMPLE_COUNTS;
}

void SensingBrain::setSensorResult(Sensor * sensor, long result)
{
	sensor->nextSampleAt = sensor->nextSampleAt % SENSING_SAMPLE_COUNTS;
	sensor->results[sensor->nextSampleAt] = result;
}

void SensingBrain::measure(Sensor * sensor)
{
	//Send a high pulse on the trigger for a given amount of time
	digitalWrite(sensor->pinTrig, HIGH);
	delayMicroseconds(10); // Some sensor need more than 10 us
	digitalWrite(sensor->pinTrig, LOW);

	//Sound wave is in transit, wait for it to come back (ECHO pin goes to LOW)
	long microseconds = pulseIn(sensor->pinEcho, HIGH, 50000UL);

	long millimetres = MicrosecondsToMillimetresDistance * microseconds;
	
	setSensorResult(sensor, millimetres);
	DEBUG_PRINT(getSensorResult(sensor));
	DEBUG_PRINT("\n");
}

Sensor* SensingBrain::getFreeSensor()
{
	for (int i = 0; i < SENSING_MAX_SENSORS; i++) {
		if (!sensors[i].enabled) {
			return &sensors[i];
		}
	}

	return nullptr;
}
