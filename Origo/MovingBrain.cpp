// 
// 
// 

#include "MovingBrain.h"
#include <Servo.h>
#include "Config.h"
#include "Origo.h"

char * MovingBrain::getName()
{
	return "Moving Brain";
}

void MovingBrain::initialize()
{
	//init servo
	forwardServo.attach(MOVING_FORWARD_SERVO_PIN);
	forwardServo.write(MOVING_FORWARD_SERVO_CENTER);

	//TODO init motor
}

int MovingBrain::tick()
{
	//Assign servos values
	int command = Origo::instance().steer;	//Steer command is between -100 and 100
	int commandClamped = command * MOVING_FORWARD_SERVO_SWIPE / 100;//Clamped is between -60 and +60
	int servoCommand = commandClamped + MOVING_FORWARD_SERVO_CENTER;//Command is now in the correct range
	forwardServo.write(servoCommand);

	/*DEBUG_PRINT("SERVO IS AT ");
	DEBUG_PRINT(servoCommand);
	DEBUG_PRINT("\n");*/

	//TODO assign motor values
}
