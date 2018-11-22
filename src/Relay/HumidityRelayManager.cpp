#include "HumidityRelayManager.h"

using namespace Sensors;

namespace Relay {
	/// <summary>Initializes a new instance of the <see cref="HumidityRelayManager"/> class.</summary>
	HumidityRelayManager::HumidityRelayManager() :
		Orange(0xe6, 0x5c, 0x00),
		Green(0x44, 0x66, 0x00),
		Purple(0x66, 0x00, 0xcc),
		Aqua(0x00, 0x99, 0xff),
		Red(0x99, 0x00, 0x00),
		ErrorRed(0xff, 0x00, 0x00)
	{}

	/// <summary>Executes initialization logic for the object.</summary>
	/// <param name="configuration">The <see cref="ControllerConfiguration"/> object that contains configuration information for the system.</param>
	/// <returns>An <see cref="InitializationResult"/> that describes the result of initialization.</returns>
	InitializationResult HumidityRelayManager::Initialize(ControllerConfiguration *configuration)
	{
		InitializationResult result;

		this->configuration = configuration;

		// configure relay control pins
		pinMode(HumidityRelayManager::HumidifierControlPin, OUTPUT);
		pinMode(HumidityRelayManager::DehumidifierControlPin, OUTPUT);

		// enable the LED indicator and set it's initial state color
		this->EnableIndicator();
		this->SetIndicatorColor(HumidityRelayManager::Orange);

		result.IsSuccessful = true;
		return result;
	}

	/// <summary>Resets the internal counter that is used to track the last time the system responded to sensor information.</summary>
	void HumidityRelayManager::KeepAlive()
	{
		this->previousKeepAliveCall = millis();
	}

	/// <summary>Adjusts the climate by enabling relays that are attached to climate management devices.</summary>
	/// <param name="data">The <see cref="SensorData"> containing readings from the sensors.</param>
	void HumidityRelayManager::AdjustClimate(SensorData data)
	{
		// touch the system to keep it alive. If we don't do this, our emergency shutoff will eventually kick in.
		this->KeepAlive();

		// Check to see what kind of humidification state we are in.
		switch (this->humidificationState)
		{
			case HumidificationState::Dehumidifying:
				// we are dehumidifying, check against the target level and run or shut down appropriately
				if (data.Climate.Humidity <= this->configuration->TargetHumidity)
				{
					// we hit our target. shut down and re-run this method to determine the next action to take.
					this->ShutDownTargetReached();
					this->AdjustClimate(data);
				}
				break;
			case HumidificationState::Humidifying:
				// we are humidifying, check against the target level and run or shut down appropriately
				if (data.Climate.Humidity >= this->configuration->TargetHumidity)
				{
					// we hit our target. shut down and re-run this method to determine the next action to take.
					this->ShutDownTargetReached();
					this->AdjustClimate(data);
				}
				break;
			case HumidificationState::None:
				// We are neither humidifying or dehumidifying, so let's check to see if we are outside of our min and max operating parameters. If we are in the range, then we're in the goldilocks zone and don't need to do anything; otherwise, we need to turn on either the humdififier or the dehumidifier.
				if (data.Climate.Humidity > this->configuration->MaximumHumidity)
				{
					// current humidity from the sensor exceeds the maximum threshold, enable the dehumidifier.
					this->EnableDehumidifier();
				}
				else if (data.Climate.Humidity < this->configuration->MinimumHumidity)
				{
					// current humidity from the sensor exceeds the minimum threshold, enable the humidifier.
					this->EnableHumidifier();
				}
				else
				{
					// Goldilocks zone, shut the system down.
					this->ShutDownGoldilocks();
				}
				break;
			default:
				// System has no idea wtf is going on, shut down for safety sake.
				this->ShutDownError();
				break;
		}
	}

	/// <summary>Checks against the internal timeout clock and if it has lapsed, shuts the system down.</summary>
	/// <remarks>This method should be called as frequently as possible to ensure the system doesn't enter into a runaway state.</remarks>
	void HumidityRelayManager::EmergencyShutoff()
	{
		// this method uses the the current milliseconds that the system has been running to create, essentially, a timer interrupt. It uses the keep alive and runaway timeout value to determine if a certain amount of time has lapsed.
		currentMillis = millis();
		if (this->currentMillis - this->previousKeepAliveCall >= this->configuration->RunawayTimeLimit)
		{
			// we exceeded our time! Shut it down!
			this->ShutDownEmergency();
		}
	}

	void HumidityRelayManager::EnableHumidifier()
	{
		this->DisableDehumidifier();
		this->humidificationState = HumidificationState::Humidifying;
		this->SetIndicatorColor(HumidityRelayManager::Aqua);

		digitalWrite(HumidityRelayManager::HumidifierControlPin, HIGH);
	}

	void HumidityRelayManager::EnableDehumidifier()
	{
		this->DisableHumidifier();
		this->humidificationState = HumidificationState::Dehumidifying;
		this->SetIndicatorColor(HumidityRelayManager::Red);

		digitalWrite(HumidityRelayManager::DehumidifierControlPin, HIGH);
	}

	void HumidityRelayManager::DisableHumidifier()
	{
		digitalWrite(HumidityRelayManager::HumidifierControlPin, LOW);
	}

	void HumidityRelayManager::DisableDehumidifier()
	{
		digitalWrite(HumidityRelayManager::DehumidifierControlPin, LOW);
	}

	void HumidityRelayManager::ShutDown()
	{
		this->humidificationState = HumidificationState::None;

		this->DisableHumidifier();
		this->DisableDehumidifier();
	}

	void HumidityRelayManager::ShutDownTargetReached()
	{
		this->ShutDown();
	}

	void HumidityRelayManager::ShutDownGoldilocks()
	{
		this->SetIndicatorColor(HumidityRelayManager::Green);
		this->ShutDown();
	}

	/// <summary>Shuts down the relays. Use this to shut down and denote that an error occurred.</summary>
	void HumidityRelayManager::ShutDownError()
	{
		this->SetIndicatorColor(HumidityRelayManager::ErrorRed);
		this->ShutDown();
	}

	void HumidityRelayManager::ShutDownEmergency()
	{
		this->SetIndicatorColor(HumidityRelayManager::Purple);
		this->ShutDown();
	}

	void HumidityRelayManager::SetIndicatorColor(RGB color)
	{
		this->EnableIndicator();

		if (HumidityRelayManager::CommonAnode)
		{
			analogWrite(HumidityRelayManager::RedPin, 255 - color.Red);
			analogWrite(HumidityRelayManager::GreenPin, 255 - color.Green);
			analogWrite(HumidityRelayManager::BluePin, 255 - color.Blue);
		}
		else
		{
			analogWrite(HumidityRelayManager::RedPin, color.Red);
			analogWrite(HumidityRelayManager::GreenPin, color.Green);
			analogWrite(HumidityRelayManager::BluePin, color.Blue);
		}
	}

	void HumidityRelayManager::DisableIndicator()
	{
		if (this->indicatorEnabled)
		{
			pinMode(HumidityRelayManager::RedPin, INPUT);
			pinMode(HumidityRelayManager::GreenPin, INPUT);
			pinMode(HumidityRelayManager::BluePin, INPUT);

			this->indicatorEnabled = false;
		}
	}

	void HumidityRelayManager::EnableIndicator()
	{
		if (!this->indicatorEnabled)
		{
			pinMode(HumidityRelayManager::RedPin, OUTPUT);
			pinMode(HumidityRelayManager::GreenPin, OUTPUT);
			pinMode(HumidityRelayManager::BluePin, OUTPUT);

			this->indicatorEnabled = true;
		}
	}
}
