#ifndef _SPEED_MONITOR_SMALL_H
#define _SPEED_MONITOR_SMALL_H

#include "BaseNumericMonitorSmall.h"
#include "..\..\..\dev\ConfigStore.h"


class SpeedMonitorSmall : public BaseNumericMonitorSmall
{
private:
    uint16_t wheelCircumferenceMm;

    /// <summary>
    /// The config store which contains the bluetooth devices to connect to
    /// </summary>
    ConfigStore* configStore;

    /// <summary>
    /// We may need to display differently depending on the config
    /// </summary>
    const char* attributeUnitsMetric;

    /// <summary>
    /// We may need to display differently depending on the config
    /// </summary>
    const char* attributeUnitsImperial;

    bool displayMetric = true;

public:
    SpeedMonitorSmall(ConfigStore* configStore, const char* title = " " LV_SYMBOL_PLAY , const char* unitsMetric = " kph", const char* unitsImperial = " mph") : BaseNumericMonitorSmall(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN, MonitorAttributeType::EVERY_SECOND, title, "") {
        this->configStore = configStore;
        this->attributeUnitsMetric = unitsMetric;
        this->attributeUnitsImperial = unitsImperial;
    }

    /// <summary>
    /// The callback on the list required to be updated, i.e. a bluetooth device detected
    /// </summary>
    /// <param name="event">The lv event that identifies pressing the device entry</param>
    virtual void updateLvOb() {
        char valueString[32];
        float kmph = this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN).bikeStateAttributeValue.valueFloat * this->wheelCircumferenceMm * 60.0f / 1000000.0f;
        float mph = kmph * 0.621371;
        sprintf(valueString, "%.1f%s", (this->displayMetric ? kmph : mph), this->attributeUnits);
        lv_label_set_text(this->value_obj, valueString);
    }

    /// <summary>
    /// This will ensure the stats are initialised and the correct interest for the monitor is assigned to the controller
    /// </summary>
    virtual void initBluetoothStats() {
        BaseNumericMonitorSmall::initBluetoothStats();
        this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::WHEEL_CIRCUMFERENCE, MonitorAttributeType::ONCE);
        BikeConfig bikeConfig = this->configStore->getBikeConfig();
        DisplayConfig displayConfig = this->configStore->getDisplayConfig();
        if (bikeConfig.wheelCircumference.managed)
            this->wheelCircumferenceMm = (bikeConfig.wheelCircumference.managed) ? bikeConfig.wheelCircumference.value : this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::WHEEL_CIRCUMFERENCE).bikeStateAttributeValue.valueUint16;
        this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS, MonitorAttributeType::EVERY_TEN_SECONDS);
        
        this->displayMetric = displayConfig.unitsMetric;
        this->attributeUnits = (this->displayMetric ? this->attributeUnitsMetric : this->attributeUnitsImperial);
    }

};

#endif