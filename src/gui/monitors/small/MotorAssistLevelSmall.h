#ifndef _MOTOR_ASSIST_LEVEL_SMALL_H
#define _MOTOR_ASSIST_LEVEL_SMALL_H

#include "BaseNumericSmall.h"

#define ASSIST_LEVEL_TEXT_0 "-"
#define ASSIST_LEVEL_TEXT_1 "Eco"
#define ASSIST_LEVEL_TEXT_2 "Trail"
#define ASSIST_LEVEL_TEXT_3 "Turbo"

class MotorAssistLevelSmall : public BaseNumericSmall
{
public:
	MotorAssistLevelSmall(const char* title = "Asst:") : BaseNumericSmall(BikeStateAttributeIndex::MOTOR_ASSIST_LEVEL, MonitorAttributeType::EVERY_MINUTE, title) {}

	virtual void initBluetoothStats()
	{
		BaseNumericSmall::initBluetoothStats();
		this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::BIKE_ON_OFF_STATE, MonitorAttributeType::EVERY_TEN_SECONDS);
	}

	virtual void statusUpdate() {
		const char* valueString;
		BikeStateAttribute& bikeStateAttribute = this->bluetoothBikeController->getBikeState().getStateAttribute(this->bikeStateAttributeIndex);

		// Convert the value into a string
		switch (bikeStateAttribute.bikeStateAttributeValue.valueUint8) {
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
		lv_label_set_text(this->value_obj, valueString);
	}
};

#endif