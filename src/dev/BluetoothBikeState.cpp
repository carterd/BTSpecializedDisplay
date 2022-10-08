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
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::PEAK_ASSIST_LEVELS)].bikeStateAttributeType = BikeStateAttributeType::ASSIST_LEVELS;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::ASSIST_PERCENTAGE)].bikeStateAttributeType = BikeStateAttributeType::ASSIST_LEVELS;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::WHEEL_CIRCUMFERENCE)].bikeStateAttributeType = BikeStateAttributeType::UINT16_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::FAKE_CHANNEL_NUMBER)].bikeStateAttributeType = BikeStateAttributeType::UINT8_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BIKE_ON_OFF_STATE)].bikeStateAttributeType = BikeStateAttributeType::UINT8_T;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BATTERY_CONNECTED_STATE)].bikeStateAttributeType = BikeStateAttributeType::BOOL;
    this->bikeStateAttributes[static_cast<int>(BikeStateAttributeIndex::BEEP_ON_OFF_STATE)].bikeStateAttributeType = BikeStateAttributeType::BOOL;
}

BikeStateToBluetoothBikeRequest::BikeStateToBluetoothBikeRequest() {
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BATTERY_CAPACITY)] 
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusBattery::CAPACITY) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BATTERY_CAPACITY_REMAINING)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusBattery::CAPACITY_REMAIN) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BATTERY_CHARGE_CYCLES)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusBattery::CHARGE_CYCLES) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BATTERY_VOLTAGE)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusBattery::VOLTAGE) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BATTERY_CURRENT)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusBattery::CURRENT) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BATTERY_HEALTH)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusBattery::HEALTH) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BATTERY_CHARGE_PERCENT)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusBattery::CHARGE_PERCENT) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BATTERY_TEMP)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusBattery::TEMP) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BATTERY_HARDWARE_VERSION)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusBattery::HARDWARE_VERSION) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BATTERY_FIRMWARE_VERSION)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusBattery::FIRMWARE_VERSION) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BATTERY_SERIAL_NUMBER)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusBattery::SERIAL_NO_GEN1) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::RIDER_POWER)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusMotor::RIDER_POWER) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::MOTOR_CADENCE)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusMotor::CADENCE) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::MOTOR_SPEED)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusMotor::SPEED) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::MOTOR_POWER)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusMotor::MOTOR_POWER) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::MOTOR_ODOMETER)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusMotor::ODOMETER) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::MOTOR_ASSIST_LEVEL)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusMotor::ASSIST_LEVEL) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::MOTOR_TEMP)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusMotor::TEMP) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::MOTOR_HARDWARE_VERSION)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusMotor::HARDWARE_NO_PART1),
                                BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusMotor::HARDWARE_NO_PART2) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::MOTOR_FIRMWARE_VERSION)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusMotor::FIRMWARE_VERSION) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::MOTOR_SERIAL_NUMBER)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusMotor::SERIAL_NO_PART1),
                                BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusMotor::SERIAL_NO_PART2));
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::PEAK_ASSIST_LEVELS)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusMotor::PEAK_ASSIST) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::ASSIST_PERCENTAGE)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusMotor::ASSIST_LEVEL) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::WHEEL_CIRCUMFERENCE)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusOther::WHEEL_CIRCUMFERENCE) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::FAKE_CHANNEL_NUMBER)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusOther::FAKE_CHANNEL) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BIKE_ON_OFF_STATE)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusOther::BIKE_ON_OFF) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BATTERY_CONNECTED_STATE)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusOther::BIKE_SERIAL_NO) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BEEP_ON_OFF_STATE)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusOther::BEEPER) );
}

BluetoothBikeRequest BikeStateToBluetoothBikeRequest::getBluetoothBikeRequest(BikeStateAttributeIndex bikeStateAttributeIndex, BikeType bikeType) {
    if (bikeStateAttributeIndex == BikeStateAttributeIndex::BATTERY_SERIAL_NUMBER) {
        switch (bikeType) {
            case BikeType::GEN1:
                return BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusBattery::SERIAL_NO_GEN1) );
            case BikeType::GEN2:
                return BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusBattery::SERIAL_NO_GEN2_PART1),
                                            BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusBattery::SERIAL_NO_GEN2_PART2) );
        }
    }
    return this->bluetoothBikeRequestMap[static_cast<int>(bikeStateAttributeIndex)];
}

void BikeState::setMonitorAttributeType(MonitorAttributeType monitorAttributeType) {
    for (int i = 0 ; i < static_cast<int>(BikeStateAttributeIndex::BIKE_STATE_ATTRIBUTE_SIZE) ; i++ ) { 
        bikeStateAttributes[i].monitorAttributeType = monitorAttributeType;
    }
}

void BikeState::setLastFetchTimeTicks(uint32_t lastFetchTimeTicks) {
    for (int i = 0; i < static_cast<int>(BikeStateAttributeIndex::BIKE_STATE_ATTRIBUTE_SIZE); i++) {
        bikeStateAttributes[i].lastFetchTimeTicks = lastFetchTimeTicks;
    }
}
