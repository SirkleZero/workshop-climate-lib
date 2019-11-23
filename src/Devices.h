#pragma once

///<summary>Represents the various devices that can exist as part of the system.</summary>
enum Devices : uint8_t
{
	ClimateController = 1u << 0,
	ClimateSensorArray = 1u << 1,
	ParticulateSensorArray = 1u << 2,
	SystemMonitor = 1u << 3,
	AllListeners = ClimateController | SystemMonitor,
	AllTransmitters = ClimateSensorArray | ParticulateSensorArray
};
