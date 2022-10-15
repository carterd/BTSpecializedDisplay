
#include "BluetoothBike.h"

#include <ArduinoBLE.h>
#include <Arduino.h>

/**
 * @brief This is an instance of a mapping table between attribute indexes and the StatusArea, StatusParameter tuple.
 */
BikeStateToBluetoothBikeRequest BluetoothBike::bikeStateToBluetoothBikeRequest;


BluetoothBike::BluetoothBike() {
    this->bikeStatuslastUpdateTime = 0;
}

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
  //LV_LOG_USER("Read = [%x, %x] [%x, %x, %x, %x]", this->readBuffer[0],this->readBuffer[1], this->readBuffer[2],this->readBuffer[3], this->readBuffer[4],this->readBuffer[5]);
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
  if (bikeStatusUpdate) this->bikeStatuslastUpdateTime = time;
}
