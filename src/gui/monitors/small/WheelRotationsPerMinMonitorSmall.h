#ifndef _WHEEL_ROTATIONS_MONITOR_SMALL_H
#define _WHEEL_ROTATIONS_MONITOR_SMALL_H

#include "BaseNumericSmall.h"


class WheelRotationsPerMinMonitorSmall : public BaseNumericSmall
{
private:
    uint16_t wheelCircumferenceMm;
public:
    WheelRotationsPerMinMonitorSmall(const char* title = "km/h:") : BaseNumericSmall(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN, MonitorAttributeType::EVERY_SECOND, title) {}

    /// <summary>
    /// The callback on the list required to be updated, i.e. a bluetooth device detected
    /// </summary>
    /// <param name="event">The lv event that identifies pressing the device entry</param>
    virtual void statusUpdate() {
        char valueString[32];
        float kmph = this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN).bikeStateAttributeValue.valueFloat * this->wheelCircumferenceMm * 60.0f / 1000000.0f;
        float mph = kmph * 0.621371;
        sprintf(valueString, "%.1f", kmph);
        lv_label_set_text(this->value_obj, valueString);
    }

    /// <summary>
    /// This will ensure the stats are initialised and the correct interest for the monitor is assigned to the controller
    /// </summary>
    virtual void initBluetoothStats() {
        BaseNumericSmall::initBluetoothStats();
        this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::WHEEL_CIRCUMFERENCE, MonitorAttributeType::ONCE);
        this->wheelCircumferenceMm = this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::WHEEL_CIRCUMFERENCE).bikeStateAttributeValue.valueUint16;
        this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS, MonitorAttributeType::EVERY_TEN_SECONDS);
    }

};

#endif