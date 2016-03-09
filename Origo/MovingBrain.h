// MovingBrain.h

#ifndef _MOVINGBRAIN_h
#define _MOVINGBRAIN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Brain.h"
#include <Servo.h>

#define MOVING_FORWARD_SERVO_PIN 39
#define MOVING_FORWARD_SERVO_CENTER 90
#define MOVING_FORWARD_SERVO_SWIPE 60

class MovingBrain : public AbstractBrain {
private:
	Servo forwardServo;
public:
	char* getName();
	void initialize();
	int tick();
};

#endif

