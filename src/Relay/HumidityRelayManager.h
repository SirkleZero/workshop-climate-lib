#ifndef HumidityRelayManager_h
#define HumidityRelayManager_h

#include <Arduino.h>

#include "..\Sensors\SensorData.h"
#include "..\Configuration\ControllerConfiguration.h"
#include "..\Display\RGB.h"

using namespace Configuration;
using namespace Display;
using namespace Sensors;

namespace Relay {
	enum HumidificationState {
		// Represents that we are in a dehumidification state and that the dehumidifier is running.
		Dehumidifying = 0x00,
		// Represents that we are in a humidification state and that the humidifier is running.
		Humidifying = 0x01,
		// Represents that we are in neither humidification or dehumidification state.
		None = 0x02
	};

	class HumidityRelayManager {
	public:
		HumidityRelayManager();

		void Initialize(ControllerConfiguration *configuration);
		void AdjustClimate(SensorData data);
		void KeepAlive();
		void EmergencyShutoff();

	private:
		// pins
		const int RedPin = 15;
		const int GreenPin = 16;
		const int BluePin = 17;
		const int HumidifierControlPin = 18;
		const int DehumidifierControlPin = 19;

		// if using a common anode LED, set this to true
		const bool CommonAnode = true;

		// colors
		const RGB Orange;
		const RGB Green;
		const RGB Purple;
		const RGB Aqua;
		const RGB Red;
		const RGB ErrorRed;

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
		void ShutDownGoldilocks();
		void ShutDownError();
		void ShutDownEmergency();
		void SetIndicatorColor(RGB color);
		void DisableIndicator();
		void EnableIndicator();
	};
}

#endif
