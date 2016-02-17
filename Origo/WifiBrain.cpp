// 
// 
// 

#include "Config.h"
#include "Origo.h"
#include "WifiBrain.h"

WifiBrain::WifiBrain(HardwareSerial* serial): esp(serial, ESP_CHPD_PIN), mqtt(&esp)
{
	this->serial = serial;
	Origo::instance().hasWifi  = false;
	Origo::instance().isOnline = false;
}

char * WifiBrain::getName()
{
	return "WifiBrain";
}

void WifiBrain::initialize()
{
	DEBUG_PRINT("\tWifi Brain: Initializing ESP\n");

	pinMode(ESP_RST_PIN, OUTPUT);
	pinMode(ESP_CHPD_PIN, OUTPUT);
	digitalWrite(ESP_CHPD_PIN, HIGH);
	digitalWrite(ESP_RST_PIN, HIGH);

	this->serial->begin(19200);

	esp.enable();
	delay(500);
	esp.reset();
	delay(500);
	while (!esp.ready());

	setupHandlers();
}

void WifiBrain::tick()
{
	esp.process();
}

void WifiBrain::setupHandlers()
{
	DEBUG_PRINT("\tWifi Brain: Setup MQTT\n");
	mqtt.connectedCb.attach(this, &WifiBrain::onMqttConnect);
	mqtt.disconnectedCb.attach(this, &WifiBrain::onMqttDisconnect);
	mqtt.publishedCb.attach(this, &WifiBrain::onMqttPublished);
	mqtt.dataCb.attach(this, &WifiBrain::onMqttData);
	if (!mqtt.begin(MQTT_BROKER_CHAN, MQTT_BROKER_USER, MQTT_BROKER_PASS, 10, 1)) {
		DEBUG_PRINT("\tWifi Brain: Setup mqtt FAILED\n");
	}

	DEBUG_PRINT("\tWifi Brain: Setup Wifi\n");
	esp.wifiCb.attach(this, &WifiBrain::onWifiEvent);
	esp.wifiConnect(WIFI_SSID, WIFI_PASS);
}

void WifiBrain::onWifiEvent(void* response)
{
	uint32_t status;
	RESPONSE res(response);

	if (res.getArgc() == 1) {
		res.popArgs((uint8_t*)&status, 4);
		if (status == STATION_GOT_IP) {
			DEBUG_PRINT("\tWifi Brain: Wifi Connected to ");
			DEBUG_PRINT(WIFI_SSID);
			DEBUG_PRINT("\n");

			Origo::instance().hasWifi = true;
			mqtt.connect(MQTT_BROKER_ADDR, MQTT_BROKER_PORT, true);
		} else {
			DEBUG_PRINT("\tWifi Brain: Wifi Connection lost\n");
			Origo::instance().hasWifi = false;
			mqtt.disconnect();
		}
	}
}

void WifiBrain::onMqttConnect(void * response)
{
	Origo::instance().isOnline = true;

	//Setup subscribes
	mqtt.subscribe("/topic/0");
	mqtt.publish("/esp", "online");
	//Publish online status
	DEBUG_PRINT("\tWifi Brain: MQTT Connected to ");
	DEBUG_PRINT(MQTT_BROKER_ADDR);
	DEBUG_PRINT("\n");
}

void WifiBrain::onMqttData(void * response)
{
	RESPONSE res(response);
	String topic = res.popString();
	String data = res.popString();

	//TODO do something with received data

	DEBUG_PRINT("\tWifi Brain: Received: ");
	DEBUG_PRINT(topic);
	DEBUG_PRINT(" = ");
	DEBUG_PRINT(data);
	DEBUG_PRINT("\n");
}

void WifiBrain::onMqttDisconnect(void * response)
{
	DEBUG_PRINT("\tWifi Brain: MQTT Connection Lost\n");
	Origo::instance().isOnline = false;
}

void WifiBrain::onMqttPublished(void * response)
{
}
