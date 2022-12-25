#include "ConfigStore.h"


#include <lvgl.h>

#ifndef ARDUINO_WIN_EMULATION
#define FORCE_REFOMAT false;
#define RP2040_FS_SIZE_KB 64
#define LFS_MBED_RP2040_DEBUG 1
#define _LFS_LOGLEVEL_ 100
#include <Arduino.h>
#include <LittleFS_Mbed_RP2040.h>
#define FS_FILE_PREFIX MBED_LITTLEFS_FILE_PREFIX
#define FS_SEPARATOR "/"
#else
#define FS_FILE_PREFIX "C:\\tmp"
#define FS_SEPARATOR "\\"
#endif

ConfigStore::ConfigStore() : 
    knownDevicesFilename(FS_FILE_PREFIX FS_SEPARATOR "knownDevices.bin"),
    bikeConfigFilename(FS_FILE_PREFIX FS_SEPARATOR "bikeConfig.bin"),
    displayConfigFilename(FS_FILE_PREFIX FS_SEPARATOR "displayConfig.bin"),
    savesDirectoryName(FS_FILE_PREFIX FS_SEPARATOR "saves")
{
}

void ConfigStore::init() {    
#ifndef ARDUINO_WIN_EMULATION
    this->littleFS = new LittleFS_MBED();
    if (!this->littleFS->init()) 
    {
        return;
    }
#endif    
    if (this->readBTAddressMap() && this->readBikeConfig() && this->readDisplayConfig() && this->savesConfig()) {
    } else {
        LV_LOG_ERROR("Error reading config files, initialise default config.");
        this->writeDefaults();
    }
}

void ConfigStore::writeDefaults() {
    this->defaults();
    this->writeBTAddressMap();
    this->writeBikeConfig();
    this->writeDisplayConfig();
}

void ConfigStore::defaults() {
    this->btAddressesConfig.clear();
    this->bikeConfig.beeper = BikeConfigAttr<bool>(false, false);                   // beeper 0 No beeping, 1 beeps
    this->bikeConfig.fakeChannel = BikeConfigAttr<uint8_t>(0, false);                // fakeChannel
    this->bikeConfig.wheelCircumference = BikeConfigAttr<uint16_t>(2293, false);     // Wheel Circumf start at 2293 mm
    this->bikeConfig.supportAssistLevels = BikeConfigAttr<AssistLevels>({10,40,100}, false); // assistLevelEco Should be something like 5,20,25,100 et-al
    this->bikeConfig.peakPowerAssistLevels = BikeConfigAttr<AssistLevels>({25,100,100}, false); // peakAssistEco Should be just 0-100

    this->displayConfig.contrast = 0x2F;  // Between 0x2F and 0x00
    this->displayConfig.monitorType = 0x0000;
    this->displayConfig.connectBatteryOnly = false;
    this->displayConfig.connectOnBoot = false;
    this->displayConfig.unitsMetric = true;

}

void ConfigStore::updateBTAddressesConfig(BTAddressesConfig& btAddressesConfig)
{
    // Ensure that btAddressConfig has changed before write
    if (this->btAddressesConfig.btAddressMap == btAddressesConfig.btAddressMap) {
        return;
    }
    this->btAddressesConfig = btAddressesConfig;
    this->writeBTAddressMap();
}

BTAddressesConfig& ConfigStore::getBTAddressesConfig()
{
    return this->btAddressesConfig;
}

void ConfigStore::updateBikeConfig(BikeConfig& bikeConfig) {
    if (this->bikeConfig == bikeConfig) {
        return;
    }
    this->bikeConfig = bikeConfig;
    this->writeBikeConfig();
}

BikeConfig& ConfigStore::getBikeConfig() {
    return this->bikeConfig;
}

void ConfigStore::updateDisplayConfig(DisplayConfig& displayConfig) {
    if (this->displayConfig == displayConfig) {
        return;
    }
    this->displayConfig = displayConfig;
    this->writeDisplayConfig();
}

DisplayConfig& ConfigStore::getDisplayConfig() {
    return this->displayConfig;
}

bool ConfigStore::readDisplayConfig() {
    FILE *file = fopen(this->displayConfigFilename, "r");
    if (file) {        
        bool result = true;
        result &= this->readUInt8(file, &(this->displayConfig.contrast));
        result &= this->readUInt16(file, &(this->displayConfig.monitorType));
        result &= this->readBool(file, &(this->displayConfig.connectBatteryOnly));
        result &= this->readBool(file, &(this->displayConfig.connectOnBoot));
        result &= this->readBool(file, &(this->displayConfig.unitsMetric));
        fclose(file);
        return result;
    }
    return false;
}

bool ConfigStore::writeDisplayConfig() {
    FILE *file = fopen(this->displayConfigFilename, "w");
    if (file) {
        this->writeUInt8(file, &(this->displayConfig.contrast));
        this->writeUInt16(file, &(this->displayConfig.monitorType));
        this->writeBool(file, &(this->displayConfig.connectBatteryOnly));
        this->writeBool(file, &(this->displayConfig.connectOnBoot));
        this->writeBool(file, &(this->displayConfig.unitsMetric));
        fclose(file);
        return true;
    }
    return false;
}

bool ConfigStore::savesConfig()
{
    DIR* dir = opendir(savesDirectoryName);

    if (dir != NULL) {
        closedir(dir);
        return true;
    }
    mkdir(savesDirectoryName, S_IRWXU | S_IRWXG | S_IRWXO);

    return true;
}

bool ConfigStore::ReadSavesNames()
{
    this->savesNames.names.clear();

    DIR* dir = opendir(savesDirectoryName);
    if (dir == NULL) return false;

    struct dirent* pDirent;
    while ((pDirent = readdir(dir)) != NULL) {
        this->savesNames.names.push_back(String(pDirent->d_name));
    }
    
    closedir(dir);

    return true;
}

bool ConfigStore::readBikeConfig() {    
    FILE *file = fopen(this->bikeConfigFilename, "r");
    if (file) {
        bool result = true;
        result &= this->readBool(file, &(this->bikeConfig.beeper.managed));
        result &= this->readBool(file, &(this->bikeConfig.beeper.value));
        result &= this->readBool(file, &(this->bikeConfig.fakeChannel.managed));
        result &= this->readUInt8(file, &(this->bikeConfig.fakeChannel.value));
        result &= this->readBool(file, &(this->bikeConfig.wheelCircumference.managed));
        result &= this->readUInt16(file, &(this->bikeConfig.wheelCircumference.value));
        result &= this->readBool(file, &(this->bikeConfig.supportAssistLevels.managed));
        result &= this->readUInt8(file, &(this->bikeConfig.supportAssistLevels.value.eco));
        result &= this->readUInt8(file, &(this->bikeConfig.supportAssistLevels.value.trail));
        result &= this->readUInt8(file, &(this->bikeConfig.supportAssistLevels.value.turbo));
        result &= this->readBool(file, &(this->bikeConfig.peakPowerAssistLevels.managed));
        result &= this->readUInt8(file, &(this->bikeConfig.peakPowerAssistLevels.value.eco));
        result &= this->readUInt8(file, &(this->bikeConfig.peakPowerAssistLevels.value.trail));
        result &= this->readUInt8(file, &(this->bikeConfig.peakPowerAssistLevels.value.turbo));
        fclose(file);
        return result;
    }
    return false;
}

bool ConfigStore::writeBikeConfig() {
    FILE *file = fopen(this->bikeConfigFilename, "w");
    if (file) {
        this->writeBool(file, &(this->bikeConfig.beeper.managed));
        this->writeBool(file, &(this->bikeConfig.beeper.value));
        this->writeBool(file, &(this->bikeConfig.fakeChannel.managed));
        this->writeUInt8(file, &(this->bikeConfig.fakeChannel.value));
        this->writeBool(file, &(this->bikeConfig.wheelCircumference.managed));
        this->writeUInt16(file, &(this->bikeConfig.wheelCircumference.value));
        this->writeBool(file, &(this->bikeConfig.supportAssistLevels.managed));
        this->writeUInt8(file, &(this->bikeConfig.supportAssistLevels.value.eco));
        this->writeUInt8(file, &(this->bikeConfig.supportAssistLevels.value.trail));
        this->writeUInt8(file, &(this->bikeConfig.supportAssistLevels.value.turbo));
        this->writeBool(file, &(this->bikeConfig.peakPowerAssistLevels.managed));
        this->writeUInt8(file, &(this->bikeConfig.peakPowerAssistLevels.value.eco));
        this->writeUInt8(file, &(this->bikeConfig.peakPowerAssistLevels.value.trail));
        this->writeUInt8(file, &(this->bikeConfig.peakPowerAssistLevels.value.turbo));
        fclose(file);
        return true;
    }
    return false;
}

bool ConfigStore::readBTAddressMap() {
    this->btAddressesConfig.clear();
    FILE *file = fopen(this->knownDevicesFilename, "r");
    if (file) {
        uint16_t addressesSize = 0;
        if (!this->readUInt16(file, &addressesSize)) return false;
        for (uint16_t i = 0; i < addressesSize; i++) {
            String addressString;
            String displayValueString;
            if (!this->readString(file, &addressString)) return false;
            if (!this->readString(file, &displayValueString)) return false;
            this->btAddressesConfig.addBTAddress(addressString ,displayValueString);
        }
        fclose(file);
        return true;
    }
    return false;
}

bool ConfigStore::writeBTAddressMap() {
    FILE *file = fopen(this->knownDevicesFilename, "w");
    if (file) {
        uint16_t addressesSize = this->btAddressesConfig.countBTAddresses();
        if (!this->writeUInt16(file, &addressesSize)) return false;
        for (std::unordered_map<String, String>::iterator it = this->btAddressesConfig.btAddressMap.begin() ; it != this->btAddressesConfig.btAddressMap.end(); it++) {
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


bool ConfigStore::readUInt16(FILE* file, uint16_t* value) {
    return (fread(value, sizeof(uint16_t), 1, file) == 1);
}

bool ConfigStore::readUInt8(FILE* file, uint8_t* value) {
    return (fread(value, sizeof(uint8_t), 1, file) == 1);
}

bool ConfigStore::readBool(FILE* file, bool* value) {
    uint8_t int8Value;
    if (this->readUInt8(file, &int8Value)) {
        *value = int8Value;
        return true;
    }
    return false;
}

bool ConfigStore::writeUInt16(FILE* file, uint16_t* value) {
    return (fwrite(value, sizeof(uint16_t), 1, file) == 1);
}

bool ConfigStore::writeUInt8(FILE* file, uint8_t* value) {
    return (fwrite(value, sizeof(int8_t), 1, file) == 1);
}

bool ConfigStore::writeBool(FILE* file, bool* value) {
    uint8_t int8Value = *value ? 0x01 : 0x00;
    return this->writeUInt8(file, &int8Value);
}

bool ConfigStore::readString(FILE* file, String* string) {
    uint16_t stringSize;
    if (!this->readUInt16(file, &stringSize)) return false;
    char* readBuffer = (char*) malloc(stringSize + 1);
    if (fread(readBuffer, stringSize, 1, file) != 1)
    {
        free(readBuffer);
        return false;
    }
    readBuffer[stringSize] = 0x00;
    *string = readBuffer;
    free(readBuffer);
    return true;
}

bool ConfigStore::writeString(FILE* file, String* string) {
    uint16_t stringSize = string->length();
    if (!this->writeUInt16(file, &stringSize)) return false;
    return (fwrite(string->c_str(), stringSize, 1, file) == 1);
}
