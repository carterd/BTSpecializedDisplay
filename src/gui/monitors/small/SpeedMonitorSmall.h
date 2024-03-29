#ifndef _SPEED_MONITOR_SMALL_H
#define _SPEED_MONITOR_SMALL_H

#include "BaseNumericMonitorSmall.h"
#include "../../../dev/ConfigStore.h"

#define KMPH_TO_MPH_MULTIPLIER 0.621371

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
    /// <summary>
    /// If true show the measurement in metric otherwise use imperial
    /// </summary>
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
    virtual void updateLvObj() {
        char valueString[32];
        // If no bike then ignore the update
        if (!this->bluetoothBike) return;

        float kmph = this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN).bikeStateAttributeValue.valueFloat * this->wheelCircumferenceMm * 60.0f / 1000000.0f;
        float mph = kmph * KMPH_TO_MPH_MULTIPLIER;

        sprintf(valueString, "%.1f%s", (this->displayMetric ? kmph : mph), this->attributeUnits);
        this->previousBikeStateAttribute.valueFloat = this->displayMetric ? kmph : mph;
        lv_label_set_text(this->value_obj, valueString);
    }

    /// <summary>
    /// This will ensure the stats are initialised and the correct interest for the monitor is assigned to the controller
    /// </summary>
    virtual void initBluetoothStats() {
        BaseNumericMonitorSmall::initBluetoothStats();
        this->bluetoothBike->readBikeStateAttribute(BikeStateAttributeIndex::WHEEL_CIRCUMFERENCE, MonitorAttributeType::ONCE);
        BikeConfig bikeConfig = this->configStore->getBikeConfig();
        DisplayConfig displayConfig = this->configStore->getDisplayConfig();

        this->wheelCircumferenceMm = (bikeConfig.wheelCircumference.managed) ? bikeConfig.wheelCircumference.value : this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::WHEEL_CIRCUMFERENCE).bikeStateAttributeValue.valueUint16;
        this->bluetoothBike->readBikeStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS, MonitorAttributeType::EVERY_TEN_SECONDS);
        
        this->displayMetric = displayConfig.unitsMetric;
        this->attributeUnits = (this->displayMetric ? this->attributeUnitsMetric : this->attributeUnitsImperial);
    }

};

#endif