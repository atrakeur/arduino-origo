// CommunicationModule.h

#ifndef _COMMUNICATIONMODULE_h
#define _COMMUNICATIONMODULE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <ringbuf.h>
#include <rest.h>
#include <mqtt.h>
#include <FP.h>
#include <espduino.h>
#include <crc16.h>

#include "Brain.h"

#define ESP_RST_PIN  6
#define ESP_CHPD_PIN 7

/*
 * Brain module for wifi communication
 * 
 * This module require an ESP8266 flashed with the code here: https://github.com/tuanpmt/espduino
 * This module use MQTT protocol for data and commands transfert
 */
class WifiBrain : public AbstractBrain {
private:
	HardwareSerial* serial;
	ESP esp;
	MQTT mqtt;

	boolean wifiConnected;
public:
	WifiBrain(HardwareSerial* esp);

	char* getName();
	void initialize();
	int tick();
protected:
	void setupHandlers();
	void onWifiEvent(void* response);
	void onMqttConnect(void* response);
	void onMqttData(void* response);
	void onMqttDisconnect(void* response);
	void onMqttPublished(void* response);
};
#endif

