#include "BluetoothBikeState.h"

BikeState::BikeState()
{
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BATTERY_CAPACITY)].bikeStateAttributeType = BikeStateAttributeType::UINT16_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BATTERY_CAPACITY_REMAINING)].bikeStateAttributeType = BikeStateAttributeType::UINT16_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BATTERY_CHARGE_CYCLES)].bikeStateAttributeType = BikeStateAttributeType::UINT16_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BATTERY_VOLTAGE)].bikeStateAttributeType = BikeStateAttributeType::FLOAT_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BATTERY_CURRENT)].bikeStateAttributeType = BikeStateAttributeType::FLOAT_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BATTERY_HEALTH)].bikeStateAttributeType = BikeStateAttributeType::UINT8_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BATTERY_CHARGE_PERCENT)].bikeStateAttributeType = BikeStateAttributeType::UINT8_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BATTERY_TEMP)].bikeStateAttributeType = BikeStateAttributeType::UINT8_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BATTERY_HARDWARE_VERSION)].bikeStateAttributeType = BikeStateAttributeType::HARDWARE_VERSION;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BATTERY_FIRMWARE_VERSION)].bikeStateAttributeType = BikeStateAttributeType::FIRMWARE_VERSION;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BATTERY_SERIAL_NUMBER)].bikeStateAttributeType = BikeStateAttributeType::UINT16_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::RIDER_POWER)].bikeStateAttributeType = BikeStateAttributeType::UINT16_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::MOTOR_CADENCE)].bikeStateAttributeType = BikeStateAttributeType::FLOAT_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::MOTOR_SPEED)].bikeStateAttributeType = BikeStateAttributeType::FLOAT_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::MOTOR_POWER)].bikeStateAttributeType = BikeStateAttributeType::UINT16_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::MOTOR_ODOMETER)].bikeStateAttributeType = BikeStateAttributeType::UINT32_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::MOTOR_ASSIST_LEVEL)].bikeStateAttributeType = BikeStateAttributeType::UINT16_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::MOTOR_TEMP)].bikeStateAttributeType = BikeStateAttributeType::UINT8_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::MOTOR_HARDWARE_VERSION)].bikeStateAttributeType = BikeStateAttributeType::HARDWARE_VERSION;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::MOTOR_FIRMWARE_VERSION)].bikeStateAttributeType = BikeStateAttributeType::FIRMWARE_VERSION;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::MOTOR_SERIAL_NUMBER)].bikeStateAttributeType = BikeStateAttributeType::NUMBER_STRING;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::PEAK_POWER_ASSIST_LEVELS)].bikeStateAttributeType = BikeStateAttributeType::ASSIST_LEVELS;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::SUPPORT_ASSIST_LEVELS)].bikeStateAttributeType = BikeStateAttributeType::ASSIST_LEVELS;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::WHEEL_CIRCUMFERENCE)].bikeStateAttributeType = BikeStateAttributeType::UINT16_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::FAKE_CHANNEL_NUMBER)].bikeStateAttributeType = BikeStateAttributeType::UINT8_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BIKE_ON_OFF_STATE)].bikeStateAttributeType = BikeStateAttributeType::UINT8_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BATTERY_CONNECTED_STATE)].bikeStateAttributeType = BikeStateAttributeType::BOOL;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BIKE_SERIAL_NUMBER)].bikeStateAttributeType = BikeStateAttributeType::NUMBER_STRING;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BEEP_ON_OFF_STATE)].bikeStateAttributeType = BikeStateAttributeType::BOOL;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::WHEEL_ROTATIONS)].bikeStateAttributeType = BikeStateAttributeType::SPEED_CADENCE_READING;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN)].bikeStateAttributeType = BikeStateAttributeType::FLOAT_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::CRANK_ROTATIONS)].bikeStateAttributeType = BikeStateAttributeType::SPEED_CADENCE_READING;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::CRANK_ROTATIONS_PER_MIN)].bikeStateAttributeType = BikeStateAttributeType::FLOAT_T;
}

void BikeState::setAllMonitorAttributeType(MonitorAttributeType monitorAttributeType) {
    for (int i = 0 ; i < static_cast<int>(BikeStateAttributeIndex::BIKE_STATE_ATTRIBUTE_SIZE) ; i++ ) { 
        bikeStateAttributes[i].monitorAttributeType = monitorAttributeType;
    }
}

void BikeState::setAllLastFetchTimeTicks(uint32_t lastFetchTimeTicks) {
    for (int i = 0; i < static_cast<int>(BikeStateAttributeIndex::BIKE_STATE_ATTRIBUTE_SIZE); i++) {
        bikeStateAttributes[i].lastFetchTimeTicks = lastFetchTimeTicks;
    }
}

void BikeState::setAllAttributeValueOfType(BikeStateAttributeType bikeStateAttributeType, const BikeStateAttribute::BikeStateAttributeValue& bikeStateAttributeValue) {
    for (int i = 0; i < static_cast<int>(BikeStateAttributeIndex::BIKE_STATE_ATTRIBUTE_SIZE); i++) {
        if (bikeStateAttributes[i].bikeStateAttributeType == bikeStateAttributeType)
            bikeStateAttributes[i].bikeStateAttributeValue = bikeStateAttributeValue;
    }
}

bool BikeState::setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, BikeStateAttribute::BikeStateAttributeValue& bikeStateAttributeValue, uint32_t lastFetchTimeTicks) {
    this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].bikeStateAttributeValue = bikeStateAttributeValue;    
    return this->setStateAttribute(bikeStateAttributeIndex, lastFetchTimeTicks);
}

bool BikeState::setStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, uint32_t lastFetchTimeTicks) {
    this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].lastFetchTimeTicks = lastFetchTimeTicks;
    MonitorAttributeType monitorAttributeType = this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].monitorAttributeType;
    if (monitorAttributeType == MonitorAttributeType::ONCE) this->bikeStateAttributes[static_cast<int>(bikeStateAttributeIndex)].monitorAttributeType = MonitorAttributeType::ALWAYS_IGNORE;
    return monitorAttributeType != MonitorAttributeType::ALWAYS_IGNORE;
}

BikeStateAttributeIndex BikeState::getOldestStateAttribute(MonitorAttributeType monitorAttributeType) {
    uint32_t lowestTimeStamp = ~0;
    int lowestAttributeIndex = static_cast<int>(BikeStateAttributeIndex::BIKE_STATE_ATTRIBUTE_SIZE);
    for (int i = 0; i < static_cast<int>(BikeStateAttributeIndex::BIKE_STATE_ATTRIBUTE_SIZE); i++) {
        if ((monitorAttributeType == MonitorAttributeType::ALWAYS_IGNORE || this->bikeStateAttributes[i].monitorAttributeType == monitorAttributeType) &&
            this->bikeStateAttributes[i].lastFetchTimeTicks < lowestTimeStamp) {
            lowestTimeStamp = this->bikeStateAttributes[i].lastFetchTimeTicks;
            lowestAttributeIndex = i;
        }
    }
    return static_cast<BikeStateAttributeIndex>(lowestAttributeIndex);
}

BikeStateAttributeIndex BikeState::getNewestStateAttribute(MonitorAttributeType monitorAttributeType) {
    uint32_t highestTimeStamp = 0;
    int highestAttributeIndex = static_cast<int>(BikeStateAttributeIndex::BIKE_STATE_ATTRIBUTE_SIZE);
    for (int i = 0; i < static_cast<int>(BikeStateAttributeIndex::BIKE_STATE_ATTRIBUTE_SIZE); i++) {
        if ((monitorAttributeType == MonitorAttributeType::ALWAYS_IGNORE || this->bikeStateAttributes[i].monitorAttributeType == monitorAttributeType) && 
            this->bikeStateAttributes[i].lastFetchTimeTicks > highestTimeStamp) {
            highestTimeStamp = this->bikeStateAttributes[i].lastFetchTimeTicks;
            highestAttributeIndex = i;
        }
    }
    return static_cast<BikeStateAttributeIndex>(highestAttributeIndex);
}