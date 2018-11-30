#include "SensorManager.h"

namespace Sensors {
	SensorManager::SensorManager(AvailableSensors enabledSensors)
	{
		this->enabledSensors = enabledSensors;
	}

	InitializationResult SensorManager::Initialize()
	{
		InitializationResult result;



		return result;
	}

	bool SensorManager::ReadSensors(SensorData *data)
	{
		return true;
	}

	void SensorManager::PrintDebug()
	{

	}

	SensorManager::~SensorManager() {}
}
