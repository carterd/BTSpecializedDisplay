#ifndef  _CONFIG_STORE_H
#define  _CONFIG_STORE_H

#include <Arduino.h>
#include <unordered_map>

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
private: 
    LittleFS_MBED* littleFS;
    const char* knownDevicesFilename;
    std::unordered_map<String, String> btAddressMap;

    bool readInt(FILE* file, int16_t* integer);
    bool writeInt(FILE* file, int16_t* integer);
    bool readString(FILE* file, String* string);
    bool writeString(FILE* file, String* string);
    bool readBTAddressMap();
    bool writeBTAddressMap();
public:
    ConfigStore();
    void init();
    void addBTAddress(const char* address, const char* display);
    void addBTAddress(String* addressString, String* displayString);
    void removeBTAddress(const char* address);
    void removeBTAddress(String* addressString);
    bool containsBTAddress(const char* address);
    bool containsBTAddress(String* addressString);
    String* getDisplayString(const char* address);
    String* getDisplayString(String* addressString);
    int countBTAddresses();
};

#endif