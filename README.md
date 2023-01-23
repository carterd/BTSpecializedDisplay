# Bluetooth Specialized Display (https://https://github.com/carterd/BTSpecializedDisplay)

Ebike display for Specialized Turbo bikes, these include Kenevo/Levo/Creo.

# Configuration

If your using the Arduino IDE to compile the software you'll need to place each of the dependencies into the 'Arduino/library'
folder, and install the BTSpecializedDisplay project files into 'Arduino/BTSpecializedDisplay'. In addition you will need to 
copy the file "lv_conf.h" from of the location 'Arduino/library/BTSpecializedDisplay/lv_conf.h' to the location 
'Arduino/library/lv_conf.h'. This will ensure the lvgl library is configured correctly for RP2040 and Adafruit libraries.

Take special note of the version of LVGL your using ... if your decide to use the latest from the GIT repository you'll need
to make specific changes to the file 'Arduino/library/Adafruit_LvGL_Glue/src/Adafruit_LvGL_Glue.h', lines 4-5. 

# Dependencies
 
 * [ArduinoBLE (carterd version)](https://github.com/carterd/ArduinoBLE)

    As of writting this readme, ArduinoBLE 1.3.2 has a bug and needs patching hence clone the above branch into your
    Arduino/library path.

 * [Arduino LvGL Glue Library](https://github.com/carterd/Arduino_LvGL_Glue) 

    This is my own version of the Adafruit LvGL Glue, and hence you'll need to clone the branch to Arduino/library path.
    The library provides driver implementation for various graphics displays with LVGL.
    
 * [LittlevGL](https://github.com/littlevgl/lvgl)

    As of writting the code, the application has been written against version 8.3 of LVGL. Hence, I'd advise using the
    latest version 8.3.4 of LVGL (as of writting). Note, to copy the "lv_conf.h" out of Adafruit_LvGL_Glue above.
    
    You can use the latest lvgl source from github, but there has been a change to the debug API and hence this will 
    require you to patch my Adafruit LvGL Glue Library. Hence, if your not confident in coding skills then use LVGL version
    8.3.4.

 * [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)

    As of writting the code, the application has been written against version 1.11.3 of Adafruit GFX Library. We'll need
    this to drive the Adafruit OLED 128x64 FeatherWing.

 * [Adafruit SH110X Library](https://github.com/adafruit/Adafruit_SH110X)

    As of writting the code, the application has been written against version 2.1.6 of Adafruit SH110X Library. We'll need
    this to drive the SH110X display. Hence other types of display may mean you don't require this library.

 * [Arduino Encorder](https://github.com/carterd/Arduino_Encoder)

    The LvGL library has a concept of an encoder, this library implements a generic framework for such an encoder as well
    as a three button encoder implementation which is compatable with the three buttons on the Adafruit OLED 128x64 FeatherWing.

 * [Adafruit_TouchScreen](https://github.com/adafruit/Adafruit_TouchScreen)

    When re-writting the Adafruit LvGL Glue Library, the touch-screen code was maintained, hence even though SH110X display
    has buttons and not touchscreen, some touch screen code is left in LvGL Glue Library and hence requires this library
    unless you explicitly remove these references to touch screens.

 * [MBED_RPI_PICO_TimerInterrupt](https://github.com/khoih-prog/MBED_RPI_PICO_TimerInterrupt)

    This is a required RP2040 specific library specifically to create an interrupt to update the LvGL timer.
 
 * [LittleFS_Mbed_RP2040](https://github.com/khoih-prog/LittleFS_Mbed_RP2040)

    AS of writting using version 1.1.0 of LittleFS_Mbed_RP2040. An implementation of a file-system on the RP2040 hence
    allowing configuration to be stored in the flash memory of the RP2040.

# Issues

The current BLE library has an issue in the file 'src/utility/ATT.cpp' function 'ATTClass::discoverDescriptors', 
until this is fixed used the patched branch at (https://github.com/carterd/ArduinoBLE).

The application has been written using a branched version of "Adafruit LvGL Glue Library" which differs significatly
hence care should be used to use the specific dependancy identified above.

# The Circuit

Please see [a relative link](circuit/Readme.md) for more details on the specifics of the hardware components and
details of wiring.

# The Enclosure

An example enclosure has been designed in Blender the assoicated .blend file of this enclosure can be found in the
directory ./enclosure, as well as a number of .STL files of the individual parts that comprise the
enclosure, see [a relative link](enclosure/Readme.md).

# Copyright Notice

Bluetooth Display for Specialized Turbo bikes
Copyright (C) 2022 Derek Carter

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.