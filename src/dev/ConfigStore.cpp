#include "ConfigStore.h"

#define FORCE_REFOMAT false;
#define RP2040_FS_SIZE_KB 64
#define LFS_MBED_RP2040_DEBUG 1
#define _LFS_LOGLEVEL_ 100

#include <LittleFS_Mbed_RP2040.h>
#include <Arduino.h>

ConfigStore::ConfigStore() : 
    knownDevicesFilename(MBED_LITTLEFS_FILE_PREFIX "/knownDevices.bin"),
    bikeConfigFilename(MBED_LITTLEFS_FILE_PREFIX "/bikeConfig.bin"),
    displayConfigFilename(MBED_LITTLEFS_FILE_PREFIX "/displayConfig.bin")
{
}

void ConfigStore::init() {    
    this->littleFS = new LittleFS_MBED();

    if (!this->littleFS->init()) 
    {
        return;
    }
    if (this->readBTAddressMap() && this->readBikeConfig() && this->readDisplayConfig()) {
        Serial.println("LITTLEFS read File");
    } else {
        Serial.println("LITTLEFS failure read File");
        this->defaults();
        this->writeBTAddressMap();
        this->writeBikeConfig();
        this->writeDisplayConfig();
    }
}

void ConfigStore::defaults() {
    this->btAddressMap.clear();
    this->bikeConfig = { 
        UNMANAGED_CONFIG,       // beeper 0 No beeping, 1 beeps
        UNMANAGED_CONFIG,       // fakeChannel
        UNMANAGED_CONFIG,       // Wheel Circumf
        UNMANAGED_CONFIG,       // assistLevelEco Should be something like 5,20,25,100 et-al
        UNMANAGED_CONFIG,       // assistLevelTrail
        UNMANAGED_CONFIG,       // assistLevelTurbo
        UNMANAGED_CONFIG,       // peakAssistEco Should be just 0-10
        UNMANAGED_CONFIG,       // peakAssistTrail
        UNMANAGED_CONFIG,       // peakAssistTurbo
    };
    this->displayConfig = {
        0x002F,                 // Between 0x2F and 0x00
        0x0000,                 // Enumeration of preset monitor settings  
    };
}

void ConfigStore::updateBikeConfig(BikeConfig bikeConfig) {
    this->bikeConfig = bikeConfig;
    this->writeBikeConfig();
}

ConfigStore::BikeConfig ConfigStore::getBikeConfig() {
    return this->bikeConfig;
}

void ConfigStore::updateDisplayConfig(DisplayConfig displayConfig) {
    this->displayConfig = displayConfig;
    this->writeDisplayConfig();
}

ConfigStore::DisplayConfig ConfigStore::getDisplayConfig() {
    return this->displayConfig;
}

void ConfigStore::removeBTAddress(const char* address) {
    String addressString(address);
    this->removeBTAddress(&addressString);
}

void ConfigStore::removeBTAddress(String* addressString) {
    this->btAddressMap.erase(*addressString);
}

bool ConfigStore::containsBTAddress(const char* address) {
    String addressString(address);
    return this->containsBTAddress(&addressString);
}

bool ConfigStore::containsBTAddress(String* addressString) {
    return this->btAddressMap.count(*addressString);
}

String* ConfigStore::getDisplayString(const char* address) {
    String addressString(address);
    return this->getDisplayString(&addressString);
}

String* ConfigStore::getDisplayString(String* addressString) {
    if (this->btAddressMap.count(*addressString)) {
        return &(this->btAddressMap[*addressString]);
    }
    return NULL;
}

int ConfigStore::countBTAddresses() {
    return this->btAddressMap.size();
}

void ConfigStore::addBTAddress(const char* address, const char* display) {
    String addressString(address);
    String displayString(display);
    this->addBTAddress(&addressString, &displayString);
}

void ConfigStore::addBTAddress(String* addressString, String* displayString) {
       this->btAddressMap[*addressString] = *addressString;
    if (this->writeBTAddressMap()) {
        Serial.println("LITTLEFS wrote File");
    } else {
        Serial.println("LITTLEFS failure write File");
    } 
}

bool ConfigStore::readDisplayConfig() {
    FILE *file = fopen(this->displayConfigFilename, "r");
    if (file) {        
        bool error = false;
        error &= this->readInt(file, &(this->displayConfig.contrast));
        error &= this->readInt(file, &(this->displayConfig.monitorType));                
        fclose(file);
        return !error;
    }
    return false;
}

bool ConfigStore::writeDisplayConfig() {
    FILE *file = fopen(this->displayConfigFilename, "w");
    if (file) {
        this->writeInt(file, &(this->displayConfig.contrast));
        this->writeInt(file, &(this->displayConfig.monitorType));
        fclose(file);
        return true;
    }
    return false;
}

bool ConfigStore::readBikeConfig() {    
    FILE *file = fopen(this->bikeConfigFilename, "r");
    if (file) {
        bool error = false;
        error &= this->readInt(file, &(this->bikeConfig.beeper));
        error &= this->readInt(file, &(this->bikeConfig.fakeChannel));
        error &= this->readInt(file, &(this->bikeConfig.wheelCircumference));
        error &= this->readInt(file, &(this->bikeConfig.assistLevelEco));
        error &= this->readInt(file, &(this->bikeConfig.assistLevelTrail));
        error &= this->readInt(file, &(this->bikeConfig.assistLevelTurbo));
        error &= this->readInt(file, &(this->bikeConfig.peakAssistEco));
        error &= this->readInt(file, &(this->bikeConfig.peakAssistTrail));
        error &= this->readInt(file, &(this->bikeConfig.peakAssistTurbo));
        fclose(file);
        return !error;
    }
    return false;
}

bool ConfigStore::writeBikeConfig() {
    FILE *file = fopen(this->bikeConfigFilename, "w");
    if (file) {
        this->writeInt(file, &(this->bikeConfig.beeper));
        this->writeInt(file, &(this->bikeConfig.fakeChannel));
        this->writeInt(file, &(this->bikeConfig.wheelCircumference));
        this->writeInt(file, &(this->bikeConfig.assistLevelEco));
        this->writeInt(file, &(this->bikeConfig.assistLevelTrail));
        this->writeInt(file, &(this->bikeConfig.assistLevelTurbo));
        this->writeInt(file, &(this->bikeConfig.peakAssistEco));
        this->writeInt(file, &(this->bikeConfig.peakAssistTrail));
        this->writeInt(file, &(this->bikeConfig.peakAssistTurbo));
        fclose(file);
        return true;
    }
    return false;
}

bool ConfigStore::readBTAddressMap() {
    this->btAddressMap.clear();
    FILE *file = fopen(this->knownDevicesFilename, "r");
    if (file) {
        int16_t addressesSize = 0;
        if (!this->readInt(file, &addressesSize)) return false;
        for (int16_t i = 0; i < addressesSize; i++) {
            String addressString;
            String displayValueString;
            if (!this->readString(file, &addressString)) return false;
            if (!this->readString(file, &displayValueString)) return false;
            this->btAddressMap[addressString] = displayValueString;
        }
        fclose(file);
        return true;
    }
    return false;
}

bool ConfigStore::writeBTAddressMap() {
    FILE *file = fopen(this->knownDevicesFilename, "w");
    if (file) {
        int16_t addressesSize = this->btAddressMap.size();
        if (!this->writeInt(file, &addressesSize)) return false;
        for (std::unordered_map<String, String>::iterator it = this->btAddressMap.begin() ; it != this->btAddressMap.end(); it++) {
            String address = ((*it).first);
            String displayValue = ((*it).second);
            if (!this->writeString(file, &address)) return false;
            if (!this->writeString(file, &displayValue)) return false;
        }
        fclose(file);
        return true;
    }
    return false;
}


bool ConfigStore::readInt(FILE* file, int16_t* integer) {
    return (fread(integer, sizeof(int16_t), 1, file) == 1);
}

bool ConfigStore::writeInt(FILE* file, int16_t* integer) {
    return (fwrite(integer, sizeof(int16_t), 1, file) == 1);
}

bool ConfigStore::readString(FILE* file, String* string) {
    int16_t stringSize;
    if (!this->readInt(file, &stringSize)) return false;
    string->reserve(stringSize + 1);
    char * readBuffer = string->begin() + 1; 

    if (fread(readBuffer, stringSize, 1, file) != 1) return false;
    readBuffer[stringSize] = 0x00;
    *string = readBuffer;
    return true;
}

bool ConfigStore::writeString(FILE* file, String* string) {
    int16_t stringSize = string->length();
    if (!this->writeInt(file, &stringSize)) return false;
    return (fwrite(string->c_str(), stringSize, 1, file) == 1);
}
