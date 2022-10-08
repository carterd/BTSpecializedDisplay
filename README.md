# Bluetooth Specialized Display (https://https://github.com/carterd/BTSpecializedDisplay)

Ebike display for Specialized Turbo bikes, these include Kenevo/Levo/Creo.

# Dependencies
 
 * [LittlevGL](https://github.com/littlevgl/lvgl)
 * [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
 * [Adafruit SH110X Library](https://github.com/adafruit/Adafruit_SH110X)
 * [Adafruit LvGL Glue Library (carterd version)](https://github.com/carterd/Adafruit_LvGL_Glue) 
 * [RPI_PICO_TimerInterrupt](https://github.com/khoih-prog/RPI_PICO_TimerInterrupt)
 * [Arduino Encorder](https://github.com/carterd/Arduino_Encoder)
 * [ArduinoBLE (carterd version)](https://github.com/carterd/ArduinoBLE)
 * [LittleFS_Mbed_RP2040](https://github.com/khoih-prog/LittleFS_Mbed_RP2040)

# Issues

The current BLE library has an issue in the file 'src/utility/ATT.cpp' function 'ATTClass::discoverDescriptors', 
until this is fixed used the patched branch at (https://github.com/carterd/ArduinoBLE).
