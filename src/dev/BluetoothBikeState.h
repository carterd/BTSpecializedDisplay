#ifndef _BLUETOOTH_BIKE_STATE_H
#define _BLUETOOTH_BIKE_STATE_H

#include <Arduino.h>
#include "BluetoothBikeDefinitions.h"

enum class MonitorAttributeType {
    ALWAYS_IGNORE = 0,             // We basically are not interested in changes in this attribute
    ONCE = 1,               // Read the value just the once
    EVERY_MINUTE = 2,       // Ensure at some time at least values get an explicit update
    EVERY_SECOND = 3,       // Maybe some instantanious value that we need to keep track of
    CONSTANTLY = 4,         // Not recomended unless it's a special case status    
};


template<class T>
class MonitoredAttribute {
protected:
    T value;    
public:
    MonitorAttributeType type;
    uint32_t time;
    T getValue() { return value; }
    T getValueConst() const { return value; }
    bool setValueAndTime(T newValue, int32_t time) {
        if (this->type == MonitorAttributeType::ALWAYS_IGNORE) return false;
        if (this->type == MonitorAttributeType::ONCE) this->type = MonitorAttributeType::ALWAYS_IGNORE;
        this->value = newValue;
        this->time = time;
        return true;
    }
    bool setTime(int32_t time) {
        if (this->type == MonitorAttributeType::ALWAYS_IGNORE) return false;
        return true;
    }
    void setMonitorAttributeType(MonitorAttributeType monitorAttributeType) {
        if (this->type < monitorAttributeType)  this->type = monitorAttributeType;        
    }
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
    char numberString[32];
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
    }
    void setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, BikeStateAttribute& bikeStateAttribute) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)] = bikeStateAttribute;
    }

    void setMonitorAttributeType(MonitorAttributeType monitorAttributeType);

    void setLastFetchTimeTicks(uint32_t lastFetchTimeTicks);
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


struct BluetoothBikeState {
    // Battery State Details
    MonitoredAttribute<uint16_t> batterySerialNumber;
    MonitoredAttribute<uint16_t> batteryCapacity;
    MonitoredAttribute<uint16_t> batteryCapacityRemaining;
    MonitoredAttribute<uint16_t> batteryChargeCycles;
    MonitoredAttribute<float_t> batteryVoltage;
    MonitoredAttribute<float_t> batteryCurrent;
    MonitoredAttribute<uint8_t> batteryHealth;
    MonitoredAttribute<uint8_t> batteryChargePercent;
    MonitoredAttribute<uint8_t> batteryTemp;
    MonitoredAttribute<HardwareVersion> batteryHardwareVersion;
    MonitoredAttribute<FirmwareVersion> batteryFirmwareVersion;
    // Motor State Details
    MonitoredAttribute<uint16_t> riderPower;
    MonitoredAttribute<float_t> motorCadence;
    MonitoredAttribute<float_t> motorSpeed;
    MonitoredAttribute<uint32_t> motorOdometer;
    MonitoredAttribute<uint16_t> motorAssistLevel;
    MonitoredAttribute<uint8_t> motorTemp;
    MonitoredAttribute<FirmwareVersion> motorFirmwareVersion;
    MonitoredAttribute<uint16_t> motorPower;
    MonitoredAttribute<AssistLevels> peakAssistLevels;
    MonitoredAttribute<NumberString> motorHardwareNumber;
    MonitoredAttribute<NumberString> motorSerailNumber;
    // Other State Details
    MonitoredAttribute<uint16_t> wheelCircumference;
    MonitoredAttribute<AssistLevels> assistPercentage;
    MonitoredAttribute<uint8_t> fakeChannelNumber;
    MonitoredAttribute<uint8_t> bikeOnOffState;
    MonitoredAttribute<NumberString> bikeSerialNumber;
    MonitoredAttribute<bool> batteryConnected;

    void setMonitorAttributeType(MonitorAttributeType monitorAttributeType) {
        batterySerialNumber.type = 
        batteryCapacity.type =
        batteryCapacityRemaining.type =
        batteryChargeCycles.type =
        batteryVoltage.type =
        batteryCurrent.type =
        batteryHealth.type =
        batteryChargePercent.type =
        batteryTemp.type = 
        batteryHardwareVersion.type = 
        batteryFirmwareVersion.type =    
        riderPower.type =
        motorCadence.type =
        motorSpeed.type =
        motorOdometer.type =
        motorAssistLevel.type =
        motorTemp.type =
        motorFirmwareVersion.type =
        motorPower.type =
        peakAssistLevels.type =
        motorHardwareNumber.type =
        motorSerailNumber.type =
        wheelCircumference.type =
        assistPercentage.type =
        fakeChannelNumber.type =
        bikeOnOffState.type =
        bikeSerialNumber.type =
        batteryConnected.type = monitorAttributeType;
    }

    void setTime(uint32_t time) {
        batterySerialNumber.time = 
        batteryCapacity.time =
        batteryCapacityRemaining.time =
        batteryChargeCycles.time =
        batteryVoltage.time =
        batteryCurrent.time =
        batteryHealth.time =
        batteryChargePercent.time =
        batteryTemp.time = 
        batteryHardwareVersion.time = 
        batteryFirmwareVersion.time =    
        riderPower.time =
        motorCadence.time =
        motorSpeed.time =
        motorOdometer.time =
        motorAssistLevel.time =
        motorTemp.time =
        motorFirmwareVersion.time =
        motorPower.time =
        peakAssistLevels.time =
        motorHardwareNumber.time =
        motorSerailNumber.time =
        wheelCircumference.time =
        assistPercentage.time =
        fakeChannelNumber.time =
        bikeOnOffState.time =
        bikeSerialNumber.time =
        batteryConnected.time = time;
    }

};

#endif