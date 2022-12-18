#ifndef  _CONFIG_STORE_H
#define  _CONFIG_STORE_H

#include <Arduino.h>
#include <unordered_map>

#include "BluetoothBikeState.h"

#define UNMANAGED_CONFIG -1

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

#include "..\config\DisplayConfig.h"
#include "..\config\BikeConfig.h"
#include "..\config\BTAddressConfig.h"
#include "..\config\SavesNames.h"


class LittleFS_MBED;

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
    const char* savesDirectoryName;

    BTAddressesConfig btAddressesConfig;
    DisplayConfig displayConfig;
    BikeConfig bikeConfig;
    SavesNames savesNames;

    bool readUInt16(FILE* file, uint16_t* value);
    bool readUInt8(FILE* file, uint8_t* value);
    bool readBool(FILE* file, bool* value);

    bool writeUInt16(FILE* file, uint16_t* integer);
    bool writeUInt8(FILE* file, uint8_t* integer);
    bool writeBool(FILE* file, bool* value);

    bool readString(FILE* file, String* string);
    bool writeString(FILE* file, String* string);
    /// <summary>
    /// Read the Bluetooth addresses that are accepted for connections to the fs
    /// </summary>
    /// <returns>True if the config was read without errors</returns>
    bool readBTAddressMap();
    /// <summary>
    /// Write the Bluetooth addresses that are accepted for connections to the fs
    /// </summary>
    /// <returns>Returns true if no errors encountered writting the config</returns>
    bool writeBTAddressMap();
    /// <summary>
    /// Read the bike config form the fs
    /// </summary>
    /// <returns>True if the config was read without errors</returns>    
    bool readBikeConfig();
    /// <summary>
    /// Write the Bike config to the fs
    /// </summary>
    /// <returns>Returns true if no errors encountered writting the config</returns>
    bool writeBikeConfig();
    /// <summary>
    /// Read the display config structure from the fs
    /// </summary>
    /// <returns>Returns true if no errors encountered reading the config</returns>
    bool readDisplayConfig();
    /// <summary>
    /// This will write the display config structure to the fs
    /// </summary>
    /// <returns></returns>
    bool writeDisplayConfig();
    /// <summary>
    /// Ensures the directory for the saves exist
    /// </summary>
    /// <returns>will always return true not much you can do if the saves directory can't be created</returns>
    bool savesConfig();
    
    bool ReadSavesNames();
public:
    /// <summary>
    /// Constructor of the config store for the device
    /// </summary>
    ConfigStore();
    /// <summary>
    /// Initialise the File System
    /// </summary>
    void init();
    /// <summary>
    /// This will ensure all the configuration settings are set to their defaults
    /// </summary>
    void defaults();
    /// <summary>
    /// Update the configuration on the FS with the given address configurations
    /// </summary>
    /// <param name="btAddressesConfig">The structure containing all the valid bluetooth addresses</param>
    void updateBTAddressesConfig(BTAddressesConfig& btAddressesConfig);
    /// <summary>
    /// Get the current configuration for Bluetooth addresses to connect to
    /// </summary>
    /// <returns></returns>
    BTAddressesConfig& getBTAddressesConfig();
    /// <summary>
    /// Update the bike config on the FS with the given bike configuration
    /// </summary>
    /// <param name="bikeConfig"></param>
    void updateBikeConfig(BikeConfig& bikeConfig);
    /// <summary>
    /// Get the bike config form the FS
    /// </summary>
    /// <returns></returns>
    BikeConfig& getBikeConfig();
    /// <summary>
    /// Update the display configuration on the FS
    /// </summary>
    /// <param name="bikeConfig"></param>
    void updateDisplayConfig(DisplayConfig& bikeConfig);
    /// <summary>
    /// Returns the current stored display configuration form the FS
    /// </summary>
    /// <returns></returns>
    DisplayConfig& getDisplayConfig();
};

#endif