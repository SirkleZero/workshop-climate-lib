#pragma once

#include <Arduino.h>

#include "InitializationResult.h"
#include "..\Sensors\BME280Data.h"
#include "..\Configuration\ControllerConfiguration.h"
#include "..\Display\RGB565.h"

using namespace Configuration;
using namespace Display;
using namespace Sensors;

namespace Relay {
	/// <summary>Describes the state that the humidification system is currently in.</summary>
	enum HumidificationState {
		/// <summary>Represents that we are in neither humidification or dehumidification state.</summary>
		None = 1u << 0,
		/// <summary>Represents that we are in a dehumidification state and that the dehumidifier is running.</summary>
		Dehumidifying = 1u << 1,
		/// <summary>Represents that we are in a humidification state and that the humidifier is running.</summary>
		Humidifying = 1u << 2
	};

	/// <summary>Class that represents the functionality of the relay attached to the climate controller.</summary>
	class HumidityRelayManager {
	public:
		HumidityRelayManager();

		InitializationResult Initialize(ControllerConfiguration *configuration);
		void AdjustClimate(BME280Data data);
		void KeepAlive();
		void EmergencyShutoff();
		void ShutDownError();
	private:
		// pins used by the relay system
		static const int RedPin = 15;
		static const int GreenPin = 16;
		static const int BluePin = 17;
		static const int HumidifierControlPin = 18;
		static const int DehumidifierControlPin = 19;

		// if using a common anode LED, set this to true
		static const bool CommonAnode = true;

		// colors
		const RGB565 Orange;
		const RGB565 Green;
		const RGB565 Purple;
		const RGB565 Aqua;
		const RGB565 Red;
		const RGB565 ErrorRed;

		// configuration
		ControllerConfiguration *configuration;
		bool indicatorEnabled = false;
		HumidificationState humidificationState = HumidificationState::None;

		// timer - used for the keep alive and the emergency shutdown functions
		unsigned long currentMillis = 0; // stores the current value of millis()
		unsigned long previousKeepAliveCall = 0; // stores the last time KeepAlive() was called

		void EnableHumidifier();
		void DisableHumidifier();

		void EnableDehumidifier();
		void DisableDehumidifier();

		void ShutDown();
		void ShutDownTargetReached();
		void ShutDownGoldilocks();
		void ShutDownEmergency();
		void SetIndicatorColor(RGB565 color);
		void DisableIndicator();
		void EnableIndicator();

		float CalculateCompensatedTargetHumidity(HumidificationState state);
	};
}
