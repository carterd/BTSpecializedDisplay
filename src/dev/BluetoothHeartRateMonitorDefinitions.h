#ifndef _BLUETOOTH_HEART_RATE_MONITOR_DEFINITIONS_H
#define _BLUETOOTH_HEART_RATE_MONITOR_DEFINITIONS_H

// This is the Cycle Service for GEN-1 Bikes, it's advertised as a 16-bit Service number
#define UUID_HEART_RATE_SERVICE                   "180d"

// The Heart Rate Measurement Characteristic
//
// U-Int-8 (Heart Rate Monitor Flags) 
//   bit 0 - TRUE indicates 16 bit measurement, FALSE indicates 32 bit measurement
//   bit 1 - Sensor Contact Status
//   bit 2 - Seosor Contact Supported
//   bit 3 - Energy Expended status bit
//   bit 4 - RR Interval supported bit
// U-Int-8 (BPM measurement) [only if Monitor Flags bit 0 = FALSE Heart rate monitor Flags]
// U-Int-16 (BPM measurement) [only if Monitor Flags bit 0 = TRUE Heart rate monitor Flags]
//   This value represents the current bpm measured
// U-Int-16 (Energy Expended) [only present if bit 3 = TRUE Heart rate monitor Flags]
//   This value represents the energy expended value
// U-Int-16 x N (number of RR Intervals) [only present if bit 4 = TRUE Heart rate monitor Flags]
//   Milliseconds for RR Intervals
#define UUID_HEART_RATE_MEASUREMENT_CHARACTERISTIC  "2a37"
// U-Int-8 (Heart Rate Monitor Flags) 
enum class HeartRateMonitorFlags { MEASURE_SIZE=0x01, SENSOR_CONTACT_STATUS=0x02, SENSOR_CONTACT_SUPPORT=0x04, ENERGY_EXPENDED_STATUS_BIT=0x08, RR_INTERVAL_SUPPORT_BIT=0x10, RESERVED2=0x20, RESERVED3_BIT=0x40, RESERVED4_BIT=0x80 };

// Body Sensor Location Characteristic
//
#define UUID_BODY_SENSOR_LOCATION_CHARACTERISTIC      "2a38"

#endif
