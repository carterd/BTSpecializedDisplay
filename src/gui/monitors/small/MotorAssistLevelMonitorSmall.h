#ifndef _MOTOR_ASSIST_LEVEL_MONITOR_SMALL_H
#define _MOTOR_ASSIST_LEVEL_MONITOR_SMALL_H

#include "BaseNumericMonitorSmall.h"

#define ASSIST_LEVEL_TEXT_0 "Off"
#define ASSIST_LEVEL_TEXT_1 "Eco"
#define ASSIST_LEVEL_TEXT_2 "Trail"
#define ASSIST_LEVEL_TEXT_3 "Turbo"

class MotorAssistLevelMonitorSmall : public BaseNumericMonitorSmall
{
public:
	MotorAssistLevelMonitorSmall(const char* title = " " LV_SYMBOL_POWER) : BaseNumericMonitorSmall(BikeStateAttributeIndex::MOTOR_ASSIST_LEVEL, MonitorAttributeType::EVERY_MINUTE, title) {}

	virtual void initBluetoothStats()
	{
		BaseNumericMonitorSmall::initBluetoothStats();
		this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::BIKE_ON_OFF_STATE, MonitorAttributeType::EVERY_TEN_SECONDS);
	}

	virtual void updateLvObj() {		
		const char* valueString;
		BikeStateAttribute& bikeStateAttribute = this->bluetoothBikeController->getBikeState().getStateAttribute(this->bikeStateAttributeIndex);

		uint16_t assistLevel = bikeStateAttribute.bikeStateAttributeValue.valueUint16;

		// Convert the value into a string
		switch (assistLevel) {
		case 1:
			valueString = ASSIST_LEVEL_TEXT_1;
			break;
		case 2:
			valueString = ASSIST_LEVEL_TEXT_2;
			break;
		case 3:
			valueString = ASSIST_LEVEL_TEXT_3;
			break;
		default:
			valueString = ASSIST_LEVEL_TEXT_0;
		}
		this->previousBikeStateAttribute.valueUint16 = assistLevel;

		lv_label_set_text(this->value_obj, valueString);
	}
};

#endif