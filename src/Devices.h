#pragma once

///<summary>Represents the devices that are part of the system.</summary>
enum Devices : uint8_t
{
	None = 1u << 0,
	HumidificationController = 1u << 1,
	RemoteSensor1 = 1u << 2,
	SystemMonitor = 1u << 3
};
