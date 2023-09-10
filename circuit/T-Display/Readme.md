# Bluetooth Specialized Display Circuit Diagram (https://github.com/carterd/BTSpecializedDisplay)

This Readme descibes the components and circuit connections for the external Ebike display for Specialized Turbo bikes, 
these include Kenevo/Levo/Creo, this is based upon the TTGO T-Display.

# Components

The circuit for the software is based on a single component:

 * TTGO-T-Display, https://github.com/Xinyuan-LilyGO/TTGO-T-Display

# Wiring Diagram

The wiring diagram is as follows:
```

       ┌───────────────────────────┐
       │                           │
       │           TTGO            │
       │                           │
     ──┤GND                    +3V3├──
       │                           │
     ──┤GND                      36├──
       │                           │
     ──┤21                       37├──
       │                           │
     ──┤22                       38├──
       │                           │
     ──┤17                       39├──
       │                           │
     ──┤2                        32├──
       │           TTGO            │
     ──┤15       T-DISPLAY       33├──
       │                           │
     ──┤13                       25├──
       │                           │
     ──┤12                       26├──
       │                           │
     ──┤GND                      27├──
       │                           │
     ──┤GND                     GND├───────────────────────── POWER LEAD GND
       │                           │
     ──┤+3V3                    +5V├──────────────────|<───── POWER LEAD 6V
       │                           │                Diode
       │                           │               1N4007
       │                           │
       │                           │
       │     ┌───────────────┐     │
       │     │               │     │
       │ BTN │     USB-C     │ BTN │
       │     │               │     │
       └─────┤               ├─────┘
             └───────────────┘ 


```
The pinouts of the two components are as follows, for the Arduino Nano RP2040 Connect as follows:

![image](TTGO-T-Display_pinout.png)
