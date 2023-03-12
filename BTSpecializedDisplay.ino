#include <Arduino.h>
#include <Wire.h>

#include <Arduino_LvGL_Glue.h>
#include <ButtonEncoder.h>
#include "Hardware.h"

#include <Fonts/PixelOperator8pt7b.h>
#include <LvglThemes/lv_theme_binary.h>
#include <LvglFonts/lv_font_symbols_8.h>
#include <Utilities/Adafruit_To_LvGL_Font.h>

#include "LedError.h"
#include "src/LvglGui.h"
#include "src/dev/BluetoothBikeController.h"

#define MIN_RESET_BUTTON_PRESS_TIME_MS 10 * 1000
#define _SERIAL_DEBUG 1

/**
 * @brief Arduino displayGlue static
 */
static Arduino_LvGL_Glue displayGlue;
/**
 * @brief lv_font generated from an Adafruit font static
 */
lv_font_t myAdaFont = {};
/**
 * @brief Statics kept around
 * 
 */
BluetoothBikeController* bluetoothBikeController;
ConfigStore* configStore;

bool resetButtonTest(Encoder* encoder) {
    unsigned long startTime = millis();
    do {
        unsigned long radius =  (millis() - startTime) / (MIN_RESET_BUTTON_PRESS_TIME_MS / DISPLAY_WIDTH);
        encoder->update();
        if ((encoder->keyPressed() && encoder->lastKeyPressed() == Encoder::EncoderKey::ENCODER_ENTER)) {
            displayfillCircle(radius);      
        }
    } while (encoder->keyPressed() && encoder->lastKeyPressed() == Encoder::EncoderKey::ENCODER_ENTER);
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

    // Initialise display
    LvGL_DisplayBase* lvglDisplay = displayInit();

    // Initialise the encoder
    Arduino_ButtonEncoder_LvGL_Input* lvglInput = encoderInit();

    // Initialise display glue
    LvGLStatus result = displayGlue.begin(lvglDisplay, lvglInput, true);
    if (result != LVGL_OK) {    
        led_error(result);
    }

    bluetoothBikeController = new BluetoothBikeController();
    configStore = new ConfigStore(FS_FILE_PREFIX FS_SEPARATOR "knownDevices.bin", FS_FILE_PREFIX FS_SEPARATOR "bikeConfig.bin", FS_FILE_PREFIX FS_SEPARATOR "displayConfig.bin", FS_FILE_PREFIX FS_SEPARATOR "saves");

    bluetoothBikeController->init();

    static FileSystem fileSystem;
    configStore->init(&fileSystem);

    if (resetButtonTest(lvglInput->getButtonEncoder())) {
        configStore->defaults();
    }

    lv_disp_t *display = displayGlue.getLvDisplay();
    lv_indev_t *indev = displayGlue.getLvInputDevice();

    // initalise the font required for the given display
    adafruitToLvGLFont(&PixelOperator8pt7b, &myAdaFont, &lv_font_symbols_8);

    // initialise the binary theme
    lv_theme_t* binary_theme = lv_theme_binary_init(displayGlue.getLvDisplay(), true, &myAdaFont);
    //lv_theme_t* default_theme = lv_theme_default_init(displayGlue.getLvDisplay(), lv_color_make(0xff,0xff,0xff), lv_color_make(0xff,0x80,0x80), true, &lv_font_montserrat_28);

    lv_disp_set_rotation(NULL, LV_DISP_ROT_180);
    lv_disp_set_theme(display, binary_theme);
    //lv_disp_set_theme(display, default_theme);

    lvgl_setup(configStore, bluetoothBikeController, displayGlue, indev);
}

void loop() {
  // put your main code here, to run repeatedly:
  lv_task_handler(); // Call LittleVGL task handler periodically
  unsigned long start = millis();
  
  bluetoothBikeController->checkForChange();

  unsigned long end = millis();
  if (start <= end && end - start < 20) {
    delay(20 + start - end);
  }
}
