# Bluetooth Specialized Display Circuit Diagram (https://github.com/carterd/BTSpecializedDisplay)

I've created a number of versions of the Bluetooth Specialized Display based on the the following:

 * Arduino Nano RP2040 Connect

 * TTGO T-Display

 * T-Display-S3

Each circuit is designed to plug into the Brose motors which are compatable with 6v lights. The compatibility 
for such motors can be found here for the Lupine lights https://www.lupine.de/eng/service/downloads/66-3.

Therefore we see that if we limit the Display circuit to max of 6v input the circuit is compatible with:

 * Gen 1 Motors
 * Gen 1.2 / 1.2E / 1.2S Motors
 * Gen 1.3 Motors

# Converting to use universal E-Bike connectors

In order to give a cheap universal connection for display and lights a Brose to universal E-Bike connector
lead can be implemented as follows:

```
Two pin E-bike "RED" connector

               ┌────────┐─────┐                            ┌────┌─────────┐     ┌────────┐─────┐
              ┌┘        └┐    └┐ 6V (Red)         (White) ┌┘   ┌┘         └┐   ┌┘        └┐    └┐ 6V (White)     
              │    ===== │     ├──────────────────────────┤    │     O     │   │    ===== │     ├───────────────────────   
     Brose    │         ┌┘     │                          │    │          ┌┘   └┐         │     │                           Bluetooth 
              │         │      │                          │    │          │     │         │     │
     Motor    │         └┐     │ 0V (Black)       (Black) │    │          └┐   ┌┘         │     │ 0V (Black)                Display
              │    ===== │     ├──────────────────────────┤    │     O     │   │    ===== │     ├───────────────────────   
              └┐        ┌┘    ┌┘                          └┐   └┐         ┌┘   └┐        ┌┘    ┌┘ 
               └────────┘─────┘                            └────└─────────┘     └────────┘─────┘ 
                Lupine/Exposure                             E-bike 2 pin RED
                Male Connector                              Female Connector

Five pin E-bike "GREEN" connector

               ┌────────┐─────┐                            ┌────┌─────────┐     ┌────────┐─────┐
              ┌┘        └┐    └┐ 6V (Red)                 ┌┘   ┌┘         └┐   ┌┘        └┐    └┐ 6V ([R]ed [B]lu[E])     
              │    ===== │     ├──────────────────────────┤    │   O   O   │   │  BE= G== │     ├───────────────────────   
     Brose    │         ┌┘     │                          │    │          ┌┘   └┐         │     │                            Bluetooth 
              │         │      │                          │    │  O       │     │      Y= │     │
     Motor    │         └┐     │ 0V (Black)               │    │          └┐   ┌┘         │     │ 0V ([B]lac[K] [G]reen)     Display
              │    ===== │     ├──────────────────────────┤    │   O   O   │   │  R== BK= │     ├───────────────────────
              └┐        ┌┘    ┌┘                          └┐   └┐         ┌┘   └┐        ┌┘    ┌┘ 
               └────────┘─────┘                            └────└─────────┘     └────────┘─────┘ 
                Lupine/Exposure                             E-bike 5 pin GREEN
                Male Connector                              Female Connector

```