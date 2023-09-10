#include <Arduino.h>
#include <Wire.h>

#include <Arduino_LvGL_Glue.h>
#include <ButtonEncoder.h>
#include "Hardware.h"

#include "LedError.h"
#include "src/LvglGui.h"
#include "src/dev/BluetoothController.h"
#include "src/dev/BluetoothBike.h"
#include "src/dev/BluetoothHeartRateMonitor.h"
#include "src/dev/BluetoothScanner.h"

#define MIN_RESET_BUTTON_PRESS_TIME_MS 10 * 1000
#define _SERIAL_DEBUG 1

/**
 * @brief Arduino displayGlue static
 */
static Arduino_LvGL_Glue displayGlue;

/**
 * @brief Statics kept around
 * 
 */
BluetoothScanner* bluetoothScanner;
BluetoothBike* bluetoothBike;
BluetoothHeartRateMonitor* bluetoothHeartRateMonitor;
BluetoothController* bluetoothController;
ConfigStore* configStore;

bool resetButtonTest(Encoder* encoder) {
    // Specific de-bounce for two button encoders
    encoder->update();
    unsigned long startTime = millis();
    do {
        unsigned long radius =  (millis() - startTime) / (MIN_RESET_BUTTON_PRESS_TIME_MS / DISPLAY_WIDTH);
        encoder->update();
        if ((encoder->keyPressed() && encoder->lastKeyPressed() == Encoder::EncoderKey::ENCODER_RIGHT)) {
            displayfillCircle(radius);      
        }
    } while (encoder->keyPressed() && encoder->lastKeyPressed() == Encoder::EncoderKey::ENCODER_RIGHT);
    if (millis() - startTime > MIN_RESET_BUTTON_PRESS_TIME_MS) return true;
    return false;
}

void setup() {
    // Allow time for display to wake up
    delay(250);
    pinMode(LED_BUILTIN, OUTPUT);

#if _SERIAL_DEBUG == 1
    // Remote if no serial monitor
    Serial.begin(115200);
    while (!Serial);
#endif

    if (!BLE.begin()) {
        //Serial.println("starting BLE failed!");
        while (1);
    }

    // Initialise display
    LvGL_DisplayBase* lvglDisplay = displayInit();

    // Initialise the encoder
    Arduino_ButtonEncoder_LvGL_Input* lvglInput = encoderInit();

    // Initialise display glue
    LvGLStatus result = displayGlue.begin(lvglDisplay, lvglInput, true);
    if (result != LVGL_OK) {    
        led_error(result);
    }
    
    bluetoothScanner = new BluetoothScanner();
    bluetoothBike = new BluetoothBike();
    bluetoothHeartRateMonitor = new BluetoothHeartRateMonitor();
    bluetoothController = new BluetoothController(bluetoothBike, bluetoothHeartRateMonitor, bluetoothScanner, BLE_CONNECTION_REQUIRES_SCAN_STOP);
    
    configStore = new ConfigStore(FS_FILE_PREFIX FS_SEPARATOR "knownDevices.bin", FS_FILE_PREFIX FS_SEPARATOR "bikeConfig.bin", FS_FILE_PREFIX FS_SEPARATOR "displayConfig.bin", FS_FILE_PREFIX FS_SEPARATOR "saves");

    static FileSystem fileSystem;
    configStore->init(&fileSystem);

    if (resetButtonTest(lvglInput->getButtonEncoder())) {
        configStore->writeDefaults();
    }
    
    if (configStore->readBTAddressMap() && configStore->readBikeConfig() && configStore->readDisplayConfig() && configStore->savesConfig()) {
    } else {
        LV_LOG_ERROR("Error reading config files, initialise default config.");
        configStore->writeDefaults();
    }

    lv_disp_t *display = displayGlue.getLvDisplay();
    lv_indev_t *indev = displayGlue.getLvInputDevice();

    // initialise the binary theme
    lv_theme_t* theme = themeInit(displayGlue.getLvDisplay());
    //lv_theme_t* binary_theme = lv_theme_binary_init(displayGlue.getLvDisplay(), true, &myAdaFont);
    //lv_theme_t* default_theme = lv_theme_default_init(displayGlue.getLvDisplay(), lv_color_make(0xff,0xff,0xff), lv_color_make(0xff,0x80,0x80), true, &lv_font_montserrat_28);

    lv_disp_set_rotation(NULL, LV_DISP_ROT_180);
    lv_disp_set_theme(display, theme);
    //lv_disp_set_theme(display, default_theme);

    lvgl_setup(configStore, bluetoothController, bluetoothBike, bluetoothHeartRateMonitor, bluetoothScanner, displayGlue, indev);
}

void loop() {

  // put your main code here, to run repeatedly:
  lv_task_handler(); // Call LittleVGL task handler periodically
  unsigned long start = millis();
  
  bluetoothBike->checkForChange();
  bluetoothScanner->checkForChange();

  unsigned long end = millis();
  if (start <= end && end - start < 20) {
    delay(20 + start - end);
  }
}
