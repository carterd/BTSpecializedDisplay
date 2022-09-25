#ifndef _BLUETOOTH_BIKE_STATE_H
#define _BLUETOOTH_BIKE_STATE_H

#include <Arduino.h>

class BluetoothBikeState {
public:
    uint16_t BatterySerialNumber;
    uint16_t BatteryCapacity;
    uint16_t BatteryCapacityRemaining;
    uint16_t BatteryChargeCycles;
    uint8_t BatteryVoltage;
    uint8_t BatteryCurrent;
    uint8_t BatteryHealth;
    uint8_t BatteryChargePercent;
    uint8_t BatteryTemp;
    uint16_t BatteryHardwareVersion;
    uint16_t BatteryFirmwareVersion;
};

#endif