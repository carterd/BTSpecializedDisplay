#ifndef  _ASSIST_PRESETS_H
#define  _ASSIST_PRESETS_H

#include "BikeConfig.h"

struct BikeAssistPreset {
    const char* Name;
    AssistLevels supportAssistLevels;      // Should be something like 5,20,25,100 et-al
    AssistLevels peakPowerAssistLevels;       // Should be just 0-10
};

#define BIKE_ASSIST_LEVELS 3
BikeAssistPreset bikeAssistPresets[BIKE_ASSIST_LEVELS] = {
    //  NAME            SUPPORT(10s)        PEAK POWER(5s)
    //                  eco/trail/turbo     eco/trail/turbo
        {"Default",      {20, 50, 100},      {50, 100, 100}},
        {"Eco",          {10, 20, 50},       {10, 50, 50}},
        {"Thrash",       {50, 80, 100},      {50, 100, 100}},
};

#endif