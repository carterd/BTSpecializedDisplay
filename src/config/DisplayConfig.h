#ifndef  _DISPLAY_CONFIG_H
#define  _DISPLAY_CONFIG_H

#include <Arduino.h>

enum class DisplayConfigAttributeIndex { Contrast = 1, MonitorType = 2, ConnectBatteryOnly = 3, ConnectOnBoot = 4, UnitsMetricImperial = 5 };

struct DisplayConfig {
public:
    uint8_t contrast;                // Between 0x2F and 0x00
    uint16_t monitorType;            // Enumeration of preset monitor settings
    bool connectBatteryOnly;        // This is more of a debug feature as you normally wont want to connect to battery only
    bool connectOnBoot;             // This is more likely to want to default to true
    bool unitsMetric;

    DisplayConfig(int8_t contrast, int16_t monitorType, bool connectBatteryOnly, bool connectOnBoot, bool unitsMetric) {
        this->contrast = contrast;
        this->monitorType = monitorType;
        this->connectBatteryOnly = connectBatteryOnly;
        this->connectOnBoot = connectOnBoot;
        this->unitsMetric = unitsMetric;
    };
    DisplayConfig() {};
    friend bool operator==(const DisplayConfig& lhs, const DisplayConfig& rhs) {
        return (lhs.contrast == rhs.contrast && lhs.monitorType == rhs.monitorType && lhs.connectBatteryOnly == rhs.connectBatteryOnly && lhs.connectOnBoot == rhs.connectOnBoot && lhs.unitsMetric == rhs.unitsMetric);
    }
    void setIntValue(DisplayConfigAttributeIndex displayConfigAttributeIndex, int newValue) {
        switch (displayConfigAttributeIndex) {
            case DisplayConfigAttributeIndex::Contrast:
            this->contrast = newValue;
            break;
            case DisplayConfigAttributeIndex::MonitorType:
            this->monitorType = newValue;
            break;
            case DisplayConfigAttributeIndex::ConnectBatteryOnly:
            this->connectBatteryOnly = newValue;
            break;
            case DisplayConfigAttributeIndex::ConnectOnBoot:
            this->connectOnBoot = newValue;
            break;
            case DisplayConfigAttributeIndex::UnitsMetricImperial:
            this->unitsMetric = newValue;
            break;
        }
    };
    int getIntValue(DisplayConfigAttributeIndex displayConfigAttributeIndex) {
        int initValue = 0;
        switch (displayConfigAttributeIndex) {
            case DisplayConfigAttributeIndex::Contrast:
            initValue = this->contrast;
            break;
            case DisplayConfigAttributeIndex::MonitorType:
            initValue = this->monitorType;
            break;
            case DisplayConfigAttributeIndex::ConnectBatteryOnly:
            initValue = this->connectBatteryOnly;
            break;
            case DisplayConfigAttributeIndex::ConnectOnBoot:
            initValue = this->connectOnBoot;
            break;
            case DisplayConfigAttributeIndex::UnitsMetricImperial:
            initValue = this->unitsMetric;
            break;
        }
        return initValue;
    };
};

#endif