#ifndef _ORIGO_h
#define _ORIGO_h

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#else
#define DEBUG_PRINT(x) do{} while(0)
#endif

class Origo
{
public:
	static Origo& instance()
	{
		static Origo instance;
		return instance;
	}

	/* Wifi Status */
	bool hasWifi;
	bool isOnline;

	/* Sensor status */
	int forward;
	int backward;
private:
	Origo() {};
	Origo(Origo const&);
	void operator=(Origo const&) {};
};

#endif
