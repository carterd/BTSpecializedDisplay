#ifndef _BLUETOOTH_BIKE_STATE_H
#define _BLUETOOTH_BIKE_STATE_H

#include <Arduino.h>
#include "BluetoothBikeDefinitions.h"

enum class MonitorAttributeType {
    ALWAYS_IGNORE = 0,             // We basically are not interested in changes in this attribute
    ONCE = 1,               // Read the value just the once
    EVERY_MINUTE = 2,       // Ensure at some time at least values get an explicit update
    EVERY_TEN_SECONDS = 3,
    EVERY_SECOND = 4,       // Maybe some instantanious value that we need to keep track of    
    CONSTANTLY = 5,         // Not recomended unless it's a special case status    
};

struct HardwareVersion {
    uint8_t majorNumber;
    uint8_t minorNumber;
};

struct FirmwareVersion {
    uint8_t majorNumber;
    uint8_t minorNumber;
    uint8_t patchNumber;
};

struct AssistLevels {
    uint8_t eco;
    uint8_t trail;
    uint8_t turbo;
};

struct NumberString {
    char value[32];
};

enum class BikeStateAttributeIndex {    
    BATTERY_CAPACITY = 0,
    BATTERY_CAPACITY_REMAINING,
    BATTERY_CHARGE_CYCLES,
    BATTERY_VOLTAGE,
    BATTERY_CURRENT,
    BATTERY_HEALTH,
    BATTERY_CHARGE_PERCENT,
    BATTERY_TEMP,
    BATTERY_HARDWARE_VERSION,
    BATTERY_FIRMWARE_VERSION,
    BATTERY_SERIAL_NUMBER,
    RIDER_POWER,
    MOTOR_CADENCE,
    MOTOR_SPEED,
    MOTOR_POWER,
    MOTOR_ODOMETER,
    MOTOR_ASSIST_LEVEL,
    MOTOR_TEMP,
    MOTOR_HARDWARE_VERSION,
    MOTOR_FIRMWARE_VERSION,
    MOTOR_SERIAL_NUMBER,
    PEAK_ASSIST_LEVELS,
    ASSIST_PERCENTAGE,
    WHEEL_CIRCUMFERENCE,
    FAKE_CHANNEL_NUMBER,
    BIKE_ON_OFF_STATE,
    BATTERY_CONNECTED_STATE,
    BEEP_ON_OFF_STATE,
    BIKE_SERIAL_NUMBER,
    ///
    BIKE_STATE_ATTRIBUTE_SIZE
};

enum class BikeStateAttributeType {
    UINT8_T = 0,
    UINT16_T,
    UINT32_T,
    FLOAT_T,
    HARDWARE_VERSION,
    FIRMWARE_VERSION,
    ASSIST_LEVELS,
    NUMBER_STRING,
    BOOL,
};

class BikeStateAttribute {
public:
    union BikeStateAttributeValue {
        bool valueBool;
        uint8_t valueUint8;
        uint16_t valueUint16;
        uint32_t valueUint32;
        float valueFloat;
        HardwareVersion valueHardwareVersion;
        FirmwareVersion valueFirmwareVersion;
        AssistLevels assistLevels;
        NumberString numberString;
    };
public:
    BikeStateAttributeType bikeStateAttributeType;
    BikeStateAttributeValue bikeStateAttributeValue;
    MonitorAttributeType monitorAttributeType;
    uint32_t lastFetchTimeTicks;
public:
    BikeStateAttribute() {
        this->monitorAttributeType = MonitorAttributeType::ALWAYS_IGNORE;
        this->lastFetchTimeTicks = 0;
    }
    BikeStateAttribute(BikeStateAttributeType bikeStateAttributeType) {
        this->bikeStateAttributeType = bikeStateAttributeType;
        this->monitorAttributeType = MonitorAttributeType::ALWAYS_IGNORE;
        this->lastFetchTimeTicks = 0;
    }
    BikeStateAttribute(BikeStateAttributeType bikeStateAttributeType, BikeStateAttributeValue& bikeStateAttributeValue) {
        this->bikeStateAttributeType = bikeStateAttributeType;
        this->bikeStateAttributeValue = bikeStateAttributeValue;
        this->monitorAttributeType = MonitorAttributeType::ALWAYS_IGNORE;
        this->lastFetchTimeTicks = 0;
    }
};

class BikeState {
private:
    BikeStateAttribute bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BIKE_STATE_ATTRIBUTE_SIZE)];    
public:
    BikeState();

    BikeStateAttribute& getStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex) {
        return this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)];
    };

    void setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, BikeStateAttribute& bikeStateAttribute) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)] = bikeStateAttribute;
    }

    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, BikeStateAttribute::BikeStateAttributeValue& bikeStateAttribute, uint32_t lastFetchTimeTicks);

    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, uint32_t lastFetchTimeTicks);

    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, NumberString& bikeStateAttribute, uint32_t lastFetchTimeTicks) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue.numberString = bikeStateAttribute;
        return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
    }

    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, HardwareVersion& bikeStateAttribute, uint32_t lastFetchTimeTicks) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue.valueHardwareVersion = bikeStateAttribute;
        return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
    }

    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, FirmwareVersion& bikeStateAttribute, uint32_t lastFetchTimeTicks) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue.valueFirmwareVersion = bikeStateAttribute;
        return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
    }

    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, AssistLevels& bikeStateAttribute, uint32_t lastFetchTimeTicks) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue.assistLevels = bikeStateAttribute;
        return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
    }

    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, float bikeStateAttribute, uint32_t lastFetchTimeTicks) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue.valueFloat = bikeStateAttribute;
        return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
    }

    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, uint32_t bikeStateAttribute, uint32_t lastFetchTimeTicks) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue.valueUint32 = bikeStateAttribute;
        return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
    }

    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, uint16_t bikeStateAttribute, uint32_t lastFetchTimeTicks) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue.valueUint16 = bikeStateAttribute;
        return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
    }

    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, uint8_t bikeStateAttribute, uint32_t lastFetchTimeTicks) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue.valueUint8 = bikeStateAttribute;
        return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
    }

    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, bool bikeStateAttribute, uint32_t lastFetchTimeTicks) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue.valueBool = bikeStateAttribute;
        return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
    }

    void setMonitorAttributeType(BikeStateAttributeIndex bikeStateAttributeIndex, MonitorAttributeType monitorAttributeType) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].monitorAttributeType = monitorAttributeType;
    }

    BikeStateAttributeIndex getOldestStateAttribute(MonitorAttributeType monitorAttributeType);

    void setAllMonitorAttributeType(MonitorAttributeType monitorAttributeType);

    void setAllLastFetchTimeTicks(uint32_t lastFetchTimeTicks);
};


class BluetoothBikeRequest {
public:
    struct BluetoothBikeRequestCommand {
        EbikeStatusArea area;
        EbikeStatusAttribute attribute;
        BluetoothBikeRequestCommand() { area = EbikeStatusArea::INVALID; attribute.ebikeStatusBattery = EbikeStatusBattery::CAPACITY; }
        BluetoothBikeRequestCommand(EbikeStatusBattery ebikeStatusBattery) { area = EbikeStatusArea::BATTERY; attribute.ebikeStatusBattery = ebikeStatusBattery; }
        BluetoothBikeRequestCommand(EbikeStatusMotor ebikeStatusMotor) { area = EbikeStatusArea::MOTOR; attribute.ebikeStatusMotor = ebikeStatusMotor; }
        BluetoothBikeRequestCommand(EbikeStatusOther ebikeStatusOther) { area = EbikeStatusArea::OTHER; attribute.ebikeStatusOther = ebikeStatusOther; }
    };
    BluetoothBikeRequestCommand request1;
    // Bit wastful on memory
    BluetoothBikeRequestCommand request2;
    BluetoothBikeRequest(BluetoothBikeRequestCommand request1, BluetoothBikeRequestCommand request2) {
        this->request1 = request1; this->request2 = request2;
    }
    BluetoothBikeRequest(BluetoothBikeRequestCommand request1) {
        this->request1 = request1; this->request2 = BluetoothBikeRequestCommand();
    }
    BluetoothBikeRequest() { }
};

class BikeStateToBluetoothBikeRequest {
public:
    BluetoothBikeRequest bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BIKE_STATE_ATTRIBUTE_SIZE)];
public:
    BikeStateToBluetoothBikeRequest();
    BluetoothBikeRequest getBluetoothBikeRequest(BikeStateAttributeIndex bikeStateAttributeIndex, BikeType bikeType);
};

#endif