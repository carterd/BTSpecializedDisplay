#ifndef _DISPLAY_DEVICE_H_
#define _DISPLAY_DEVICE_H_

#include <Arduino_LvGL_Glue.h>
#include <LvglInputs/Arduino_ButtonEncoder_LvGL_Input.h>
#include "src/dev/ConfigStore.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Config of the Display under the RP2040 which will be ADAFRUIT_SH110X
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef ARDUINO_ARCH_RP2040

// Bluetooth
//
#define BLE_CONNECTION_REQUIRES_SCAN_STOP true

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

#include <Fonts/PixelOperator8pt7b.h>
#include "src/themes/lv_theme_SH1107.h"
#include <LvglFonts/lv_font_symbols_8.h>
#include <Utilities/Adafruit_To_LvGL_Font.h>

lv_theme_t* themeInit(lv_disp_t *disp) {
    static lv_font_t myAdaFont = {};
    adafruitToLvGLFont(&PixelOperator8pt7b, &myAdaFont, &lv_font_symbols_8);
    return lv_theme_sh1107_init(disp, true, &myAdaFont);
}

void displayfillCircle(unsigned long radius) {
    displayDevice.fillCircle(displayDevice.width() / 2, displayDevice.height() / 2, radius, SH110X_WHITE);
    displayDevice.display();
}

void displaySetContrast(void *display, int contrastLevel) {
    switch (contrastLevel) {
        case 0:
            contrastLevel = 0x01;
            break;
        case 1:
            contrastLevel = 0x14;
            break;
        case 2:
            contrastLevel = 0x2F;
            break;
    }
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

bool FileSystem::readDir(char* fileName, int maxFileName) {
    static struct dirent* pDirent;
    if ((pDirent = readdir((DIR*) (this->dir))) == NULL) {
        return false;
    }
    strncpy(fileName, pDirent->d_name, maxFileName-1);
    fileName[maxFileName-1] = 0x00;
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
    return true;
}

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
// Config of the Display for T-DISPLAY
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef ARDUINO_TTGO_LoRa32_V1

// Encoder
//
#include <TwoButtonEncoder.h>
#define ENCODER_LEFT_PIN 0
#define ENCODER_RIGHT_PIN 35
#define ENCODER_PRESSED_STATE LOW
#define ENCODER_BUTTON_PIN_MODE INPUT_PULLUP
static TwoButtonEncoder encoder(ENCODER_LEFT_PIN, ENCODER_RIGHT_PIN, ENCODER_PRESSED_STATE, ENCODER_BUTTON_PIN_MODE);

Arduino_ButtonEncoder_LvGL_Input* encoderInit() {
    static Arduino_ButtonEncoder_LvGL_Input lvglInput(&encoder);
    return &lvglInput;
}

// Bluetooth
//
#define BLE_CONNECTION_REQUIRES_SCAN_STOP false

// Display
//
#include "TFT_eSPI.h"
#include <LvglDisplays/TFTESPI_LvGL_Display.h>
#define DISPLAY_WIDTH 135
#define DISPLAY_HEIGHT 240
static TFT_eSPI displayDevice(DISPLAY_WIDTH, DISPLAY_HEIGHT);

LvGL_DisplayBase* displayInit() {
    displayDevice.init();
    displayDevice.setRotation(0);
    displayDevice.setSwapBytes(true);
    displayDevice.fillScreen(TFT_BLACK);

    pinMode(TFT_BL, OUTPUT);
    ledcSetup(0, 5000, 8);
    ledcAttachPin(TFT_BL, 0);
    ledcWrite(0, 255);

    static TFTESPI_LvGL_Display lvglDisplay(&displayDevice);
    return &lvglDisplay;
}

void displaySetContrast(void *display, int contrastLevel) {
    switch (contrastLevel) {
        case 0:
            contrastLevel = 0x08;
            break;
        case 1:
            contrastLevel = 0x7F;
            break;
        case 2:
            contrastLevel = 0xFF;
            break;
    }
    ledcWrite(0, contrastLevel);
}

#include <Fonts/PixelOperator8pt7b.h>
#include "src/themes/lv_theme_TDISPLAY.h"
#include <Utilities/Adafruit_To_LvGL_Font.h>
#include <font/lv_font.h>

//lv_theme_t* themeInit(lv_disp_t *disp) {
//    return lv_theme_default_init(disp, lv_color_make(0xff,0xff,0xff), lv_color_make(0xff,0x80,0x80), true, &lv_font_montserrat_28);
//}
lv_theme_t* themeInit(lv_disp_t *disp) {
    return lv_theme_tdisplay_init(disp, true, &lv_font_montserrat_20);
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
    if (this->dir) { 
        delete((fs::File*) this->dir); 
        this->dir = NULL;
    }
    fs::File* dirFile = new fs::File(((fs::LittleFSFS*)(this->fileSystem))->open(dirPath));
    if (*dirFile && dirFile->isDirectory()) {
        this->dir = dirFile;
        return true;
    }
    LV_LOG_ERROR("unable to openDir '%s'", dirPath);
    delete(dirFile);
    this->dir = NULL;
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

bool FileSystem::readDir(char* fileName, int maxFileName) {
    if (this->dir) {
        fs::File nextFile = ((fs::File*) (this->dir))->openNextFile();
        if (nextFile) {
            strncpy(fileName, nextFile.path(), maxFileName-1);
            fileName[maxFileName-1] = 0x00;
            return true;
        }
        return false;
    } else {
        return false;
    }
}

bool FileSystem::openFile(const char* filePath, const char* mode) {
    if (this->file) { 
        delete((fs::File*) this->file); 
        this->file = NULL;
    }
    fs::File* fileFile = new fs::File(((fs::LittleFSFS*)(this->fileSystem))->open(filePath, mode));

    if (!*fileFile || fileFile->isDirectory()) {
        LV_LOG_ERROR("unable to openFile '%s' mode '%s'", filePath, mode);
        delete(fileFile);
        this->file = NULL;
        return false;
    }
    this->file = fileFile;
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
    delete((fs::File*) this->file); 
    this->file = NULL;
    return false;
}

#endif


///////////////////////////////////////////////////////////////////////////////////////////////////
// Config of the Display for T-DISPLAY
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef ARDUINO_ESP32S3_DEV

// Bluetooth
//
#define BLE_CONNECTION_REQUIRES_SCAN_STOP false

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

    pinMode(TFT_BL, OUTPUT);
    ledcSetup(0, 5000, 8);
    ledcAttachPin(TFT_BL, 0);
    ledcWrite(0, 255);

    static TFTESPI_LvGL_Display lvglDisplay(&displayDevice);
    return &lvglDisplay;
}

void displaySetContrast(void *display, int contrastLevel) {
    switch (contrastLevel) {
        case 0:
            contrastLevel = 0x08;
            break;
        case 1:
            contrastLevel = 0x7F;
            break;
        case 2:
            contrastLevel = 0xFF;
            break;
    }
    ledcWrite(0, contrastLevel);
}

#include <Fonts/PixelOperator8pt7b.h>
#include "src/themes/lv_theme_TDISPLAY_S3.h"
#include <Utilities/Adafruit_To_LvGL_Font.h>
#include <font/lv_font.h>

//lv_theme_t* themeInit(lv_disp_t *disp) {
//    return lv_theme_default_init(disp, lv_color_make(0xff,0xff,0xff), lv_color_make(0xff,0x80,0x80), true, &lv_font_montserrat_28);
//}
lv_theme_t* themeInit(lv_disp_t *disp) {
    return lv_theme_tdisplay_s3_init(disp, true, &lv_font_montserrat_20);
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
    if (this->dir) { 
        delete((fs::File*) this->dir); 
        this->dir = NULL;
    }
    fs::File* dirFile = new fs::File(((fs::LittleFSFS*)(this->fileSystem))->open(dirPath));
    if (*dirFile && dirFile->isDirectory()) {
        this->dir = dirFile;
        return true;
    }
    LV_LOG_ERROR("unable to openDir '%s'", dirPath);
    delete(dirFile);
    this->dir = NULL;
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

bool FileSystem::readDir(char* fileName, int maxFileName) {
    if (this->dir) {
        fs::File nextFile = ((fs::File*) (this->dir))->openNextFile();
        if (nextFile) {
            strncpy(fileName, nextFile.path(), maxFileName-1);
            fileName[maxFileName-1] = 0x00;
            return true;
        }
        return false;
    } else {
        return false;
    }
}

bool FileSystem::openFile(const char* filePath, const char* mode) {
    if (this->file) { 
        delete((fs::File*) this->file); 
        this->file = NULL;
    }
    fs::File* fileFile = new fs::File(((fs::LittleFSFS*)(this->fileSystem))->open(filePath, mode));

    if (!*fileFile || fileFile->isDirectory()) {
        LV_LOG_ERROR("unable to openFile '%s' mode '%s'", filePath, mode);
        delete(fileFile);
        this->file = NULL;
        return false;
    }
    this->file = fileFile;
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
    delete((fs::File*) this->file); 
    this->file = NULL;
    return false;
}

#endif




#endif