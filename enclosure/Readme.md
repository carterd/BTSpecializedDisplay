# Bluetooth Specialized Display Enclosure (https://github.com/carterd/BTSpecializedDisplay)

This Readme descibes the files of 3D models for the enclosure of an external Ebike display for Specialized Turbo bikes, 
these include Kenevo/Levo/Creo.

# The Files

The enclosure is designed in Blender, and an associated .blend file is included. As well as each individual component exported as an STL file as follows:

 * SpecializedDisplay.blend

    This blend file contains the following Collection of objects:

    * CaseFloor - The bottom of the enclosure and have attempted to include a curve surface in an attempt to match the toptube
                  of a bike.

    * Controller - A reference object to show the rough dimensions of the Arduino Nana RP2040 Connect.

    * Display - A reference object to show the rough dimensions of the Adafruit 128x64 OLED display and the position of it's buttons.

    * LongButton - This is the 3 push buttons which should be filled down in order that they are the correct length for contact with
                   the buttons of the Adafruit 128x64 OLED display.

    * RoundCase - The main case body for both the Adafruit 128x64 OLED display, and the Arduino Nana RP2040 Connect.

    * RoundCaseTop - The top cover of the encolsure.

 * SpecializedDisplayButtons.stl - Exported STL file of the LongButton object from the SpecializedDisplay.blend file.

 * SpecializedDisplayCase.stl - Exported STL file of the RoundCase object from the SpecializedDisplay.blend file.
 
 * SpecializedDisplayFloor.stl - Exported STL file of the CaseFloor object from the SpecializedDisplay.blend file.

 * SpecializedDisplayTop.stl - Exported STL file of the RoundCaseTop object from the SpecializedDisplay.blend file.

 # Construction

 After printing the four STL files, SpecializedDisplayButtons.stl, SpecializedDisplayCase.stl, SpecializedDisplayFloor.stl, SpecializedDisplayTop.stl; modification with sand-paper and/or file maybe required for them to snugly fit together.

 In order to attempt to water proof the device, I've glued in place a clear plastic film over the display appature of the
 SpecializedDisplayTop part of the enclosure.

 Then the SpecializedDisplayTop and SpecializedDisplayCase parts are glued together.

 The SpecializedDisplayButtons parts are filed down till they function correctly with the Adafruit display correctly fitted.
 Then the buttons are glued in place while still allowing the buttons to actuate correctly.

 Then apply a thin layer of Latex Rubber in the gaps between buttons and case in an attempt to provide a waterproof barrier.
 Taking care not to cover any of the clear plastic film over the display appature.

 Finally use the SpecializedDisplayFloor as a template for drilling 1mm holes into the SpecializedDisplayCase and use very
 small M1.2 or M1.4 screws to fix the floor into place.

 I did not find the need to pack behind either the Arduino Nana RP2040 Connect, or Adafruit 128x64 OLED display but this
 maybe required if these components are found to be loose within the enclosure.

 # Mounting

 The enclosure is designed to be mounted on the top-tube of the bike, and a power cable running from the Display can be routed
 to the Brose motor where 6 volts outputs are provided for front and rear lights. For connection to the Brose Motor use something
 like:

   * Exposure E-Bike Light Connector - Brose 

   * Lupine Light Cable for E-Bike - Brose
