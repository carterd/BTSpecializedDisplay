#ifndef _DISPLAY_DEVICE_H_
#define _DISPLAY_DEVICE_H_

#include <Arduino_LvGL_Glue.h>
#include "src/dev/ConfigStore.h"

// Config of the Display under the RP2040 which will be ADAFRUIT_SH110X
//
#ifdef ARDUINO_ARCH_RP2040

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <LvglDisplays/Adafruit_SH110X_LvGL_Display.h>
#define ADAFRUIT_SH1107_RESET_D_PIN 20
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 128
static Adafruit_SH1107 displayDevice(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, ADAFRUIT_SH1107_RESET_D_PIN);

LvGL_DisplayBase* displayInit() {
    bool displayWorking = false;
    while (!displayWorking) {
        displayWorking = displayDevice.begin(0x3C, true); // Address 0x3C default
        digitalWrite(LED_BUILTIN, HIGH);            // turn the LED on (HIGH is the voltage level)
        if (!displayWorking) delay(2000);           // wait for a second
    };
    displayDevice.clearDisplay();
    displayDevice.display();

    static Adafruit_SH110X_LvGL_Display lvglDisplay(&displayDevice);
    return &lvglDisplay;
}

void displayfillCircle(unsigned long radius) {
    displayDevice.fillCircle(displayDevice.width() / 2, displayDevice.height() / 2, radius, SH110X_WHITE);
    displayDevice.display();
}

#define FORCE_REFOMAT false;
#define RP2040_FS_SIZE_KB 64
#define LFS_MBED_RP2040_DEBUG 1
#define _LFS_LOGLEVEL_ 100
#include <Arduino.h>
#include <LittleFS_Mbed_RP2040.h>
#define FS_FILE_PREFIX MBED_LITTLEFS_FILE_PREFIX
#define FS_SEPARATOR "/"

bool FileSystem::init() {
    this->fileSystem = new LittleFS_MBED();
    if (!((LittleFS_MBED*)(this->fileSystem))->init()) {
        return false;
    }
    return true;
}
bool FileSystem::openDir(const char* dirPath) {
    this->dir = opendir(dirPath);
    if (this->dir) return true;
    return false;
}
bool FileSystem::closeDir() {
    if (this->dir) {
        closedir((DIR*)this->dir);
        return true;
    }
    return false;
}
bool FileSystem::mkDir(const char* dirPath) {
    if (mkdir(dirPath, S_IRWXU | S_IRWXG | S_IRWXO) == 0) return true;
    return false;
}

bool FileSystem::readDir(char** fileName) {
    static struct dirent* pDirent;
    if ((pDirent = readdir((DIR*) (this->dir))) == NULL) {
        return false;
    }
    *fileName = pDirent->d_name;
    return true;
}
bool FileSystem::openFile(const char* filePath, const char* mode) {
    this->file = fopen(filePath, mode);
    if (this->file) return true;
    return false;
}
bool FileSystem::readFile(void* buffer, size_t size, size_t n) {
    return fread(buffer, size, n, (FILE*) (this->file)) == n;
}
bool FileSystem::writeFile(const void* buffer, size_t size, size_t n) {
    return fwrite(buffer, size, n, (FILE*) (this->file)) == n;
}
bool FileSystem::closeFile() {
    fclose((FILE*)this->file);
}


#endif
//
// End of Config for RP2040 and ADAFRUIT_SH110X

// Config of the Display for T-DISPLAY
//
#ifdef ESP32
#include "TFT_eSPI.h"
#include <LvglDisplays/TFTESPI_LvGL_Display.h>
#define DISPLAY_WIDTH 170
#define DISPLAY_HEIGHT 320
static TFT_eSPI displayDevice(DISPLAY_WIDTH, DISPLAY_HEIGHT);

LvGL_DisplayBase* displayInit() {
    displayDevice.init();
    displayDevice.setRotation(0);
    displayDevice.setSwapBytes(true);
    displayDevice.fillScreen(TFT_BLACK);

    static TFTESPI_LvGL_Display lvglDisplay(&displayDevice);
    return &lvglDisplay;
}

void displayfillCircle(unsigned long radius) {
    displayDevice.fillCircle(displayDevice.width() / 2, displayDevice.height() / 2, radius, TFT_WHITE);
}

#define FORMAT_LITTLEFS_IF_FAILED true
#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>
#define FS_FILE_PREFIX ""
#define FS_SEPARATOR "/"

void* fsInit() {
    if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
        return NULL;
    }
    return &LittleFS;
}



#endif




#endif