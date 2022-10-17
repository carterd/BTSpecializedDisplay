
#include "BluetoothBike.h"
#include "BluetoothBikeController.h"

#include <ArduinoBLE.h>
#include <Arduino.h>

/**
 * @brief This is an instance of a mapping table between attribute indexes and the StatusArea, StatusParameter tuple.
 */
BikeStateToBluetoothBikeRequest BluetoothBike::bikeStateToBluetoothBikeRequest;

/**
 * @brief Construct a new Bluetooth Bike Bluetooth Bike object
 * 
 * @param bleDevice the BLE device which is the bile
 */
BluetoothBike::BluetoothBike() {    
    this->bikeType = BikeType::NONE;
    this->bikeStatusLastUpdateTime = 0;
}

void BluetoothBike::setBleDevice(BLEDevice& bleDevice) {
	if (this->bleDevice && this->isConnected())
		this->disconnect();
	if (this->bleDevice != bleDevice) {
		const static BikeStateAttribute::BikeStateAttributeValue initialBikeStateAttributeValue = { 
  		.numberString = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	            				  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };

   	// Reset all the bike stats monitoring and timings
   	this->resetBikeStateMonitorAttributeType();
   	this->resetBikeStateLastFetchTime();
  	this->resetBikeStateAttributeValue(initialBikeStateAttributeValue);
	}
	this->bleDevice = bleDevice;
}

/**
 * @brief Connects to the bike and updates the characterisitcs for the bike
 * 
 * @return true The bluetooth bike object is connected to the external bike
 * @return false Connection has failed this object is not connected to the external bike
 */
bool BluetoothBike::connect() {
  if (this->bleDevice.connect()) {
    if (this->bleDevice.discoverAttributes()) {
      this->bikeType = BikeType::NONE;
      if (this->bleDevice.hasService(UUID_SPECIALIZED_READ_SERVICE_GEN1)) {
        this->bikeType = BikeType::GEN1;
        this->ebsReadKeyBleCha = this->bleDevice.characteristic(UUID_SPECIALIZED_READ_KEY_CHARACTERISTIC_GEN1);
        this->ebsReadValueBleCha = this->bleDevice.characteristic(UUID_SPECIALIZED_READ_VALUE_CHARACTERISTIC_GEN1);
        this->ebsWriteKeyValueBleCha = this->bleDevice.characteristic(UUID_SPECIALIZED_WRITE_KEY_VALUE_CHARACTERISTIC_GEN1);
        this->ebsNotifyKeyValueBleCha = this->bleDevice.characteristic(UUID_SPECIALIZED_NOTIFY_KEY_VALUE_CHARACTERISTIC_GEN1);
      }
      if (this->bleDevice.hasService(UUID_SPECIALIZED_READ_SERVICE_GEN2)) {
        this->bikeType = BikeType::GEN2;
        this->ebsReadKeyBleCha = this->bleDevice.characteristic(UUID_SPECIALIZED_READ_KEY_CHARACTERISTIC_GEN2);
        this->ebsReadValueBleCha = this->bleDevice.characteristic(UUID_SPECIALIZED_READ_VALUE_CHARACTERISTIC_GEN2);
        this->ebsWriteKeyValueBleCha = this->bleDevice.characteristic(UUID_SPECIALIZED_WRITE_KEY_VALUE_CHARACTERISTIC_GEN2);
        this->ebsNotifyKeyValueBleCha = this->bleDevice.characteristic(UUID_SPECIALIZED_NOTIFY_KEY_VALUE_CHARACTERISTIC_GEN2);
      }
      if (this->bleDevice.hasService(UUID_CYCLE_SERVICE)) {
        this->cscMeasurementBleCha = this->bleDevice.characteristic(UUID_CSC_MEASUREMENT_CHARACTERISTIC);
        this->cscFeatureBleCha = this->bleDevice.characteristic(UUID_CSC_FEATURE_CHARACTERISTIC);
        this->scControlPointBleCha = this->bleDevice.characteristic(UUID_SC_CONTROL_POINT_CHARACTERISTIC);
      }
      this->bikeStatusLastUpdateTime = 0;
      // Bike is connected
      if (this->subscribeEbsNotifications() && this->subscribeCscNotifications()) {
        
        return true;
      }
    }
    this->bleDevice.disconnect();
  }
  // Unable to make connection
  return false;
}

bool BluetoothBike::disconnect() {
  if (this->isConnected()) {
    return this->bleDevice.disconnect();
  }
  return true;
}

bool BluetoothBike::subscribeEbsNotifications() {
  //  notifications use static handlers on the controller as the controller can identify which BluetoothBike to forward to
  if (this->ebsNotifyKeyValueBleCha.canSubscribe() && this->ebsNotifyKeyValueBleCha.subscribe()) {
    this->ebsNotifyKeyValueBleCha.setEventHandler(BLEUpdated, BluetoothBikeController::update_ebs_handler_cb);
    return true;
  }
  return false;
}

bool BluetoothBike::subscribeCscNotifications() {
  // Only supported on GEN1
  if (this->bikeType != BikeType::GEN1) {
    return true;
  }
  //  notifications use static handlers on the controller as the controller can identify which BluetoothBike to forward to
  if (this->cscMeasurementBleCha.canSubscribe() && this->cscMeasurementBleCha.subscribe()) {
    this->cscMeasurementBleCha.setEventHandler(BLEUpdated, BluetoothBikeController::update_csc_handler_cb);
    return true;
  }
  return false;
}

void BluetoothBike::poll() {
  if (this->isConnected())
    this->bleDevice.poll();
}

bool BluetoothBike::isConnected() {
  return this->bleDevice.connected();
}

//
// Processing of Reading Writing and Notifications
//

/**
 * @brief Specifically Reads the ebs value by sending a request to ebsReadKeyBleCha and reading the response
 * from ebsReadValueBleCha.
 * 
 * @param ebikeStatusArea The status area such as MOTOR, BATTERY or OTHER
 * @param ebikeStatusParameter The specific parameter in the status area to read such as CAPACITY
 * @return true The attribute has been successfully processed and if enable the status has been updated with the read.
 * @return false An issue occured during the read operation and the status attribute will not have been updated.
 */
bool BluetoothBike::readEbsBikeValue(int ebikeStatusArea, int ebikeStatusParameter) {
  // int32 requires the bytes in this order (due to endian conversion)
  uint16_t key = (ebikeStatusParameter << 8) + ebikeStatusArea;
  if (this->ebsReadKeyBleCha.writeValue(key)) {
    // Ensure we've at least read the key back
    if (this->ebsReadValueBleCha.readValue(this->readBuffer, 20) > 1) {
      if ( (this->readBuffer[0] == ebikeStatusArea) && (this->readBuffer[1] == ebikeStatusParameter)) {
        // Now process the read buffer
        this->readBufferToEbsBikeState();
        return true;
      } else {
        LV_LOG_ERROR("valueKey mismatch FAILURE");
      }
    } else {
      LV_LOG_ERROR("ebsReadValueBleCha.readValue FAILURE");
    }
  } else {
    LV_LOG_ERROR("ebsReadKeyBleChar.writeValue FAILURE");
  }
  return false;
}

/**
 * @brief This processes the read buffer to bike state parameter
 * 
 */
void BluetoothBike::readBufferToEbsBikeState() {  
  bool bikeStatusUpdate = false;
  uint32_t time = millis();
  EbikeStatusArea ebikeStatusArea = static_cast<EbikeStatusArea>(this->readBuffer[0]);
  switch (ebikeStatusArea) {
    case EbikeStatusArea::BATTERY: {
      EbikeStatusBattery ebikeStatusBattery = static_cast<EbikeStatusBattery>(this->readBuffer[1]);
      switch (ebikeStatusBattery) {
        case EbikeStatusBattery::CAPACITY:         bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_CAPACITY, this->bufferToUint16(), time); break;
        case EbikeStatusBattery::CAPACITY_REMAIN:  bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_CAPACITY_REMAINING, this->bufferToUint16(), time); break;
        case EbikeStatusBattery::HEALTH:           bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_HEALTH, this->bufferToUint8(), time); break;
        case EbikeStatusBattery::TEMP:             bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_TEMP, this->bufferToUint8(), time); break;
        case EbikeStatusBattery::CHARGE_CYCLES:    bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_CHARGE_CYCLES, this->bufferToUint16(), time); break;
        case EbikeStatusBattery::VOLTAGE:          bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_VOLTAGE, this->bufferToUint8() / 5.0f + 20, time); break;
        case EbikeStatusBattery::CURRENT:          bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_CURRENT, this->bufferToUint8() / 5.0f, time); break;
        case EbikeStatusBattery::HARDWARE_VERSION: {            
            // Possible code with GEN2
            HardwareVersion hardwareVersion { this->bufferToUint8(), 0 };
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_HARDWARE_VERSION, hardwareVersion, time);
          } break;
        case EbikeStatusBattery::FIRMWARE_VERSION: {
            FirmwareVersion firmwareVersion { this->bufferToUint8(BATTERY_FIRMWARE_VERSION_MAJOR_NUMBER_BUFFER_INDEX), 
                                              this->bufferToUint8(BATTERY_FIRMWARE_VERSION_MINOR_NUMBER_BUFFER_INDEX),
                                              this->bufferToUint8(BATTERY_FIRMWARE_VERSION_PATCH_NUMBER_BUFFER_INDEX) };
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_FIRMWARE_VERSION, firmwareVersion, time);
          } break;
        case EbikeStatusBattery::SERIAL_NO_GEN1:  bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_SERIAL_NUMBER, this->bufferToUint16(), time); break;
        case EbikeStatusBattery::CHARGE_PERCENT:  bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_CHARGE_PERCENT, this->bufferToUint8(), time); break;
        case EbikeStatusBattery::SERIAL_NO_GEN2_PART1:
        case EbikeStatusBattery::SERIAL_NO_GEN2_PART2:
        // GEN 2 TODO
          break;
      }
    }
    break;
    case EbikeStatusArea::MOTOR: {
      EbikeStatusMotor ebikeStatusMotor = static_cast<EbikeStatusMotor>(this->readBuffer[1]);
      switch (ebikeStatusMotor) {
        case EbikeStatusMotor::RIDER_POWER:   bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::RIDER_POWER, this->bufferToUint16(), time); break;
        case EbikeStatusMotor::CADENCE:       bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_CADENCE, this->bufferToUint16() / 10.0f, time); break;
        case EbikeStatusMotor::SPEED:         bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_SPEED, this->bufferToUint16() / 10.0f, time); break;
        case EbikeStatusMotor::ODOMETER:      bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_ODOMETER, this->bufferToUint32(), time); break;
        case EbikeStatusMotor::ASSIST_LEVEL:  bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_ASSIST_LEVEL, this->bufferToUint16(), time); break;
        case EbikeStatusMotor::TEMP:          bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_TEMP, this->bufferToUint8(), time); break;
        case EbikeStatusMotor::MOTOR_POWER:   bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_FIRMWARE_VERSION, this->bufferToUint16(), time); break;
        case EbikeStatusMotor::FIRMWARE_VERSION: {
          FirmwareVersion firmwareVersion { this->bufferToUint8(MOTOR_FIRMWARE_VERSION_MAJOR_NUMBER_BUFFER_INDEX), 
                                            this->bufferToUint8(MOTOR_FIRMWARE_VERSION_MINOR_NUMBER_BUFFER_INDEX), 
                                            this->bufferToUint8(MOTOR_FIRMWARE_VERSION_PATCH_NUMBER_BUFFER_INDEX) }; 
          bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_FIRMWARE_VERSION, firmwareVersion, time);
        } break;
        case EbikeStatusMotor::PEAK_ASSIST:
        {
            AssistLevels assistLevels = this->bikeState.getStateAttribute(BikeStateAttributeIndex::PEAK_ASSIST_LEVELS).bikeStateAttributeValue.assistLevels;
            switch (this->bikeType) {
            case BikeType::GEN1: {                
                uint8_t peakTypeAndValue = this->bufferToUint8();
                uint8_t peakType = peakTypeAndValue % 10;
                uint8_t peakLevel = peakTypeAndValue - peakType;
                switch (peakType) {
                case 1: assistLevels.eco = peakLevel; break;
                case 2: assistLevels.trail = peakLevel; break;
                case 3: assistLevels.turbo = peakLevel; break;
                }
            } break;
            case BikeType::GEN2: {
                AssistLevels assistLevels = this->bikeState.getStateAttribute(BikeStateAttributeIndex::PEAK_ASSIST_LEVELS).bikeStateAttributeValue.assistLevels;
                assistLevels.eco = this->bufferToUint8(2);
                assistLevels.trail = this->bufferToUint8(3);
                assistLevels.turbo = this->bufferToUint8(4);
                bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::PEAK_ASSIST_LEVELS, assistLevels, time);
            } break;
            } break;
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::PEAK_ASSIST_LEVELS, assistLevels, time);
        } break;
        case EbikeStatusMotor::HARDWARE_NO_PART1: {
            char* motorHardwareNumber = this->bikeState.getStateAttribute(BikeStateAttributeIndex::MOTOR_HARDWARE_VERSION).bikeStateAttributeValue.numberString.value;
            bufferToCharArray(&(motorHardwareNumber[0]), 18);
            motorHardwareNumber[20] = 0x00;
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_HARDWARE_VERSION, time);
          } break;
        case EbikeStatusMotor::HARDWARE_NO_PART2: {
            char* motorHardwareNumber = this->bikeState.getStateAttribute(BikeStateAttributeIndex::MOTOR_HARDWARE_VERSION).bikeStateAttributeValue.numberString.value;
            bufferToCharArray(&(motorHardwareNumber[18]), 2);
            motorHardwareNumber[20] = 0x00;
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_HARDWARE_VERSION, time);
          } break;
        case EbikeStatusMotor::SERIAL_NO_PART1: {
            char* motorSerailNumber = this->bikeState.getStateAttribute(BikeStateAttributeIndex::MOTOR_SERIAL_NUMBER).bikeStateAttributeValue.numberString.value;
            bufferToCharArray(&(motorSerailNumber[0]), 18);
            motorSerailNumber[23] = 0x00;
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_SERIAL_NUMBER, time);
          } break;
        case EbikeStatusMotor::SERIAL_NO_PART2: {
            char* motorSerailNumber = this->bikeState.getStateAttribute(BikeStateAttributeIndex::MOTOR_SERIAL_NUMBER).bikeStateAttributeValue.numberString.value;
            bufferToCharArray(&(motorSerailNumber[18]), 5);
            motorSerailNumber[23] = 0x00;
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_SERIAL_NUMBER, time);
          } break;
      }
    } break;
    case EbikeStatusArea::OTHER: {
      EbikeStatusOther ebikeStatusOther = static_cast<EbikeStatusOther>(this->readBuffer[1]);
      switch (ebikeStatusOther) {
        case EbikeStatusOther::WHEEL_CIRCUMFERENCE: bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::WHEEL_CIRCUMFERENCE, this->bufferToUint16(), time); break;
        case EbikeStatusOther::ASSIST_ECO: {
          this->bikeState.getStateAttribute(BikeStateAttributeIndex::ASSIST_PERCENTAGE).bikeStateAttributeValue.assistLevels.eco = this->bufferToUint8();
          bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::ASSIST_PERCENTAGE, time);
        } 
        break;
        case EbikeStatusOther::ASSIST_TRAIL: {
            this->bikeState.getStateAttribute(BikeStateAttributeIndex::ASSIST_PERCENTAGE).bikeStateAttributeValue.assistLevels.trail = this->bufferToUint8();
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::ASSIST_PERCENTAGE, time);
        }
        break;
        case EbikeStatusOther::ASSIST_TURBO: {
            this->bikeState.getStateAttribute(BikeStateAttributeIndex::ASSIST_PERCENTAGE).bikeStateAttributeValue.assistLevels.turbo = this->bufferToUint8();
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::ASSIST_PERCENTAGE, time);
        }
        break;
        case EbikeStatusOther::BIKE_ON_OFF:         bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BIKE_ON_OFF_STATE, this->bufferToUint8(), time); break;
        case EbikeStatusOther::BIKE_SERIAL_NO:
          {
            char* bikeSerailNumber = this->bikeState.getStateAttribute(BikeStateAttributeIndex::BIKE_SERIAL_NUMBER).bikeStateAttributeValue.numberString.value;
            bufferToCharArray(&(bikeSerailNumber[0]), 17);
            bikeSerailNumber[17] = 0x00;
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BIKE_SERIAL_NUMBER, time);
            bikeStatusUpdate |= this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_CONNECTED_STATE, (bikeSerailNumber[0] == 0xff && bikeSerailNumber[1] == 0xff && bikeSerailNumber[2] == 0xff && bikeSerailNumber[3]) ? false : true, time); break;
          }
          break;
      }
    } break;
  }

  // If we identify a bike status attribute has been changed then update the time stamp in bikeStatuslastUpdateTime
  if (bikeStatusUpdate) {    
    this->bikeStatusLastUpdateTime = time;
    LV_LOG_USER("MOTOR ASSIST LEVEL read %d", this->bikeStatusLastUpdateTime);
  }
}


void BluetoothBike::readBufferToCscMeasurement()
{
  bool bikeStatusUpdate = false;
  uint32_t time = millis();

  int readBufferIndex = 1;
  if (this->readBuffer[0] & static_cast<int>(CscMeasurementBit::CSC_WHEEL_REV_BIT)) {
    SpeedCadenceReading wheelSpeedCadenceReading = { 
        this->bufferToUint32(readBufferIndex),
        this->bufferToUint16(readBufferIndex+4)
      };
    readBufferIndex += 6;
    SpeedCadenceReading lastWheelSpeedCadenceReading = { 
      this->bikeState.getStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS).bikeStateAttributeValue.cscReading.rotationCount,
      this->bikeState.getStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS).bikeStateAttributeValue.cscReading.eventTime
    };
    bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS, wheelSpeedCadenceReading, time);

    if ((wheelSpeedCadenceReading.eventTime != 0 || wheelSpeedCadenceReading.rotationCount != 0) &&
        lastWheelSpeedCadenceReading.eventTime != wheelSpeedCadenceReading.eventTime &&
        lastWheelSpeedCadenceReading.rotationCount <= wheelSpeedCadenceReading.rotationCount) {
      // finally calculate the speed
      int32_t eventTimeDelta = lastWheelSpeedCadenceReading.eventTime <= wheelSpeedCadenceReading.eventTime ? 
        wheelSpeedCadenceReading.eventTime - lastWheelSpeedCadenceReading.eventTime : 
        0x10000 + wheelSpeedCadenceReading.eventTime - lastWheelSpeedCadenceReading.eventTime;
      int32_t wheelRotationsDelta = wheelSpeedCadenceReading.rotationCount - lastWheelSpeedCadenceReading.rotationCount;
      float wheelRatationsPerSecond = 60000.0 * wheelRotationsDelta / eventTimeDelta;
      bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN, wheelRatationsPerSecond, time);
    }
  }
  if (this->readBuffer[0] & static_cast<int>(CscMeasurementBit::CSC_CRANK_REV_BIT)) {
    SpeedCadenceReading crankSpeedCadenceReading = { 
        this->bufferToUint16(readBufferIndex),
        this->bufferToUint16(readBufferIndex+2)
      };
    readBufferIndex += 4;
    SpeedCadenceReading lastCrankSpeedCadenceReading = { 
      this->bikeState.getStateAttribute(BikeStateAttributeIndex::CRANK_ROTATIONS).bikeStateAttributeValue.cscReading.rotationCount,
      this->bikeState.getStateAttribute(BikeStateAttributeIndex::CRANK_ROTATIONS).bikeStateAttributeValue.cscReading.eventTime
    };
    bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::CRANK_ROTATIONS, crankSpeedCadenceReading, time);

    LV_LOG_USER("CADENCE = %d %d ", lastCrankSpeedCadenceReading.rotationCount, crankSpeedCadenceReading.rotationCount);
    LV_LOG_USER("TIMES = %d %d ", lastCrankSpeedCadenceReading.eventTime, crankSpeedCadenceReading.eventTime);

    if ((crankSpeedCadenceReading.eventTime != 0 || crankSpeedCadenceReading.rotationCount != 0) &&
      lastCrankSpeedCadenceReading.eventTime != crankSpeedCadenceReading.eventTime &&
      lastCrankSpeedCadenceReading.rotationCount <= crankSpeedCadenceReading.rotationCount) {
      // finally calculate the speed
      int32_t eventTimeDelta = lastCrankSpeedCadenceReading.eventTime <= crankSpeedCadenceReading.eventTime ? 
        crankSpeedCadenceReading.eventTime - lastCrankSpeedCadenceReading.eventTime : 
        0x10000 + crankSpeedCadenceReading.eventTime - lastCrankSpeedCadenceReading.eventTime;
      int32_t crankRotationsDelta = crankSpeedCadenceReading.rotationCount - lastCrankSpeedCadenceReading.rotationCount;
      float crankRatationsPerSecond = 60000.0 * crankRotationsDelta / eventTimeDelta;
      bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::CRANK_ROTATIONS_PER_MIN, crankRatationsPerSecond, time);

      LV_LOG_USER("sums are 60000 * %ld / %ld -> %d", crankRotationsDelta, eventTimeDelta, (int) (crankRatationsPerSecond));
    }
  }
  // If we identify a bike status attribute has been changed then update the time stamp in bikeStatuslastUpdateTime
  if (bikeStatusUpdate) {
    this->bikeStatusLastUpdateTime = time;
  }
}


bool BluetoothBike::readRequest(BluetoothBikeRequest bluetoothBikeRequest) {
  bool result = this->readRequestCommand(bluetoothBikeRequest.request1);
  if (result) result = this->readRequestCommand(bluetoothBikeRequest.request2);

  return result;
}

bool BluetoothBike::readRequestCommand(BluetoothBikeRequest::BluetoothBikeRequestCommand bluetoothBikeRequestCommand) {
  if (bluetoothBikeRequestCommand.isValid()) {
    return this->readEbsBikeValue(bluetoothBikeRequestCommand.area, bluetoothBikeRequestCommand.attribute);
  }
  // return true if requesting to read request on something which isn't mapped to a valid request
  return true;
}

bool BluetoothBike::readBikeStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, MonitorAttributeType monitorAttributeType) {
    bool result;
    this->setMinimumBikeStateMonitorAttributeType(bikeStateAttributeIndex, monitorAttributeType);
    BluetoothBikeRequest bluetoothBikeRequest = BluetoothBike::bikeStateToBluetoothBikeRequest.getBluetoothBikeRequest(bikeStateAttributeIndex, this->bikeType);
    result = this->readRequest(bluetoothBikeRequest);
    return result;
}

//
// Class callback functions, called by controller
//

void BluetoothBike::updateEbsCharacteristicCB(BLEDevice device, BLECharacteristic characteristic)  {
  if (device == this->bleDevice) {    
    if (characteristic.readValue(this->readBuffer, 20) > 1) {
        this->readBufferToEbsBikeState();
    }
  }
}


void BluetoothBike::updateCscCharacteristicCB(BLEDevice device, BLECharacteristic characteristic) {
  if (device == this->bleDevice) {
    if (characteristic.readValue(this->readBuffer, 20) > 1) {
        this->readBufferToCscMeasurement();
    }
  }
}