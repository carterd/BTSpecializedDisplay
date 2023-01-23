#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <ButtonEncoder.h>

#include <Arduino_LvGL_Glue.h>
#include <LvglDisplays/Adafruit_SH110X_LvGL_Display.h>
#include <LvglInputs/Arduino_ButtonEncoder_LvGL_Input.h>
//#include <DisplayCallbacks/DisplayCallback_SH110X.h>
//#include <InputCallbacks/InputCallback_ButtonEncoder.h>
#include <Fonts/PixelOperator8pt7b.h>
#include <LvglThemes/lv_theme_binary.h>
#include <LvglFonts/lv_font_symbols_8.h>
#include <Utilities/Adafruit_To_LvGL_Font.h>

#include "LedError.h"
#include "src/LvglGui.h"
#include "src/dev/BluetoothBikeController.h"

#define MIN_RESET_BUTTON_PRESS_TIME_MS 10 * 1000
#define ADAFRUIT_SH1107_RESET_D_PIN 20

#define _SERIAL_DEBUG 0

static ButtonEncoder encoder(16, 14, 15);
static Adafruit_SH1107 adafruitDisplay(64, 128, &Wire, ADAFRUIT_SH1107_RESET_D_PIN);

/**
 * @brief Adafruit displayGlue static
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

bool resetButtonTest() {
    unsigned long startTime = millis();
    do {
        unsigned long i =  (millis() - startTime) / (MIN_RESET_BUTTON_PRESS_TIME_MS / adafruitDisplay.width());
        encoder.update();
        if ((encoder.keyPressed() && encoder.lastKeyPressed() == Encoder::EncoderKey::ENCODER_ENTER)) { 
            adafruitDisplay.fillCircle(adafruitDisplay.width() / 2, adafruitDisplay.height() / 2, i, SH110X_WHITE);
            adafruitDisplay.display();
        }
    } while (encoder.keyPressed() && encoder.lastKeyPressed() == Encoder::EncoderKey::ENCODER_ENTER);
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
    bool displayWorking = false;
    while (!displayWorking) {
        displayWorking = adafruitDisplay.begin(0x3C, true); // Address 0x3C default
        digitalWrite(LED_BUILTIN, HIGH);            // turn the LED on (HIGH is the voltage level)
        if (!displayWorking) delay(2000);           // wait for a second
    };
    adafruitDisplay.clearDisplay();
    adafruitDisplay.display();

    // Initialise display glue

    Adafruit_SH110X_LvGL_Display lvglDisplay(&adafruitDisplay);
    Arduino_ButtonEncoder_LvGL_Input lvglInput(&encoder);
    LvGLStatus result = displayGlue.begin(&lvglDisplay, &lvglInput, true);
    if (result != LVGL_OK) {    
        led_error(result);
    }

    bluetoothBikeController = new BluetoothBikeController();
    configStore = new ConfigStore();
    bluetoothBikeController->init();
    configStore->init();

    if (resetButtonTest()) {
        configStore->defaults();
    }

    lv_disp_t *display = displayGlue.getLvDisplay();
    lv_indev_t *indev = displayGlue.getLvInputDevice();
  
    // initalise the font required for the given display
    adafruitToLvGLFont(&PixelOperator8pt7b, &myAdaFont, &lv_font_symbols_8);
  
    // initialise the binary theme
    lv_theme_t* binary_theme = lv_theme_binary_init(displayGlue.getLvDisplay(), true, &myAdaFont);
    lv_disp_set_rotation(NULL, LV_DISP_ROT_180);
    lv_disp_set_theme(display, binary_theme);

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
