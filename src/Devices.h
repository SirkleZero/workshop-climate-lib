#pragma once

///<summary>Represents the various devices that can exist as part of the system.</summary>
enum Devices : uint8_t
{
	Controller = 1u << 0,
	ClimateSensor = 1u << 1,
	ParticulateSensor = 1u << 2,
	SystemMonitor = 1u << 3,
	AllListeners = Controller | SystemMonitor,
	AllTransmitters = ClimateSensor | ParticulateSensor
};
