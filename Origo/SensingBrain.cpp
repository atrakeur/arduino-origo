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
}

char * SensingBrain::getName()
{
	return "Sensing brain";
}

void SensingBrain::initialize()
{
	this->addSensor("forward", 9, 8, &(Origo::instance().forward));
}

void SensingBrain::tick()
{
	digitalWrite(13, HIGH);
	if (sensorCount == 0) {
		return;
	}

	//Select the next sensor
	for (int i = 1; i < SENSING_MAX_SENSORS + 1; i++) {
		if (sensors[(previousSensor + i) % SENSING_MAX_SENSORS].enabled) {
			previousSensor = (previousSensor + i) % SENSING_MAX_SENSORS;
			break;
		}
	}

	//Run measure if selected sensor is enabled
	if (sensors[previousSensor].enabled) {
		//Send trigger
		digitalWrite(sensors[previousSensor].trigPin, HIGH);
		delayMicroseconds(10); // I think I can cope with blocking for a whole 10us here...
		digitalWrite(sensors[previousSensor].trigPin, LOW);

		//Place into the nextValue of samples
		long newValue = MicrosecondsToMillimetresDistance * pulseIn(sensors[previousSensor].echoPin, HIGH);
		sensors[previousSensor].nextValue = (sensors[previousSensor].nextValue + 1) % SENSING_SAMPLE_COUNTS;
		sensors[previousSensor].values[sensors[previousSensor].nextValue] = newValue;

		//Populate with values
		Origo::instance().forward = getSensorValue(sensors[previousSensor].name);
	}
	digitalWrite(13, LOW);
}

int SensingBrain::addSensor(const char * name, int trigPin, int echoPin, int* value)
{
	DEBUG_PRINT("Sensing Brain: Adding sensor ");
	DEBUG_PRINT(name);
	DEBUG_PRINT("\n");

	Sensor* sensor = getFreeSensorSlot();
	if (sensor == nullptr) {
		DEBUG_PRINT("SensingBrain: No more sensors slots\n");
		return SENSING_ERROR_NOSLOT;
	}

	sensor->enabled = true;
	sensor->name = name;
	sensor->trigPin = trigPin;
	sensor->echoPin = echoPin;
	sensor->nextValue = 0;

	sensorCount++;

	pinMode(trigPin, OUTPUT);
	digitalWrite(trigPin, LOW);
	pinMode(echoPin, INPUT);

	return SENSING_OK;
}

int SensingBrain::removeSensor(const char * name)
{
	Sensor* sensor = getSensorConfig(name);
	if (sensor == nullptr) {
		return SENSING_ERROR_NOTFOUND;
	}

	sensor->enabled = false;
	sensorCount--;

	return SENSING_OK;
}

int SensingBrain::getSensorValue(const char * name)
{
	Sensor* sensor = getSensorConfig(name);
	if (sensor == nullptr) {
		return SENSING_ERROR_NOTFOUND;
	}

	int average = 0;
	for (int i = 0; i < SENSING_SAMPLE_COUNTS; i++) {
		average += sensor->values[i];

		/* Uncomment this if having a very low false value at startup is fine or not
		if (sensor->values[i] == 0) {
			return average / i;
		}*/
	}

	return average / SENSING_SAMPLE_COUNTS;
}

Sensor * SensingBrain::getSensorConfig(const char * name)
{
	for (int i = 0; i < SENSING_MAX_SENSORS; i++) {
		if (sensors[i].enabled && strcmp(sensors[i].name, name) == 0) {
			return &sensors[i];
		}
	}

	return nullptr;
}

Sensor * SensingBrain::getFreeSensorSlot()
{
	for (int i = 0; i < SENSING_MAX_SENSORS; i++) {
		if (!sensors[i].enabled) {
			return &sensors[i];
		}
	}
	return nullptr;
}
