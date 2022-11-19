#ifndef  _CONFIG_STORE_H
#define  _CONFIG_STORE_H

#include <Arduino.h>
#include <unordered_map>

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

class ConfigStore 
{
public:
    struct DisplayConfig {
        int16_t contrast;               // Between 0x2F and 0x00
        int16_t monitorType;            // Enumeration of preset monitor settings
    };
    struct BikeConfig {
        int16_t beeper;                 // 0 No beeping, 1 beeps
        int16_t fakeChannel;            //
        int16_t wheelCircumference;     //
        int16_t assistLevelEco;         // Should be something like 5,20,25,100 et-al
        int16_t assistLevelTrail;       //
        int16_t assistLevelTurbo;       //
        int16_t peakAssistEco;          // Should be just 0-10
        int16_t peakAssistTrail;        //
        int16_t peakAssistTurbo;        //
    };
private: 
    LittleFS_MBED* littleFS;
    const char* knownDevicesFilename;
    const char* bikeConfigFilename;
    const char* displayConfigFilename;

    std::unordered_map<String, String> btAddressMap;
    DisplayConfig displayConfig;
    BikeConfig bikeConfig;

    bool readInt(FILE* file, int16_t* integer);
    bool writeInt(FILE* file, int16_t* integer);
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
    void addBTAddress(const char* address, const char* display);
    void addBTAddress(String* addressString, String* displayString);
    void removeBTAddress(const char* address);
    void removeBTAddress(String* addressString);
    void updateBikeConfig(BikeConfig bikeConfig);
    BikeConfig getBikeConfig();
    void updateDisplayConfig(DisplayConfig bikeConfig);
    DisplayConfig getDisplayConfig();
    bool containsBTAddress(const char* address);
    bool containsBTAddress(String* addressString);
    String* getDisplayString(const char* address);
    String* getDisplayString(String* addressString);
    int countBTAddresses();
};

#endif