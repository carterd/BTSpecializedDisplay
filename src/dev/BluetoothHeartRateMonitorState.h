#ifndef _BLUETOOTH_HEART_RATE_MONITOR_STATE_H
#define _BLUETOOTH_HEART_RATE_MONITOR_STATE_H

#include <Arduino.h>

class HeartRateMonitorState {
public:    
    bool sensorContactSupported;
    bool sensorContactStatus;
    uint16_t heartRateBpm;
    uint16_t expendedEnergy;
    uint8_t rrIntervalCount;
    float rrInterval[10];
    uint32_t lastFetchTimeTicks;
};

#endif