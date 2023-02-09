#include "ConfigStore.h"
#include <lvgl.h>

ConfigStore::ConfigStore(const char* knownDevicesFilename, const char* bikeConfigFilename, const char* displayConfigFilename, const char* savesDirectoryName) : 
    knownDevicesFilename(knownDevicesFilename), // FS_FILE_PREFIX FS_SEPARATOR "knownDevices.bin"),
    bikeConfigFilename(bikeConfigFilename), // FS_FILE_PREFIX FS_SEPARATOR "bikeConfig.bin"),
    displayConfigFilename(displayConfigFilename), // FS_FILE_PREFIX FS_SEPARATOR "displayConfig.bin"),
    savesDirectoryName(savesDirectoryName) // FS_FILE_PREFIX FS_SEPARATOR "saves")
{
}

void ConfigStore::init(FileSystem* fileSystem) {    
    this->fileSystem = fileSystem;
    if (!this->fileSystem->init()) { 
        LV_LOG_ERROR("Error FS unable to be initialised.");
        return; 
    }
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
    if (this->fileSystem->openFile(this->displayConfigFilename, "r")) {
        bool result = true;
        result &= this->readUInt8(&(this->displayConfig.contrast));
        result &= this->readUInt16(&(this->displayConfig.monitorType));
        result &= this->readBool(&(this->displayConfig.connectBatteryOnly));
        result &= this->readBool(&(this->displayConfig.connectOnBoot));
        result &= this->readBool(&(this->displayConfig.unitsMetric));
        this->fileSystem->closeFile();
        return result;
    }
    return false;
}

bool ConfigStore::writeDisplayConfig() {
    if (this->fileSystem->openFile(this->displayConfigFilename, "w")) {
        this->writeUInt8(&(this->displayConfig.contrast));
        this->writeUInt16(&(this->displayConfig.monitorType));
        this->writeBool(&(this->displayConfig.connectBatteryOnly));
        this->writeBool(&(this->displayConfig.connectOnBoot));
        this->writeBool(&(this->displayConfig.unitsMetric));
        this->fileSystem->closeFile();
        return true;
    }
    return false;
}

bool ConfigStore::savesConfig()
{
    if (this->fileSystem->openDir(savesDirectoryName)) {
        this->fileSystem->closeDir();
        return true;
    }
    this->fileSystem->mkDir(savesDirectoryName);

    return true;
}

bool ConfigStore::ReadSavesNames()
{
    this->savesNames.names.clear();
    if (! this->fileSystem->openDir(savesDirectoryName)) {
        return false;
    }

    char* dirName = NULL;
    while (this->fileSystem->readDir(&dirName)) {
        this->savesNames.names.push_back(String(dirName));
    }
    
    this->fileSystem->closeDir();

    return true;
}

bool ConfigStore::readBikeConfig() {
    if (this->fileSystem->openFile(this->bikeConfigFilename, "r")) {
        bool result = true;
        result &= this->readBool(&(this->bikeConfig.beeper.managed));
        result &= this->readBool(&(this->bikeConfig.beeper.value));
        result &= this->readBool(&(this->bikeConfig.fakeChannel.managed));
        result &= this->readUInt8(&(this->bikeConfig.fakeChannel.value));
        result &= this->readBool(&(this->bikeConfig.wheelCircumference.managed));
        result &= this->readUInt16(&(this->bikeConfig.wheelCircumference.value));
        result &= this->readBool(&(this->bikeConfig.supportAssistLevels.managed));
        result &= this->readUInt8(&(this->bikeConfig.supportAssistLevels.value.eco));
        result &= this->readUInt8(&(this->bikeConfig.supportAssistLevels.value.trail));
        result &= this->readUInt8(&(this->bikeConfig.supportAssistLevels.value.turbo));
        result &= this->readBool(&(this->bikeConfig.peakPowerAssistLevels.managed));
        result &= this->readUInt8(&(this->bikeConfig.peakPowerAssistLevels.value.eco));
        result &= this->readUInt8(&(this->bikeConfig.peakPowerAssistLevels.value.trail));
        result &= this->readUInt8(&(this->bikeConfig.peakPowerAssistLevels.value.turbo));
        this->fileSystem->closeFile();
        return result;
    }
    return false;
}

bool ConfigStore::writeBikeConfig() {
    if (this->fileSystem->openFile(this->bikeConfigFilename, "w")) {
        this->writeBool(&(this->bikeConfig.beeper.managed));
        this->writeBool(&(this->bikeConfig.beeper.value));
        this->writeBool(&(this->bikeConfig.fakeChannel.managed));
        this->writeUInt8(&(this->bikeConfig.fakeChannel.value));
        this->writeBool(&(this->bikeConfig.wheelCircumference.managed));
        this->writeUInt16(&(this->bikeConfig.wheelCircumference.value));
        this->writeBool(&(this->bikeConfig.supportAssistLevels.managed));
        this->writeUInt8(&(this->bikeConfig.supportAssistLevels.value.eco));
        this->writeUInt8(&(this->bikeConfig.supportAssistLevels.value.trail));
        this->writeUInt8(&(this->bikeConfig.supportAssistLevels.value.turbo));
        this->writeBool(&(this->bikeConfig.peakPowerAssistLevels.managed));
        this->writeUInt8(&(this->bikeConfig.peakPowerAssistLevels.value.eco));
        this->writeUInt8(&(this->bikeConfig.peakPowerAssistLevels.value.trail));
        this->writeUInt8(&(this->bikeConfig.peakPowerAssistLevels.value.turbo));
        this->fileSystem->closeFile();
        return true;
    }
    return false;
}

bool ConfigStore::readBTAddressMap() {
    this->btAddressesConfig.clear();
    if (this->fileSystem->openFile(this->knownDevicesFilename, "r")) {
        uint16_t addressesSize = 0;
        if (!this->readUInt16(&addressesSize)) return false;
        for (uint16_t i = 0; i < addressesSize; i++) {
            String addressString;
            String displayValueString;
            if (!this->readString(&addressString)) return false;
            if (!this->readString(&displayValueString)) return false;
            this->btAddressesConfig.addBTAddress(addressString ,displayValueString);
        }
        this->fileSystem->closeFile();
        return true;
    }
    return false;
}

bool ConfigStore::writeBTAddressMap() {
    if (this->fileSystem->openFile(this->knownDevicesFilename, "w")) {
        uint16_t addressesSize = this->btAddressesConfig.countBTAddresses();
        if (!this->writeUInt16(&addressesSize)) return false;
        for (std::unordered_map<String, String>::iterator it = this->btAddressesConfig.btAddressMap.begin() ; it != this->btAddressesConfig.btAddressMap.end(); it++) {
            String address = ((*it).first);
            String displayValue = ((*it).second);
            if (!this->writeString(&address)) return false;
            if (!this->writeString(&displayValue)) return false;
        }
        this->fileSystem->closeFile();
        return true;
    }
    return false;
}


bool ConfigStore::readUInt16(uint16_t* value) {
    return this->fileSystem->readFile(value, sizeof(uint16_t), 1);
}

bool ConfigStore::readUInt8(uint8_t* value) {
    return this->fileSystem->readFile(value, sizeof(uint8_t), 1);
}

bool ConfigStore::readBool(bool* value) {
    uint8_t int8Value;
    if (this->readUInt8(&int8Value)) {
        *value = int8Value;
        return true;
    }
    return false;
}

bool ConfigStore::writeUInt16(uint16_t* value) {
    return this->fileSystem->writeFile(value, sizeof(uint16_t), 1);
}

bool ConfigStore::writeUInt8(uint8_t* value) {
    return this->fileSystem->writeFile(value, sizeof(int8_t), 1);
}

bool ConfigStore::writeBool(bool* value) {
    uint8_t int8Value = *value ? 0x01 : 0x00;
    return this->writeUInt8(&int8Value);
}

bool ConfigStore::readString(String* string) {
    uint16_t stringSize;
    if (!this->readUInt16(&stringSize)) return false;
    char* readBuffer = (char*) malloc(stringSize + 1);
    if (!this->fileSystem->readFile(readBuffer, stringSize, 1))
    {
        free(readBuffer);
        return false;
    }
    readBuffer[stringSize] = 0x00;
    *string = readBuffer;
    free(readBuffer);
    return true;
}

bool ConfigStore::writeString(String* string) {
    uint16_t stringSize = string->length();
    if (!this->writeUInt16(&stringSize)) return false;
    return this->fileSystem->writeFile(string->c_str(), stringSize, 1);
}