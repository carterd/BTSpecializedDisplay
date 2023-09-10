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
}

void ConfigStore::writeDefaults() {
    this->defaults();
    if (this->writeBTAddressMap() && this->writeBikeConfig() && this->writeDisplayConfig()) {
    } else {
        LV_LOG_ERROR("Error writing config files.");
    }
}

void ConfigStore::defaults() {
    this->btAddressesConfig.clear();
    this->bikeConfig.beeper = BikeConfigAttr<bool>(false, false);                   // beeper 0 No beeping, 1 beeps
    this->bikeConfig.fakeChannel = BikeConfigAttr<uint8_t>(0, false);                // fakeChannel
    this->bikeConfig.wheelCircumference = BikeConfigAttr<uint16_t>(2293, false);     // Wheel Circumf start at 2293 mm
    this->bikeConfig.supportAssistLevels = BikeConfigAttr<AssistLevels>({10,40,100}, false); // assistLevelEco Should be something like 5,20,25,100 et-al
    this->bikeConfig.peakPowerAssistLevels = BikeConfigAttr<AssistLevels>({25,100,100}, false); // peakAssistEco Should be just 0-100

    this->displayConfig.contrast = 0x02;  // Between 0x00 and 0x02
    this->displayConfig.monitorType = 0x0000;
    this->displayConfig.connectBatteryOnly = false;
    this->displayConfig.connectOnBoot = false;
    this->displayConfig.unitsMetric = true;

    this->displayConfig.bikeAssistPresets.clear();
    this->displayConfig.bikeAssistPresets.push_back(BikeAssistPreset(PRESET_NAME_DEFAULT, AssistLevels(20, 50, 100), AssistLevels(50, 100, 100)));
    this->displayConfig.bikeAssistPresets.push_back(BikeAssistPreset(PRESET_NAME_ECO,     AssistLevels(10, 20, 50),  AssistLevels(10, 50, 50)));
    this->displayConfig.bikeAssistPresets.push_back(BikeAssistPreset(PRESET_NAME_THRASH,  AssistLevels(50, 80, 100),  AssistLevels(50, 100, 100)));
}

void ConfigStore::updateBTAddressesConfig(BTAddressesConfig& btAddressesConfig)
{
    // Ensure that btAddressConfig has changed before write
    if (this->btAddressesConfig.btAddressMap == btAddressesConfig.btAddressMap) {
        return;
    }
    this->btAddressesConfig = btAddressesConfig;
    if (this->writeBTAddressMap()) {        
    } else {
        LV_LOG_ERROR("Error writing config files.");
    }
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
    if (this->writeBikeConfig()) {
    } else {
        LV_LOG_ERROR("Error writing config files.");
    }
}

BikeConfig& ConfigStore::getBikeConfig() {
    return this->bikeConfig;
}

void ConfigStore::updateDisplayConfig(DisplayConfig& displayConfig) {
    if (this->displayConfig == displayConfig) {
        LV_LOG_USER("No change in display config");
        return;
    }
    this->displayConfig = displayConfig;
    if (this->writeDisplayConfig()) {
    } else {
        LV_LOG_ERROR("Error writing config files.");
    }
}

DisplayConfig& ConfigStore::getDisplayConfig() {
    return this->displayConfig;
}

bool ConfigStore::readDisplayConfig() {
    LV_LOG_USER("readDisplayConfig");
    if (this->fileSystem->openFile(this->displayConfigFilename, "r")) {
        bool result = true;
        result &= this->readUInt8(&(this->displayConfig.contrast));
        result &= this->readUInt16(&(this->displayConfig.monitorType));
        result &= this->readBool(&(this->displayConfig.connectBatteryOnly));
        result &= this->readBool(&(this->displayConfig.connectOnBoot));
        result &= this->readBool(&(this->displayConfig.unitsMetric));
        result &= this->readBikeAssistPresets(&(this->displayConfig.bikeAssistPresets));        
        this->fileSystem->closeFile();
        LV_LOG_USER("assistPresets (number of presets = %d)", this->displayConfig.bikeAssistPresets.size());
        LV_LOG_USER("read complete (result = %d)", result);
        return result;
    }
    LV_LOG_USER("unable to open config");
    return false;
}

bool ConfigStore::writeDisplayConfig() {
    LV_LOG_USER("writeDisplayConfig");
    if (this->fileSystem->openFile(this->displayConfigFilename, "w")) {
        bool result = true;
        result &= this->writeUInt8(&(this->displayConfig.contrast));
        result &= this->writeUInt16(&(this->displayConfig.monitorType));
        result &= this->writeBool(&(this->displayConfig.connectBatteryOnly));
        result &= this->writeBool(&(this->displayConfig.connectOnBoot));
        result &= this->writeBool(&(this->displayConfig.unitsMetric));
        result &= this->writeBikeAssistPresets(&(this->displayConfig.bikeAssistPresets));
        result &= this->fileSystem->closeFile();
        LV_LOG_USER("write complete (result = %d)", result);
        return result;
    }
    LV_LOG_USER("unable to open config");
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

bool ConfigStore::readSavesNames()
{
    this->savesNames.names.clear();
    if (! this->fileSystem->openDir(savesDirectoryName)) {
        return false;
    }

    char dirName[32];
    while (this->fileSystem->readDir(dirName, 32)) {
        this->savesNames.names.push_back(String(dirName));
    }
    
    this->fileSystem->closeDir();

    return true;
}

bool ConfigStore::readBikeConfig() {
    LV_LOG_USER("starting");
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
        LV_LOG_USER("read complete (result = %d)", result);
        return result;
    }
    LV_LOG_USER("unable to open config");
    return false;
}

bool ConfigStore::writeBikeConfig() {
    LV_LOG_USER("writeBikeConfig");
    if (this->fileSystem->openFile(this->bikeConfigFilename, "w")) {
        bool result = true;
        result &= this->writeBool(&(this->bikeConfig.beeper.managed));
        result &= this->writeBool(&(this->bikeConfig.beeper.value));
        result &= this->writeBool(&(this->bikeConfig.fakeChannel.managed));
        result &= this->writeUInt8(&(this->bikeConfig.fakeChannel.value));
        result &= this->writeBool(&(this->bikeConfig.wheelCircumference.managed));
        result &= this->writeUInt16(&(this->bikeConfig.wheelCircumference.value));
        result &= this->writeBool(&(this->bikeConfig.supportAssistLevels.managed));
        result &= this->writeUInt8(&(this->bikeConfig.supportAssistLevels.value.eco));
        result &= this->writeUInt8(&(this->bikeConfig.supportAssistLevels.value.trail));
        result &= this->writeUInt8(&(this->bikeConfig.supportAssistLevels.value.turbo));
        result &= this->writeBool(&(this->bikeConfig.peakPowerAssistLevels.managed));
        result &= this->writeUInt8(&(this->bikeConfig.peakPowerAssistLevels.value.eco));
        result &= this->writeUInt8(&(this->bikeConfig.peakPowerAssistLevels.value.trail));
        result &= this->writeUInt8(&(this->bikeConfig.peakPowerAssistLevels.value.turbo));
        this->fileSystem->closeFile();
        LV_LOG_USER("write complete (result = %d)", result);
        return true;
    }
    LV_LOG_USER("unable to open config");
    return false;
}


bool ConfigStore::readBikeAssistPresets(std::vector<BikeAssistPreset>* bikeAssistPresets) {
    bool result = true;
    bikeAssistPresets->clear();
    
    // Get the size first
    uint16_t numBikeAssistPresets;
    result = this->readUInt16(&numBikeAssistPresets);
    // Now read in the presets
    for (int i = 0; (i < numBikeAssistPresets) && result; i++) {
        BikeAssistPreset bikeAssistPreset;
        result &= this->readString(&(bikeAssistPreset.Name));
        result &= this->readUInt8(&(bikeAssistPreset.supportAssistLevels.eco));
        result &= this->readUInt8(&(bikeAssistPreset.supportAssistLevels.trail));
        result &= this->readUInt8(&(bikeAssistPreset.supportAssistLevels.turbo));
        result &= this->readUInt8(&(bikeAssistPreset.peakPowerAssistLevels.eco));
        result &= this->readUInt8(&(bikeAssistPreset.peakPowerAssistLevels.trail));
        result &= this->readUInt8(&(bikeAssistPreset.peakPowerAssistLevels.turbo));
        bikeAssistPresets->push_back(bikeAssistPreset);
    }
    return result;
}

bool ConfigStore::writeBikeAssistPresets(std::vector<BikeAssistPreset>* bikeAssistPresets) {
    bool result = true;
    // Store the size first
    uint16_t numBikeAssistPresets = bikeAssistPresets->size();
    result = this->writeUInt16(&numBikeAssistPresets);
    for (auto it = bikeAssistPresets->begin(); (it != bikeAssistPresets->end()) && result; it++) {
        result &= this->writeString(&(it->Name));
        result &= this->writeUInt8(&(it->supportAssistLevels.eco));
        result &= this->writeUInt8(&(it->supportAssistLevels.trail));
        result &= this->writeUInt8(&(it->supportAssistLevels.turbo));
        result &= this->writeUInt8(&(it->peakPowerAssistLevels.eco));
        result &= this->writeUInt8(&(it->peakPowerAssistLevels.trail));
        result &= this->writeUInt8(&(it->peakPowerAssistLevels.turbo));
    }
    return result;
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