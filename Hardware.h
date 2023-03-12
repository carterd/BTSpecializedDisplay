#ifndef _DISPLAY_DEVICE_H_
#define _DISPLAY_DEVICE_H_

#include <Arduino_LvGL_Glue.h>
#include <LvglInputs/Arduino_ButtonEncoder_LvGL_Input.h>
#include "src/dev/ConfigStore.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Config of the Display under the RP2040 which will be ADAFRUIT_SH110X
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef ARDUINO_ARCH_RP2040

// Encoder
//
#include <ThreeButtonEncoder.h>
#define ENCODER_LEFT_PIN 16
#define ENCODER_RIGHT_PIN 14
#define ENCODER_SELECT_PIN 15
#define ENCODER_PRESSED_STATE LOW
#define ENCODER_BUTTON_PIN_MODE INPUT_PULLUP
static ThreeButtonEncoder encoder(ENCODER_LEFT_PIN, ENCODER_RIGHT_PIN, ENCODER_SELECT_PIN, ENCODER_PRESSED_STATE, ENCODER_BUTTON_PIN_MODE);

Arduino_ButtonEncoder_LvGL_Input* encoderInit() {
    static Arduino_ButtonEncoder_LvGL_Input lvglInput(&encoder);
    return &lvglInput;
}

// Display Stuff
//
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

void displaySetContrast(void *display, int contrastLevel) {
    ((Adafruit_SH1107*) display)->setContrast(contrastLevel);
}

// File System
//

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

///////////////////////////////////////////////////////////////////////////////////////////////////
// Config of the Display for T-DISPLAY
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef ESP32

// Encoder
//
#include <TwoButtonEncoder.h>
#define ENCODER_LEFT_PIN 0
#define ENCODER_RIGHT_PIN 14
#define ENCODER_PRESSED_STATE LOW
#define ENCODER_BUTTON_PIN_MODE INPUT_PULLUP
static TwoButtonEncoder encoder(ENCODER_LEFT_PIN, ENCODER_RIGHT_PIN, ENCODER_PRESSED_STATE, ENCODER_BUTTON_PIN_MODE);

Arduino_ButtonEncoder_LvGL_Input* encoderInit() {
    static Arduino_ButtonEncoder_LvGL_Input lvglInput(&encoder);
    return &lvglInput;
}

// Display
//
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

// File System
//
#define FORMAT_LITTLEFS_IF_FAILED true
#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>
#define FS_FILE_PREFIX ""
#define FS_SEPARATOR "/"

bool FileSystem::init() {    
    this->fileSystem = &LittleFS;
    if (!((fs::LittleFSFS*)(this->fileSystem))->begin(FORMAT_LITTLEFS_IF_FAILED)) {
        return false;
    }
    return true;
}
bool FileSystem::openDir(const char* dirPath) {    
    static fs::File dirFile = ((fs::LittleFSFS*)(this->fileSystem))->open(dirPath);
    if (dirFile && dirFile.isDirectory()) {
        this->dir = &dirFile;
        return true;
    }
    return false;
}
bool FileSystem::closeDir() {
    if (this->dir) {
        ((fs::File*)(this->dir))->close();
        this->dir = NULL;
        return true;
    }
    return false;
}
bool FileSystem::mkDir(const char* dirPath) {
    if (((fs::LittleFSFS*)(this->fileSystem))->mkdir(dirPath)) return true;
    return false;
}

bool FileSystem::readDir(char** fileName) {
    if (this->dir) {
        static fs::File nextFile = ((fs::File*) (this->dir))->openNextFile();
        if (nextFile) {
            *fileName = (char*) nextFile.path();
            return true;
        }
        return false;
    } else {
        return false;
    }
}
bool FileSystem::openFile(const char* filePath, const char* mode) {
    static fs::File fileFile = ((fs::LittleFSFS*)(this->fileSystem))->open(filePath, mode);
    if (!fileFile || fileFile.isDirectory()) {
        this->file = NULL;
        return false;
    }
    this->file = &fileFile;    
    return true;
}
bool FileSystem::readFile(void* buffer, size_t size, size_t n) {    
    return ((fs::File*)(this->file))->read((uint8_t*)buffer, size*n) == size*n;
}
bool FileSystem::writeFile(const void* buffer, size_t size, size_t n) {
    return ((fs::File*)(this->file))->write((uint8_t*)buffer, size*n) == size*n;
}
bool FileSystem::closeFile() {
    ((fs::File*)(this->file))->close();
}

void displaySetContrast(void *display, int contrastLevel) {
    
}

#endif




#endif