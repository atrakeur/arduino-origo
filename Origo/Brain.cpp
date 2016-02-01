// 
// 
// 

#include "Origo.h"
#include "Brain.h"

int Brain::addModule(AbstractBrain * module)
{
	DEBUG_PRINT("Brain: Adding module ");
	DEBUG_PRINT(module->getName());
	DEBUG_PRINT("\n");

	BrainConfig* freeConfig = getFreeModuleConfig();
	if (freeConfig == nullptr) {
		DEBUG_PRINT("Brain: No more modules slots\n");
		return BRAIN_ERROR_NO_MODULES;
	}

	freeConfig->enabled = true;
	freeConfig->module = module;

	return BRAIN_OK;
}

int Brain::removeModule(AbstractBrain * module)
{
	DEBUG_PRINT("Brain: Removing module ");
	DEBUG_PRINT(module->getName());
	DEBUG_PRINT("");

	BrainConfig* config = getModuleConfig(module);
	config->enabled = false;
	config->module = nullptr;

	return BRAIN_OK;
}

void Brain::initialize()
{
	DEBUG_PRINT("Brain: Initializing modules \n");
	for (int i = 0; i < BRAIN_MAX_MODULES; i++) {
		if (configs[i].enabled) {
			DEBUG_PRINT("\tBrain: Initializing module ");
			DEBUG_PRINT(configs[i].module->getName());
			DEBUG_PRINT("\n");

			configs[i].module->initialize();
		}
	}
}

void Brain::tick()
{
	//Run module code
	if (configs[nextModule].enabled) {
		executeModule(&configs[nextModule]);
	}

	//Jump to the next module available
	for (int i = 1; i < BRAIN_MAX_MODULES; i++) {
		int index = (nextModule + i) % BRAIN_MAX_MODULES;
		if (configs[index].enabled) {
			nextModule = index;
			break;
		}
	}
	
}

void Brain::printModule(AbstractBrain * module, Print * output)
{
	BrainConfig* moduleConfig = getModuleConfig(module);

	if (moduleConfig == nullptr) {
		return;
	}

	output->print("Brain: ");
	output->print("\t\t");
	output->print(moduleConfig->module->getName());
	output->print("\t\t");
}

void Brain::printModules(Print * output)
{
	output->print("Brain: ");
	output->print("\t\t");
	output->print("Name");
	output->print("\t\t\t");
	output->print("Run time");
	output->print("\t");
	output->print("Late Time");
	output->print("\n");

	for (int i = 0; i < BRAIN_MAX_MODULES; i++) {
		if (configs[i].enabled) {
			printModule(configs[i].module, output);
		}
	}

	output->print("\n");
}

BrainConfig * Brain::getModuleConfig(AbstractBrain * module)
{
	for (int i = 0; i < BRAIN_MAX_MODULES; i++) {
		if (configs[i].enabled && configs[i].module == module) {
			return &configs[i];
		}
	}

	return nullptr;
}

BrainConfig * Brain::getFreeModuleConfig()
{
	for (int i = 0; i < BRAIN_MAX_MODULES; i++) {
		if (!configs[i].enabled) {
			return &configs[i];
		}
	}

	return nullptr;
}

void Brain::executeModule(BrainConfig * config)
{
	actualModule = config;

	config->module->tick();

	actualModule = nullptr;
}
