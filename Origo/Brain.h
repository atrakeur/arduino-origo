// Brain.h

#ifndef _BRAIN_h
#define _BRAIN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define BRAIN_MAX_MODULES 4

#define BRAIN_OK 0
#define BRAIN_ERROR_NO_MODULES -1

/**
* Defines a Brain module
*/
class AbstractBrain {
public:
	/* The name of the module */
	virtual char* getName() = 0;
	/* Initialize the module */
	virtual void initialize() = 0;
	/* Run the module code */
	virtual void tick() = 0;
};

typedef struct BrainConfig {
	boolean enabled;
	AbstractBrain* module;

	unsigned long nextRunTime;
};

class Brain {
private:
	int nextModule;
	int moduleCount;
	BrainConfig* actualModule;
	BrainConfig configs[BRAIN_MAX_MODULES];
public:
	int addModule(AbstractBrain* module);
	int removeModule(AbstractBrain* module);
	void initialize();
	void tick();
	void printModule(AbstractBrain* module, Print* output);
	void printModules(Print* output);

protected:
	BrainConfig* getModuleConfig(AbstractBrain* module);
	BrainConfig* getFreeModuleConfig();
	void executeModule(BrainConfig * config);
};

#endif

