#include "MovingBrain.h"
#include "StatusLed.h"
#include <NewPing.h>
#include <ringbuf.h>
#include <rest.h>
#include <mqtt.h>
#include <FP.h>
#include <espduino.h>
#include <crc16.h>
#include <EEPROM.h>
#include <Servo.h>

#include "Config.h"
#include "Origo.h"
#include "Brain.h"
#include "WifiBrain.h"
#include "SensingBrain.h"
#include "MovingBrain.h"

Brain brain;
StatusLed status;
//SensingBrain sensors;
WifiBrain wifi(&Serial1);
MovingBrain move;

void setup() {
	Serial.begin(19200);
	while (!Serial);

	Serial.println("Setup: Configuring brain modules");
	brain.addModule(&status);
	brain.addModule(&wifi);
	brain.addModule(&move);
	//brain.addModule(&sensors);

	Serial.println("Setup: Initializing brain modules");
	brain.initialize();

	Serial.println("Setup: starting done");
}

void loop() {
	brain.tick();
}
