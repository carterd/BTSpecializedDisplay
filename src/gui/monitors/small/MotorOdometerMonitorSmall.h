#ifndef _MOTOR_ODOMETER_MONITOR_SMALL_H
#define _MOTOR_ODOMETER_MONITOR_SMALL_H

#include "BaseNumericMonitorSmall.h"


class MotorOdometerMonitorSmall : public BaseNumericMonitorSmall
{
private:
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
    MotorOdometerMonitorSmall(ConfigStore* configStore, const char* title = "odo", const char* unitsMetric = " km", const char* unitsImperial = " m") : BaseNumericMonitorSmall(BikeStateAttributeIndex::MOTOR_ODOMETER, MonitorAttributeType::EVERY_MINUTE, title, attributeUnits) {
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
        float km = this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::MOTOR_ODOMETER).bikeStateAttributeValue.valueUint32 / 1000.0f;
        float miles = km * 0.621371;

        char* previousLabel = lv_label_get_text(this->value_obj);
        
	    if ((*previousLabel != 0) && (this->previousBikeStateAttribute.valueFloat == this->displayMetric ? km : miles)) return;

        sprintf(valueString, "%.0f%s", (this->displayMetric ? km : miles), this->attributeUnits);
        this->previousBikeStateAttribute.valueFloat = this->displayMetric ? km : miles;

        lv_label_set_text(this->value_obj, valueString);
    }

    /// <summary>
    /// This will ensure the stats are initialised and the correct interest for the monitor is assigned to the controller
    /// </summary>
    virtual void initBluetoothStats() {
        BaseNumericMonitorSmall::initBluetoothStats();
        DisplayConfig displayConfig = this->configStore->getDisplayConfig();

        this->displayMetric = this->configStore->getDisplayConfig().unitsMetric;
        this->attributeUnits = (this->displayMetric ? this->attributeUnitsMetric : this->attributeUnitsImperial);
    }
};

#endif