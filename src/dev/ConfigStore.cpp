#include "ConfigStore.h"

#define FORCE_REFOMAT false;
#define RP2040_FS_SIZE_KB 64
#define LFS_MBED_RP2040_DEBUG 1
#define _LFS_LOGLEVEL_ 100

#include <LittleFS_Mbed_RP2040.h>
#include <Arduino.h>

ConfigStore::ConfigStore() : knownDevicesFilename(MBED_LITTLEFS_FILE_PREFIX "/knownDevices.bin") {
}

void ConfigStore::init() {    
    this->littleFS = new LittleFS_MBED();

    if (!this->littleFS->init()) 
    {
        return;
    }
    if (this->readBTAddressMap()) {
        Serial.println("LITTLEFS read File");
    } else {
        Serial.println("LITTLEFS failure read File");
    }
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

bool ConfigStore::readBTAddressMap() {
    this->btAddressMap.clear();
    FILE *file = fopen(this->knownDevicesFilename, "r");
    if (file) {
        int16_t addressesSize = 0;
        this->readInt(file, &addressesSize);
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
