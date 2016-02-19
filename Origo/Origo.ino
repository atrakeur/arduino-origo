#include <NewPing.h>
#include <ringbuf.h>
#include <rest.h>
#include <mqtt.h>
#include <FP.h>
#include <espduino.h>
#include <crc16.h>
#include <EEPROM.h>

#include "Config.h"
#include "Origo.h"
#include "Brain.h"
#include "WifiBrain.h"
#include "SensingBrain.h"

Brain brain;
SensingBrain sensors;
WifiBrain wifi(&Serial1);

void setup() {
	Serial.begin(19200);
	while (!Serial);

	Serial.println("Setup: Configuring brain modules");
	brain.addModule(&wifi);
	brain.addModule(&sensors);

	Serial.println("Setup: Initializing brain modules");
	brain.initialize();

	Serial.println("Setup: starting done");
}

void loop() {
	brain.tick();
}
