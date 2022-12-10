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

BikeStateAttributeIndex BluetoothBikeRequestToBikeState::notMappedAttributeIndex = BikeStateAttributeIndex::BIKE_STATE_ATTRIBUTE_SIZE;

BluetoothBikeRequestToBikeState::BluetoothBikeRequestToBikeState()
{
    this->batteryBikeStateAttributeIndexMap[EbikeStatusBattery::CAPACITY] = BikeStateAttributeIndex::BATTERY_CAPACITY;
    this->batteryBikeStateAttributeIndexMap[EbikeStatusBattery::CAPACITY_REMAIN] = BikeStateAttributeIndex::BATTERY_CAPACITY_REMAINING;
    this->batteryBikeStateAttributeIndexMap[EbikeStatusBattery::HEALTH] = BikeStateAttributeIndex::BATTERY_HEALTH;
    this->batteryBikeStateAttributeIndexMap[EbikeStatusBattery::TEMP] = BikeStateAttributeIndex::BATTERY_TEMP;
    this->batteryBikeStateAttributeIndexMap[EbikeStatusBattery::CHARGE_CYCLES] = BikeStateAttributeIndex::BATTERY_CHARGE_CYCLES;
    this->batteryBikeStateAttributeIndexMap[EbikeStatusBattery::VOLTAGE] = BikeStateAttributeIndex::BATTERY_VOLTAGE;
    this->batteryBikeStateAttributeIndexMap[EbikeStatusBattery::CURRENT] = BikeStateAttributeIndex::BATTERY_CURRENT;
    this->batteryBikeStateAttributeIndexMap[EbikeStatusBattery::HARDWARE_VERSION] = BikeStateAttributeIndex::BATTERY_HARDWARE_VERSION;
    this->batteryBikeStateAttributeIndexMap[EbikeStatusBattery::FIRMWARE_VERSION] = BikeStateAttributeIndex::BATTERY_FIRMWARE_VERSION;
    this->batteryBikeStateAttributeIndexMap[EbikeStatusBattery::CHARGE_PERCENT] = BikeStateAttributeIndex::BATTERY_CHARGE_PERCENT;

    this->motorBikeStateAttributeIndexMap[EbikeStatusMotor::RIDER_POWER] = BikeStateAttributeIndex::RIDER_POWER;
    this->motorBikeStateAttributeIndexMap[EbikeStatusMotor::CADENCE] = BikeStateAttributeIndex::MOTOR_CADENCE;
    this->motorBikeStateAttributeIndexMap[EbikeStatusMotor::SPEED] = BikeStateAttributeIndex::MOTOR_SPEED;
    this->motorBikeStateAttributeIndexMap[EbikeStatusMotor::ODOMETER] = BikeStateAttributeIndex::MOTOR_ODOMETER;
    this->motorBikeStateAttributeIndexMap[EbikeStatusMotor::ASSIST_LEVEL] = BikeStateAttributeIndex::MOTOR_ASSIST_LEVEL;
    this->motorBikeStateAttributeIndexMap[EbikeStatusMotor::TEMP] = BikeStateAttributeIndex::MOTOR_TEMP;
    this->motorBikeStateAttributeIndexMap[EbikeStatusMotor::FIRMWARE_VERSION] = BikeStateAttributeIndex::MOTOR_FIRMWARE_VERSION;
    this->motorBikeStateAttributeIndexMap[EbikeStatusMotor::SHUTTLE] = BikeStateAttributeIndex::MOTOR_SPEED;

    this->otherBikeStateAttributeIndexMap[EbikeStatusOther::WHEEL_CIRCUMFERENCE] = BikeStateAttributeIndex::WHEEL_CIRCUMFERENCE;
    this->otherBikeStateAttributeIndexMap[EbikeStatusOther::FAKE_CHANNEL] = BikeStateAttributeIndex::FAKE_CHANNEL_NUMBER;
    this->otherBikeStateAttributeIndexMap[EbikeStatusOther::BEEPER] = BikeStateAttributeIndex::BEEP_ON_OFF_STATE;
    this->otherBikeStateAttributeIndexMap[EbikeStatusOther::BIKE_ON_OFF] = BikeStateAttributeIndex::BIKE_ON_OFF_STATE;
    this->otherBikeStateAttributeIndexMap[EbikeStatusOther::BIKE_SERIAL_NO] = BikeStateAttributeIndex::BIKE_SERIAL_NUMBER;
}

BikeStateAttributeIndex BluetoothBikeRequestToBikeState::getBikeStateAttributeIndex(EbikeStatusBattery ebikeStatusBattery) {
    std::unordered_map<EbikeStatusBattery, BikeStateAttributeIndex>::iterator batteryBikeStateAttributeIndexIter = this->batteryBikeStateAttributeIndexMap.find(ebikeStatusBattery);
    if (batteryBikeStateAttributeIndexIter == batteryBikeStateAttributeIndexMap.end()) {
        return BluetoothBikeRequestToBikeState::notMappedAttributeIndex;
    }
    else {
        return (*batteryBikeStateAttributeIndexIter).second;
    }
}

BikeStateAttributeIndex BluetoothBikeRequestToBikeState::getBikeStateAttributeIndex(EbikeStatusMotor ebikeStatusMotor) {
    std::unordered_map<EbikeStatusMotor, BikeStateAttributeIndex>::iterator motorBikeStateAttributeIndexIter = this->motorBikeStateAttributeIndexMap.find(ebikeStatusMotor);
    if (motorBikeStateAttributeIndexIter == motorBikeStateAttributeIndexMap.end()) {
        return BluetoothBikeRequestToBikeState::notMappedAttributeIndex;
    }
    else {
        return (*motorBikeStateAttributeIndexIter).second;
    }
}

BikeStateAttributeIndex BluetoothBikeRequestToBikeState::getBikeStateAttributeIndex(EbikeStatusOther ebikeStatusOther) {
    std::unordered_map<EbikeStatusOther, BikeStateAttributeIndex>::iterator otherBikeStateAttributeIndexIter = this->otherBikeStateAttributeIndexMap.find(ebikeStatusOther);
    if (otherBikeStateAttributeIndexIter == otherBikeStateAttributeIndexMap.end()) {
        return BluetoothBikeRequestToBikeState::notMappedAttributeIndex;
    }
    else {
        return (*otherBikeStateAttributeIndexIter).second;
    }
}