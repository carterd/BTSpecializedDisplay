#ifndef  _CONFIG_STORE_H
#define  _CONFIG_STORE_H

#include <Arduino.h>
#include <unordered_map>

#include "BluetoothBikeState.h"

#define UNMANAGED_CONFIG -1

typedef std::pair<String, String> BTEntry;

class LittleFS_MBED;

#ifndef ARDUINO_WIN_EMULATION
template<>
class std::hash<String> {
    public:
    std::size_t operator()(const String& s) const {
        std::size_t hash = 0;
        const char* ptr = s.c_str();
        for(unsigned int i = 0; i < s.length(); i++) {
            hash ^= ptr[i] << ((i % sizeof(std::size_t)) * 8);
        }
        return hash;
    }
};
#endif

enum class DisplayConfigAttributeIndex { Contrast = 1, MonitorType = 2, ConnectBatteryOnly = 3, ConnectOnBoot = 4, UnitsMetricImperial = 5 };
enum class BikeConfigAttributeIndex { Beeper = 1, FakeChannel = 2, WheelCircumference = 3, SupportAssistEco = 4, SupportAssistTrail = 5, SupportAssistTurbo = 6, PeakPowerAssistEco = 7, PeakPowerAssistTrail = 8, PeakPowerAssistTurbo = 9, SupportAssistLevelsManaged = 10, PeakPowerAssistLevelsManaged = 11 };

struct DisplayConfig {
public:
    uint8_t contrast;                // Between 0x2F and 0x00
    uint16_t monitorType;            // Enumeration of preset monitor settings
    bool connectBatteryOnly;        // This is more of a debug feature as you normally wont want to connect to battery only
    bool connectOnBoot;             // This is more likely to want to default to true
    bool unitsMetric;

    DisplayConfig(int8_t contrast, int16_t monitorType, bool connectBatteryOnly, bool connectOnBoot, bool unitsMetric) {
        this->contrast = contrast;
        this->monitorType = monitorType;
        this->connectBatteryOnly = connectBatteryOnly;
        this->connectOnBoot = connectOnBoot;
        this->unitsMetric = unitsMetric;
    };
    DisplayConfig() {};
    friend bool operator==(const DisplayConfig& lhs, const DisplayConfig& rhs) {
        return (lhs.contrast == rhs.contrast && lhs.monitorType == rhs.monitorType && lhs.connectBatteryOnly == rhs.connectBatteryOnly && lhs.connectOnBoot == rhs.connectOnBoot && lhs.unitsMetric == rhs.unitsMetric);
    }
    void setIntValue(DisplayConfigAttributeIndex displayConfigAttributeIndex, int newValue) {
        switch (displayConfigAttributeIndex) {
            case DisplayConfigAttributeIndex::Contrast:
            this->contrast = newValue;
            break;
            case DisplayConfigAttributeIndex::MonitorType:
            this->monitorType = newValue;
            break;
            case DisplayConfigAttributeIndex::ConnectBatteryOnly:
            this->connectBatteryOnly = newValue;
            break;
            case DisplayConfigAttributeIndex::ConnectOnBoot:
            this->connectOnBoot = newValue;
            break;
            case DisplayConfigAttributeIndex::UnitsMetricImperial:
            this->unitsMetric = newValue;
            break;
        }
    };
    int getIntValue(DisplayConfigAttributeIndex displayConfigAttributeIndex) {
        int initValue = 0;
        switch (displayConfigAttributeIndex) {
            case DisplayConfigAttributeIndex::Contrast:
            initValue = this->contrast;
            break;
            case DisplayConfigAttributeIndex::MonitorType:
            initValue = this->monitorType;
            break;
            case DisplayConfigAttributeIndex::ConnectBatteryOnly:
            initValue = this->connectBatteryOnly;
            break;
            case DisplayConfigAttributeIndex::ConnectOnBoot:
            initValue = this->connectOnBoot;
            break;
            case DisplayConfigAttributeIndex::UnitsMetricImperial:
            initValue = this->unitsMetric;
            break;
        }
        return initValue;
    };
};

template <typename T>
class BikeConfigAttr {
public:
    T value;
    bool managed;
    BikeConfigAttr(T value, bool managed) { this->value = value; this->managed = managed; }
    BikeConfigAttr(bool managed) { this->manged = managed; }
    BikeConfigAttr() { this->managed = false; }
    void setMangedValue(T value) { this->managed = true; this->value = value; }
    friend bool operator==(const BikeConfigAttr<T>& lhs, const BikeConfigAttr<T>& rhs) {
        return (lhs.managed == rhs.managed && lhs.value == rhs.value);
    }
};


struct BikeConfig {
    BikeConfigAttr<bool> beeper;                   // 0 No beeping, 1 beeps
    BikeConfigAttr<uint8_t> fakeChannel;            //
    BikeConfigAttr<uint16_t> wheelCircumference;    //
    BikeConfigAttr<AssistLevels> supportAssistLevels;      // Should be something like 5,20,25,100 et-al
    BikeConfigAttr<AssistLevels> peakPowerAssistLevels;       // Should be just 0-10
    BikeConfig(BikeConfigAttr<bool> beeper, BikeConfigAttr<uint8_t> fakeChannel, BikeConfigAttr<uint16_t> wheelCircumference, BikeConfigAttr<AssistLevels> supportAssistLevels, BikeConfigAttr<AssistLevels> peakPowerAssistLevels) {
        this->beeper = beeper;
        this->fakeChannel = fakeChannel;
        this->wheelCircumference = wheelCircumference;
        this->supportAssistLevels = supportAssistLevels;
        this->peakPowerAssistLevels = peakPowerAssistLevels;
    }
    BikeConfig() {};
    void setManagedValue(BikeConfigAttributeIndex bikeConfigAttributeIndex, bool managed) {
        switch (bikeConfigAttributeIndex) {
            case BikeConfigAttributeIndex::Beeper:
            this->beeper.managed = managed;
            break;
            case BikeConfigAttributeIndex::FakeChannel:
            this->fakeChannel.managed = managed;
            break;
            case BikeConfigAttributeIndex::WheelCircumference:
            this->wheelCircumference.managed = managed;
            break;
            case BikeConfigAttributeIndex::SupportAssistLevelsManaged:
            case BikeConfigAttributeIndex::SupportAssistEco:
            case BikeConfigAttributeIndex::SupportAssistTrail:
            case BikeConfigAttributeIndex::SupportAssistTurbo:
            this->supportAssistLevels.managed = managed;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistLevelsManaged:
            case BikeConfigAttributeIndex::PeakPowerAssistEco:
            case BikeConfigAttributeIndex::PeakPowerAssistTrail:
            case BikeConfigAttributeIndex::PeakPowerAssistTurbo:
            this->peakPowerAssistLevels.managed = managed;
            break;
        }
    }
    friend bool operator==(const BikeConfig& lhs, const BikeConfig& rhs) {
        return (lhs.beeper == rhs.beeper && lhs.fakeChannel == rhs.fakeChannel && lhs.wheelCircumference == rhs.wheelCircumference && lhs.peakPowerAssistLevels == rhs.peakPowerAssistLevels && lhs.supportAssistLevels == rhs.supportAssistLevels);
    }
    void setIntValue(BikeConfigAttributeIndex bikeConfigAttributeIndex, int newValue) {
        switch (bikeConfigAttributeIndex) {
            case BikeConfigAttributeIndex::Beeper:
            this->beeper.value = newValue;
            break;
            case BikeConfigAttributeIndex::FakeChannel:
            this->fakeChannel.value = newValue;
            break;
            case BikeConfigAttributeIndex::WheelCircumference:
            this->wheelCircumference.value = newValue;
            break;
            case BikeConfigAttributeIndex::SupportAssistEco:
            this->supportAssistLevels.value.eco = newValue;
            break;
            case BikeConfigAttributeIndex::SupportAssistTrail:
            this->supportAssistLevels.value.trail = newValue;
            break;
            case BikeConfigAttributeIndex::SupportAssistTurbo:
            this->supportAssistLevels.value.turbo = newValue;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistEco:
            this->peakPowerAssistLevels.value.eco = newValue;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistTrail:
            this->peakPowerAssistLevels.value.trail = newValue;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistTurbo:
            this->peakPowerAssistLevels.value.turbo = newValue;
            break;
            case BikeConfigAttributeIndex::SupportAssistLevelsManaged:
            this->supportAssistLevels.managed = newValue;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistLevelsManaged:
            this->peakPowerAssistLevels.managed = newValue;
            break;
        }
    };
    bool getManagedValue(BikeConfigAttributeIndex bikeConfigAttributeIndex) {
        bool managed = false;
        switch (bikeConfigAttributeIndex) {
            case BikeConfigAttributeIndex::Beeper:
            managed = this->beeper.managed;
            break;
            case BikeConfigAttributeIndex::FakeChannel:
            managed = this->fakeChannel.managed;
            break;
            case BikeConfigAttributeIndex::WheelCircumference:
            managed = this->wheelCircumference.managed;
            break;
            case BikeConfigAttributeIndex::SupportAssistLevelsManaged:
            case BikeConfigAttributeIndex::SupportAssistEco:
            case BikeConfigAttributeIndex::SupportAssistTrail:
            case BikeConfigAttributeIndex::SupportAssistTurbo:
            managed = this->supportAssistLevels.managed;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistLevelsManaged:
            case BikeConfigAttributeIndex::PeakPowerAssistEco:
            case BikeConfigAttributeIndex::PeakPowerAssistTrail:
            case BikeConfigAttributeIndex::PeakPowerAssistTurbo:
            managed = this->peakPowerAssistLevels.managed;
        }
        return managed;
    };

    int getIntValue(BikeConfigAttributeIndex bikeConfigAttributeIndex) {
        int initValue = 0;
        switch (bikeConfigAttributeIndex) {
            case BikeConfigAttributeIndex::Beeper:
            initValue = this->beeper.value;
            break;
            case BikeConfigAttributeIndex::FakeChannel:
            initValue = this->fakeChannel.value;
            break;
            case BikeConfigAttributeIndex::WheelCircumference:
            initValue = this->wheelCircumference.value;
            break;
            case BikeConfigAttributeIndex::SupportAssistEco:
            initValue = this->supportAssistLevels.value.eco;
            break;
            case BikeConfigAttributeIndex::SupportAssistTrail:
            initValue = this->supportAssistLevels.value.trail;
            break;
            case BikeConfigAttributeIndex::SupportAssistTurbo:
            initValue = this->supportAssistLevels.value.turbo;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistEco:
            initValue = this->peakPowerAssistLevels.value.eco;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistTrail:
            initValue = this->peakPowerAssistLevels.value.trail;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistTurbo:
            initValue = this->peakPowerAssistLevels.value.turbo;
            break;
            case BikeConfigAttributeIndex::SupportAssistLevelsManaged:
            initValue = this->supportAssistLevels.managed;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistLevelsManaged:
            initValue = this->peakPowerAssistLevels.managed;
            break;
        }
        return initValue;
    };

};

struct BTAddressesConfig {
    std::unordered_map<String, String> btAddressMap;
    void clear() {
        this->btAddressMap.clear();
    }
    void addBTAddress(const char* address, const char* display) {
        String addressString(address);
        String displayString(display);
        this->addBTAddress(addressString, displayString);
    }
    void addBTAddress(const String& addressString, const String& displayString) {
        this->btAddressMap[addressString] = displayString;
    }
    void removeBTAddress(const char* address) {
        String addressString(address);
        this->removeBTAddress(addressString);
    }
    void removeBTAddress(const String& addressString) {
        this->btAddressMap.erase(addressString);
    }
    bool containsBTAddress(const char* address) {
        String addressString(address);
        return this->containsBTAddress(addressString);
    }
    bool containsBTAddress(const String& addressString) {
        return this->btAddressMap.count(addressString);
    }
    String* getBTAddressDisplayString(const char* address) {
        String addressString(address);
        return this->getBTAddressDisplayString(addressString);
    }
    String* getBTAddressDisplayString(const String& addressString) {
        if (this->btAddressMap.count(addressString)) {
            return &(this->btAddressMap[addressString]);
        }
        return NULL;
    }
    int countBTAddresses() {
        return this->btAddressMap.size();
    }
};

/// <summary>
/// The config store is a file system store used to store the display's configuration.
/// The displays configuration is diveded into the DisplayConfig and BikeConfig
/// </summary>
class ConfigStore 
{
private: 
    LittleFS_MBED* littleFS;
    const char* knownDevicesFilename;
    const char* bikeConfigFilename;
    const char* displayConfigFilename;

    BTAddressesConfig btAddressesConfig;
    DisplayConfig displayConfig;
    BikeConfig bikeConfig;

    bool readUInt16(FILE* file, uint16_t* value);
    bool readUInt8(FILE* file, uint8_t* value);
    bool readBool(FILE* file, bool* value);

    bool writeUInt16(FILE* file, uint16_t* integer);
    bool writeUInt8(FILE* file, uint8_t* integer);
    bool writeBool(FILE* file, bool* value);

    bool readString(FILE* file, String* string);
    bool writeString(FILE* file, String* string);
    // Reading/Writing address map file
    bool readBTAddressMap();
    bool writeBTAddressMap();
    // Reading/Writing BikeConfig
    bool readBikeConfig();
    bool writeBikeConfig();
    // Reading/Writing DisplayConfig
    bool readDisplayConfig();
    bool writeDisplayConfig();
public:
    ConfigStore();
    void init();
    void defaults();

    void updateBTAddressesConfig(BTAddressesConfig& btAddressesConfig);
    BTAddressesConfig& getBTAddressesConfig();
    void updateBikeConfig(BikeConfig& bikeConfig);
    BikeConfig& getBikeConfig();
    void updateDisplayConfig(DisplayConfig& bikeConfig);
    DisplayConfig& getDisplayConfig();
};

#endif