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

struct SpeedCadenceReading {
    uint32_t rotationCount;
    uint32_t eventTime;
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
    PEAK_POWER_ASSIST_LEVELS,
    SUPPORT_ASSIST_LEVELS,
    WHEEL_CIRCUMFERENCE,
    FAKE_CHANNEL_NUMBER,
    BIKE_ON_OFF_STATE,
    BATTERY_CONNECTED_STATE,
    BEEP_ON_OFF_STATE,
    BIKE_SERIAL_NUMBER,
    WHEEL_ROTATIONS,
    WHEEL_ROTATIONS_PER_MIN,
    CRANK_ROTATIONS,
    CRANK_ROTATIONS_PER_MIN,
    ///
    BIKE_STATE_ATTRIBUTE_SIZE
};

enum class BikeStateAttributeType {
    BOOL = 0,
    UINT8_T,
    UINT16_T,
    UINT32_T,
    FLOAT_T,
    HARDWARE_VERSION,
    FIRMWARE_VERSION,
    ASSIST_LEVELS,
    NUMBER_STRING,
    SPEED_CADENCE_READING,
};

/// <summary>
/// BikeStateAttribute which contains the data for the state and a MonitorAttribute type for how often
/// the given attribute should be monitored, as well as details such as last fetchtime ticks. Which is the RP2040
/// time ticks not say any time stored within the bike.
/// </summary>
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
        AssistLevels valueAssistLevels;
        NumberString valueNumberString;
        SpeedCadenceReading cscReading;
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

/// <summary>
/// This is a representation of the current know state of the bike and it's various attributes.
/// Each state attribute is indexed by BikeStateAttributeIndex, and each attribute is an instance of
/// BikeStateAttribute which both contains the data for the state and a MonitorAttribute type for how often
/// the given attribute should be monitored.
/// </summary>
class BikeState {
private:
    BikeStateAttribute bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BIKE_STATE_ATTRIBUTE_SIZE)];    

public:
    /// <summary>
    /// Constructor for the BikeState class defaults to initialse the bike state to a default state
    /// </summary>
    BikeState();

    /// <summary>
    /// Accessor that returns the given attribute indexed by the given bikeStateAttributeIndex
    /// </summary>
    BikeStateAttribute& getStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex) {
        return this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)];
    };

    /// <summary>
    /// Accessor that sets the given attribute indexed by the given bikeStateAttributeIndex, with the given bikeStateAttribute.
    /// </summary>
    void setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, BikeStateAttribute& bikeStateAttribute) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)] = bikeStateAttribute;
    }

    /// <summary>
    /// Accessor that sets the given attribute indexed by the given bikeStateAttributeIndex, simply updating the lastFetchTime.
    /// The bike state attribute will also have it's monitorAttributeType updated if it's MonitorAttributeType::ONCE to MonitorAttributeType::IGNORE.
    /// </summary>
    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, uint32_t lastFetchTimeTicks);

    /// <summary>
    /// Accessor that sets the given attribute indexed by the given bikeStateAttributeIndex, with the given bikeStateAttributeValue and lastFetchTime.
    /// The bike state attribute will also have it's monitorAttributeType updated if it's MonitorAttributeType::ONCE to MonitorAttributeType::IGNORE.
    /// </summary>
    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, BikeStateAttribute::BikeStateAttributeValue& bikeStateAttribute, uint32_t lastFetchTimeTicks);

    /// <summary>
    /// Accessor that sets the given attribute indexed by the given bikeStateAttributeIndex, with the given SpeedCadenceReading and lastFetchTime.
    /// The bike state attribute will also have it's monitorAttributeType updated if it's MonitorAttributeType::ONCE to MonitorAttributeType::IGNORE.
    /// </summary>
    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, SpeedCadenceReading& bikeStateAttribute, uint32_t lastFetchTimeTicks) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue.cscReading = bikeStateAttribute;
        return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
    }

    /// <summary>
    /// Accessor that sets the given attribute indexed by the given bikeStateAttributeIndex, with the given NumberString and lastFetchTime.
    /// The bike state attribute will also have it's monitorAttributeType updated if it's MonitorAttributeType::ONCE to MonitorAttributeType::IGNORE.
    /// </summary>
    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, NumberString& bikeStateAttribute, uint32_t lastFetchTimeTicks) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue.valueNumberString = bikeStateAttribute;
        return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
    }

    /// <summary>
    /// Accessor that sets the given attribute indexed by the given bikeStateAttributeIndex, with the given HardwareVersion and lastFetchTime.
    /// The bike state attribute will also have it's monitorAttributeType updated if it's MonitorAttributeType::ONCE to MonitorAttributeType::IGNORE.
    /// </summary>
    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, HardwareVersion& bikeStateAttribute, uint32_t lastFetchTimeTicks) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue.valueHardwareVersion = bikeStateAttribute;
        return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
    }

    /// <summary>
    /// Accessor that sets the given attribute indexed by the given bikeStateAttributeIndex, with the given FirmwareVersion and lastFetchTime.
    /// The bike state attribute will also have it's monitorAttributeType updated if it's MonitorAttributeType::ONCE to MonitorAttributeType::IGNORE.
    /// </summary>
    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, FirmwareVersion& bikeStateAttribute, uint32_t lastFetchTimeTicks) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue.valueFirmwareVersion = bikeStateAttribute;
        return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
    }

    /// <summary>
    /// Accessor that sets the given attribute indexed by the given bikeStateAttributeIndex, with the given AssistLevels and lastFetchTime.
    /// The bike state attribute will also have it's monitorAttributeType updated if it's MonitorAttributeType::ONCE to MonitorAttributeType::IGNORE.
    /// </summary>
    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, AssistLevels& bikeStateAttribute, uint32_t lastFetchTimeTicks) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue.valueAssistLevels = bikeStateAttribute;
        return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
    }

    /// <summary>
    /// Accessor that sets the given attribute indexed by the given bikeStateAttributeIndex, with the given float and lastFetchTime.
    /// The bike state attribute will also have it's monitorAttributeType updated if it's MonitorAttributeType::ONCE to MonitorAttributeType::IGNORE.
    /// </summary>
    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, float bikeStateAttribute, uint32_t lastFetchTimeTicks) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue.valueFloat = bikeStateAttribute;
        return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
    }

    /// <summary>
    /// Accessor that sets the given attribute indexed by the given bikeStateAttributeIndex, with the given uint32_t and lastFetchTime.
    /// The bike state attribute will also have it's monitorAttributeType updated if it's MonitorAttributeType::ONCE to MonitorAttributeType::IGNORE.
    /// </summary>
    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, uint32_t bikeStateAttribute, uint32_t lastFetchTimeTicks) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue.valueUint32 = bikeStateAttribute;
        return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
    }

    /// <summary>
    /// Accessor that sets the given attribute indexed by the given bikeStateAttributeIndex, with the given uint16_t and lastFetchTime.
    /// The bike state attribute will also have it's monitorAttributeType updated if it's MonitorAttributeType::ONCE to MonitorAttributeType::IGNORE.
    /// </summary>
    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, uint16_t bikeStateAttribute, uint32_t lastFetchTimeTicks) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue.valueUint16 = bikeStateAttribute;
        return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
    }

    /// <summary>
    /// Accessor that sets the given attribute indexed by the given bikeStateAttributeIndex, with the given uint8_t and lastFetchTime.
    /// The bike state attribute will also have it's monitorAttributeType updated if it's MonitorAttributeType::ONCE to MonitorAttributeType::IGNORE.
    /// </summary>
    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, uint8_t bikeStateAttribute, uint32_t lastFetchTimeTicks) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue.valueUint8 = bikeStateAttribute;
        return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
    }

    /// <summary>
    /// Accessor that sets the given attribute indexed by the given bikeStateAttributeIndex, with the given bool and lastFetchTime.
    /// The bike state attribute will also have it's monitorAttributeType updated if it's MonitorAttributeType::ONCE to MonitorAttributeType::IGNORE.
    /// </summary>
    bool setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, bool bikeStateAttribute, uint32_t lastFetchTimeTicks) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue.valueBool = bikeStateAttribute;
        return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
    }

    /// <summary>
    /// Accessor that sets the given monitorAttributeType of the attribute indexed by bikeStateAttributeIndex.
    /// </summary>
    void setMonitorAttributeType(BikeStateAttributeIndex bikeStateAttributeIndex, MonitorAttributeType monitorAttributeType) {
        this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].monitorAttributeType = monitorAttributeType;
    }

    /// <summary>
    /// gets the oldest state attribute of a given monitorAttributeType.
    /// </summary>
    /// Note that always ignore means simply get the oldest sate attribute
    BikeStateAttributeIndex getOldestStateAttribute(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ALWAYS_IGNORE);

    /// <summary>
    /// gets the newest state attribute of a given monitorAttributeType.
    /// </summary>
    /// Note that always ignore means simply get the oldest sate attribute
    BikeStateAttributeIndex getNewestStateAttribute(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ALWAYS_IGNORE);

    /// <summary>
    /// sets all attributes to the given monitorAttributeType, used for resetting bike state.
    /// </summary>
    void setAllMonitorAttributeType(MonitorAttributeType monitorAttributeType);

    /// <summary>
    /// sets all attributes to the given lastFetchTimeTicks, used for resetting bike state.
    /// </summary>
    void setAllLastFetchTimeTicks(uint32_t lastFetchTimeTicks);

    /// <summary>
    /// sets all attributes of a given Type to be the given BikeStateAttributeValue, used for resetting bike state.
    /// </summary>
    void setAllAttributeValueOfType(BikeStateAttributeType bikeStateAttributeType, const BikeStateAttribute::BikeStateAttributeValue& bikeStateAttributeValue);
};

#endif