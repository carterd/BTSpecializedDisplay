#ifndef _BLUETOOTH_BIKE_DEFINITIONS_H
#define _BLUETOOTH_BIKE_DEFINITIONS_H


//
// This is all the definitions for Specialized Turbo bikes all in one place.
//
// I've attempted to cover as far as possible both GEN-1 and GEN-2 implementations
//

// A Simple enum to identify different bike types
enum class BikeType { NONE = 0, GEN1 = 1, GEN2 = 2 };



// It seems that GEN-1 Specialized Turbo bikes implment the Cycling Speed & Cadence Service (CSC Service):-
// The details for CSC Service are provided at "https://www.bluetooth.com/specifications/specs/cycling-speed-and-cadence-service-1-0/"

// This is the Cycle Service for GEN-1 Bikes, it's advertised as a 16-bit Service number
#define UUID_CYCLE_SERVICE                   "1816"

// The CSC Measurement Characteristic, this is a Notify only characteristic and reports the Speed and Cadence measures
//
// U-Int-8 (CSC Measurement Characteristic Flags)
//   bit 0 - TRUE indicates the wheel revolution data present
//   bit 1 - TRUE indicates crank revolution data present
enum class CscMeasurementBit { CSC_WHEEL_REV_BIT = 1, CSC_CRANK_REV_BIT = 2 };
// U-Int-32 (Cumulative Wheel Revolutions) [only present if bit 0 = TRUE of CSC Measurement Characteristic Flags]
//   This is a cumulative revolution count
// U-Int-16 (Last Wheel Event Time) [only present if bit 0 = TRUE of CSC Measurement Characteristic Flags]
//   This is a time stamp in 1/1024s, hence will roll over every 64 seconds
// U-Int-16 (Cumulative Crank Revolutions) [only present if bit 1 = TRUE of CSC Measurement Characteristic Flags]
//   This is a cumulative revolution count
// U-Int-16 (Last Crank Event Time) [only present if bit 1 = TRUE of CSC Measurement Characteristic Flags]
//   This is a time stamp in 1/1024s, hence will roll over every 64 seconds
#define UUID_CSC_MEASUREMENT_CHARACTERISTIC  "2a5b"

// The CSC Feature Characteristic, should always return 0x03
//
// U-Int-8 (CSC Feature Characteristic Flags)
//   bit 0 - TRUE indicates the wheel revolution data supported
//   bit 1 - TRUE indicates crank revolution data supported
//
// Hence 0x03 identifies that both wheel revolutions and crank data supported
//
#define UUID_CSC_FEATURE_CHARACTERISTIC      "2a5c"

// The SC Control Point Characteristic, write only to send commands to configure CSC
//
// U-Int-8 (SC Control Point OP-Code)
//   0x01 - Set Cumulative Value
//   0x02 - Start Sensor Calibration
//   0x03 - Update Sensor Location
//   0x04 - Request Supported Sensor Locations
// U-Int-32 [required if OP-Code is 0x01 Set Cumulative Value]
// 
#define UUID_SC_CONTROL_POINT_CHARACTERISTIC "2a55"

// Specialized Read Service
//
// The Specialized Turbo ebike implements a Read Service in which a Key is written to the service, 
// and the Value for that key can then be requested from the Read Service.
//
// The read service therefore has two characterisitics, one is a key-value which is write only. 
// The second characteristic is a read only characteristic which responds to the value held against
// the key.
//
#define UUID_SPECIALIZED_READ_SERVICE_GEN1   "00000001-0000-4b49-4e4f-525441474947"
#define UUID_SPECIALIZED_READ_SERVICE_GEN2   "00000001-3731-3032-494d-484f42525554"

// Specialized Read Key Write Characteristic
//
// This characteristic allows a key value to be written to the service whereby the key selects the
// value to be read by the 'Specialized Read Value Characteristic'
//
// U-Int-8 Ebike Status Area Key Identifier
//   0x00 - The Battery Specific status area values
//   0x01 - The Motor Specific status area values
//   0x02 - The Wheels and bike and other values
//   0x04 - A secondary Battery specific status area values
enum class EbikeStatusArea { BATTERY=0, MOTOR=1, OTHER=2, SECONDARY_BATTERY=4, INVALID=0xFF};
// U-Int-8 Ebike Status Area Specific Key Identifier 
//   [For Battery Status Area 0x00]
//     0x00 - Capacity
//     0x01 - Capacity Remaining
//     0x02 - Health
//     0x03 - Temp
//     0x04 - Charge Cycles
//     0x05 - Voltage
//     0x06 - Current
//     0x07 - Hardware Version
//     0x08 - Firmware Version
//     0x09 - Serial Number (GEN1 only)
//     0x0A - Unknown [u-int-32] zeros (see 0108)
//     0x0B - _NOT_USED_?
//     0x0C - Charge Percentage
//     0x0D - _NOT_USED_?
//     0x0E - _NOT_USED_?
//     0x0F - _NOT_USED_?
//     0x10 - Serial Number Part-1 (GEN2 only)
//     0x11 - Serial Number Part-2 (GEN2 only)
enum class EbikeStatusBattery { CAPACITY=0x00, CAPACITY_REMAIN=0x01, HEALTH=0x02, TEMP=0x03, CHARGE_CYCLES=0x04, VOLTAGE=0x05, CURRENT=0x06, HARDWARE_VERSION=0x07, FIRMWARE_VERSION=0x08, SERIAL_NO_GEN1=0x09, UNKNOWN_A=0x0A, CHARGE_PERCENT = 0x0C, SERIAL_NO_GEN2_PART1=0x10, SERIAL_NO_GEN2_PART2=0x11, BIKE_STATE_BATTERY_SIZE=0x12 };
//   [For Motor Status Area 0x01]
//     0x00 - Rider Power
//     0x01 - Motor Cadence
//     0x02 - Motor Speed
//     0x03 - _NOT_USED_?
//     0x04 - Odometer
//     0x05 - Motor Assist Level
//     0x06 - UNKNOWN [u-int-8] (always zero)
//     0x07 - Temperature
//     0x08 - UNKNOWN [u-int-32] zeros (see 000A)
//     0x09 - _NOT_USED_?
//     0x0A - Firmware Version
//     0x0B - _NOT_USED_?
//     0x0C - Motor Power
//     0x0D - _NOT_USED_?
//     0x0E - _NOT_USED_?
//     0x0F - _NOT_USED_?
//     0x10 - Peak Assit Settings
//     0x11 - Hardware Number Part-1
//     0x12 - Hardware Number Part-2
//     0x13 - Serail Number Part-1 
//     0x14 - Serail Number Part-2
//     0x15 - Shuttle???
enum class EbikeStatusMotor { RIDER_POWER=0x00, CADENCE=0x01, SPEED=0x02, ODOMETER=0x04, ASSIST_LEVEL=0x05, UNKNOWN_6=0x06, TEMP=0x07, UNKNOWN_8=0x08, FIRMWARE_VERSION=0x0A, MOTOR_POWER=0x0C, PEAK_POWER_ASSIST=0x10, HARDWARE_NO_PART1=0x11, HARDWARE_NO_PART2=0x12, SERIAL_NO_PART1=0x13, SERIAL_NO_PART2=0x14, SHUTTLE=0x15, BIKE_STATE_MOTOR_SIZE = 0x16 };
//   [For Bike Other Status Area 0x02]
//     0x00 - Wheel Circumference
//     0x01 - Unknown always 00 Some Kind of bike Version
//     0x02 - _NOT_USED_?
//     0x03 - Bike Assit Eco Percentage
//     0x04 - Bike Assit Trail Percentage
//     0x05 - Bike Assit Turbo Percentage
//     0x06 - Bike Fake Channel
//     0x07 - Unknown (not used)
//     0x08 - _NOT_USED_?
//     0x09 - Beeper
//     0x0A - Unknown always 02
//     0x0B - Bike On Off State
//     0x0C - _NOT_USED_?
//     0x0D - _NOT_USED_?
//     0x0E - _NOT_USED_?
//     0x0F - _NOT_USED_?
//     0x10 - Bike Serial
//     0x11 - Unknown always (00 00 00)
enum class EbikeStatusOther { WHEEL_CIRCUMFERENCE=0x00, UNKNOWN_1=0x01, UNKNOWN_2=0x02, SUPPORT_ASSIST_ECO=0x03, SUPPORT_ASSIST_TRAIL=0x04, SUPPORT_ASSIST_TURBO=0x05, FAKE_CHANNEL=0x06, UNKNOWN_7=0x07, UNKONWN_8=0x08, BEEPER=0x09, UNKONWN_A =0x0A, BIKE_ON_OFF=0x0B, BIKE_SERIAL_NO=0x10, UNKNOWN_11=0x11, BIKE_STATE_OTHER_SIZE = 0x12 };
#define UUID_SPECIALIZED_READ_KEY_CHARACTERISTIC_GEN1    "00000021-0000-4b49-4e4f-525441474947"
#define UUID_SPECIALIZED_READ_KEY_CHARACTERISTIC_GEN2    "00000021-3731-3032-494d-484f42525554"

// Specialized Read Value Characterisitic
//
// This is the value read for the corresponding Read Key written to the Read Key Write Characteristic above.
// The returned value is prefaced with the two U-Int-8 values specifying the key assocaited with the value.
//
// U-Int-8 Ebike Status Area Key Identifier
//   (see Specialized Read Key Write Characteristic for specifics)
// U-Int-8 Ebike Status Area Specific Key Identifier 
//   (see Specialized Read Key Write Characteristic for specifics)
// Variable-Length Ebike Status Value
//   [0x00, 0x00] U-Int-16 Battery Capacity Watt Hours
//   [0x00, 0x01] U-Int-16 Battery Charge Capacity Remaining Watt Hours
//   [0x00, 0x02] U-Int-8 Battery Health Percentage
//   [0x00, 0x03] Int-8 Battery Temperature in Centigrade (Not sure if this could be signed for negative temps?)
//   [0x00, 0x04] U-Int-16 Battery Charge Cycles
//   [0x00, 0x05] U-Int-8 Battery Voltage in 0.2 of a Volt above 20 Volts (eg. 0x00 = 20V, 0x67 = 40.6V)
//   [0x00, 0x06] U-Int-8 Battery Current in 0.2 of an Amp (eg. 0x00 = 0A, 0x0A = 2A)
//   GEN1 [0x00, 0x07] U-Int-8 Battery Hardware version major number
//   GEN2 [0x00, 0x07] U-Int-8 Battery Hardware version major number, U-Int-8 Hardware minor number (CHECK ORDER HERE)
//   [0x00, 0x08] U-Int-8 Battery Firmware version minor number hex, U-Int-8 Firmware major number hex, U-Int-8 Firware patch number hex
//   GEN1 [0x00, 0x09] U-Int-16 Battery Serial Number
//   [0x00, 0x0A] U-Int-32 Unknown
//   [0x00, 0x0C] U-Int-8 Battery Remaining Charge percentage
//   GEN2 [0x00, 0x10] Char[18] Battery Serial Number Part 1 (characters 0-17)
//   GEN2 [0x00, 0x11] Char[2] Battery Serial Number Part 2 (characters 18-19) (CHECK SIZE HERE)
//   [0x01, 0x00] U-Int-16 Rider Power in Watts
//   [0x01, 0x01] U-Int-16 Motor Cadence in 0.1 revolutions per (CHECK TIME)
//   [0x01, 0x02] U-Int-16 Motor Speed in 0.1 (CHECK SPEED)
//   [0x01, 0x04] U-Int-32 Motor Odometer in Meters
//   [0x01, 0x05] U-Int-16 Motor Assist Level ( 0x0001 eco, 0x0002 trail, 0x0003 turbo)
//   [0x01, 0x06] U-Int-8 UNKNOWN!!
//   [0x01, 0x07] Int-8 Motor Temperature
//   [0x01, 0x08] U-Int-32 UNKNOWN!!
//   [0x01, 0x0A] U-Int-8 Motor Firmware version major number hex, U-Int-8 Firmware minor number hex, U-Int-8 Firware patch number hex
//   [0x01, 0x0C] U-Int-16 Motor Power in Watts?
//   GEN1 [0x01, 0x10] U-Int-8 Peak Assit Eco/Trail/Trubo, value changes convert to decimal, tens digit is assit level 0-10, unit digit is; 1 for Eco, 2 for Trial, 3 for Trubo
//   GEN2 [0x01, 0x10] U-Int-8 Peak Assit Eco, U-Int-8 Peak Assit Trail, U-Int-8 Peak Assit Turbo
//   [0x01, 0x11] Char[18] Motor Hardware Number Part-1
//   [0x01, 0x12] Char[2] Motor Hardware Number Part-2
//   [0x01, 0x13] Char[18] Motor Serail Number Part-1 
//   [0x01, 0x14] Char[5] Motor Serail Number Part-2
//   [0x01, 0x15] U-Int-8 Shuttle???
//   [0x02, 0x00] U-Int-16 Wheel Circumference in mm
//   [0x02, 0x01] U-Int-8 (00?)
//   [0x02, 0x03] U-Int-8 Assit Eco Percentage
//   [0x02, 0x04] U-Int-8 Assit Trail Percentage
//   [0x02, 0x05] U-Int-8 Assit Turbo Percentage
//   [0x02, 0x06] U-Int-8 Fake Channel No
//   [0x02, 0x07] U-Int ???
//   [0x02, 0x09] U-Int-8 Beeper, 0x01 = ON, 0x00 = OFF
//   [0x02, 0x0A] U-Int-8 ??? Always 0x02
//   [0x02, 0x0B] U-Int-8 Bike On/Off Status, 0x01 = ON, 0x00 = OFF
//   [0x02, 0x10] Char[17] Bike Serial Number (Unused chars are set to 0x00)
//   [0x02, 0x11] U-Int-24 ??? zeros
#define UUID_SPECIALIZED_READ_VALUE_CHARACTERISTIC_GEN1  "00000011-0000-4b49-4e4f-525441474947"
#define UUID_SPECIALIZED_READ_VALUE_CHARACTERISTIC_GEN2  "00000011-3731-3032-494d-484f42525554"

// Specialized Write Service
//
// The write service allows parameters of the e-bike to be configured by writing values to the write characteristic.
//
#define UUID_SPECIALIZED_WRITE_SERVICE_GEN1  "00000002-0000-4b49-4e4f-525441474947"
#define UUID_SPECIALIZED_WRITE_SERVICE_GEN2  "00000002-3731-3032-494d-484f42525554"

// Specialized Write Characteristic
//
// The write characteristic service uses values in the key value format of that of the 'Specialized Read Value Characterisitic'.
//
// U-Int-8 Ebike Status Area Key Identifier
//   (see Specialized Read Key Write Characteristic for specifics)
// U-Int-8 Ebike Status Area Specific Key Identifier 
//   (see Specialized Read Key Write Characteristic for specifics)
// Variable-Length Ebike Status Value
//   (see Specialized Read Value Characterisitic for specifics)
//   GEN1 [0x01, 0x10] U-Int-8 Peak Assit Eco/Trail/Trubo, value changes convert to decimal, tens digit is assit level 0-10, unit digit is; 1 for Eco, 2 for Trial, 3 for Trubo
//   GEN2 [0x01, 0x10] U-Int-8 Peak Assit Eco, U-Int-8 Peak Assit Trail, U-Int-8 Peak Assit Turbo
//   [0x02, 0x03] U-Int-8 Assit Eco Percentage
//   [0x02, 0x04] U-Int-8 Assit Trail Percentage
//   [0x02, 0x05] U-Int-8 Assit Turbo Percentage
//   [0x02, 0x06] U-Int-8 Fake Channel No
#define UUID_SPECIALIZED_WRITE_KEY_VALUE_CHARACTERISTIC_GEN1  "00000012-0000-4b49-4e4f-525441474947"
#define UUID_SPECIALIZED_WRITE_KEY_VALUE_CHARACTERISTIC_GEN2  "00000012-3731-3032-494d-484f42525554"

// Specialized Notify Service
//
// The notify service allows parameters of the e-bike to be continually updated from the bike sending notification messages.
//
#define UUID_SPECIALIZED_NOTIFY_SERVICE_GEN1 "00000003-0000-4b49-4e4f-525441474947"
#define UUID_SPECIALIZED_NOTIFY_SERVICE_GEN2 "00000003-3731-3032-494d-484f42525554"

// Specialized Notification Characteristic
//
// The notify service returns values in the key value format of that of the 'Specialized Read Value Characterisitic'
// However, the returned values seem to be restricted to the following:
//
// U-Int-8 Ebike Status Area Key Identifier
//   (see Specialized Read Key Write Characteristic for specifics)
// U-Int-8 Ebike Status Area Specific Key Identifier 
//   (see Specialized Read Key Write Characteristic for specifics)
// Variable-Length Ebike Status Value
//   (see Specialized Read Value Characterisitic for specifics)
//
//   Special attention should be paid to [0x01, 0x10] Peak Assit for GEN1 bikes as the notification seems to only way
//   Peak Assit values are obtained, as direct reading via 'Specialized Read Value Characterisitic' seems to respond with
//   values for Eco, Trail and Turbo levels at random.
//
#define UUID_SPECIALIZED_NOTIFY_KEY_VALUE_CHARACTERISTIC_GEN1  "00000013-0000-4b49-4e4f-525441474947"
#define UUID_SPECIALIZED_NOTIFY_KEY_VALUE_CHARACTERISTIC_GEN2  "00000013-3731-3032-494d-484f42525554"

union EbikeStatusAttribute {
    EbikeStatusBattery ebikeStatusBattery;
    EbikeStatusMotor ebikeStatusMotor;
    EbikeStatusOther ebikeStatusOther;
};

#endif