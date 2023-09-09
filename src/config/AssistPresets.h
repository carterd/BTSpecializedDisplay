#ifndef  _ASSIST_PRESETS_H
#define  _ASSIST_PRESETS_H

#include "BikeConfig.h"

#define PRESET_NAME_DEFAULT     "Default"
#define PRESET_NAME_LOW         "Low"
#define PRESET_NAME_ECO         "Eco"
#define PRESET_NAME_LIMP        "Limp"
#define PRESET_NAME_NORMAL      "Normal"
#define PRESET_NAME_MEDIUM      "Medium"
#define PRESET_NAME_HIGH        "High"
#define PRESET_NAME_THRASH      "Thrash"
#define PRESET_NAME_FAVOURITE   "Favourite"
#define PRESET_NAME_PRESET_1    "Preset-1"
#define PRESET_NAME_PRESET_2    "Preset-2"
#define PRESET_NAME_PRESET_3    "Preset-3"
#define PRESET_NAME_PRESET_4    "Preset-4"

struct BikeAssistPreset {
    String Name;
    AssistLevels supportAssistLevels;      // Should be something like 5,20,25,100 et-al
    AssistLevels peakPowerAssistLevels;       // Should be just 0-10

    BikeAssistPreset(const char* name, AssistLevels* supportAssistLevels, AssistLevels* peakPowerAssistLevels) {
        this->Name = name;
        this->supportAssistLevels = *supportAssistLevels;
        this->peakPowerAssistLevels = *peakPowerAssistLevels;
    }

    BikeAssistPreset(String name, AssistLevels supportAssistLevels, AssistLevels peakPowerAssistLevels) {
        this->Name = name;
        this->supportAssistLevels = supportAssistLevels;
        this->peakPowerAssistLevels = peakPowerAssistLevels;
    }
    
    BikeAssistPreset() {}

     friend bool operator==(const BikeAssistPreset& lhs, const BikeAssistPreset& rhs) {
        return (lhs.Name == rhs.Name && lhs.supportAssistLevels == rhs.supportAssistLevels && lhs.peakPowerAssistLevels == rhs.peakPowerAssistLevels);
    }
};

#endif