#include "BluetoothBikeRequest.h"

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
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::PEAK_POWER_ASSIST_LEVELS)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusMotor::PEAK_POWER_ASSIST) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::SUPPORT_ASSIST_LEVELS)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusOther::SUPPORT_ASSIST_ECO),
                                BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusOther::SUPPORT_ASSIST_TRAIL),
                                BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusOther::SUPPORT_ASSIST_TURBO) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::WHEEL_CIRCUMFERENCE)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusOther::WHEEL_CIRCUMFERENCE) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::FAKE_CHANNEL_NUMBER)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusOther::FAKE_CHANNEL) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BIKE_ON_OFF_STATE)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusOther::BIKE_ON_OFF) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BATTERY_CONNECTED_STATE)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusOther::BIKE_SERIAL_NO) );
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BIKE_SERIAL_NUMBER)]
        = BluetoothBikeRequest(BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusOther::BIKE_SERIAL_NO));
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BEEP_ON_OFF_STATE)]
        = BluetoothBikeRequest( BluetoothBikeRequest::BluetoothBikeRequestCommand(EbikeStatusOther::BEEPER) );
    // Csc Measurements are notifications only so no mapping for reading these parameters
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::WHEEL_ROTATIONS)] = BluetoothBikeRequest();
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN)] = BluetoothBikeRequest();
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::CRANK_ROTATIONS)] = BluetoothBikeRequest();
    this->bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::CRANK_ROTATIONS_PER_MIN)] = BluetoothBikeRequest();
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
