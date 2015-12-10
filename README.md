# PSoC-ST7565
A PSoC component for using ST7565 serial graphical LCDs.

## Hardware compatibility
On the PSoC side, I have tested this with the CY8CKIT-042 PSoC4 Pioneer
kit and it works just fine. It should work on PSoC5 too but I haven't
tried it, if anyone gives it a try contact me to update this.

About the graphical LCDs, I used the one Adafruit sells over
[here](https://www.adafruit.com/products/250?&main_page=product_info&products_id=250).
There is a possibility that other displays with the same controller 
will require some variations in the initialization code.

## Installation
Clone this repository on your computer, inside the Source folder there
are two projects, ST7565-Component contains the PSoC component and 
should be added to your workspace as a dependency and ST7565-SampleProject
which contains example code and schematics on how to use the display.

## Usage
The display required 5 data lines (RST, A0, CS, DIN, SCLK), 1 power source
(+5V). The component includes pins for the backlight (for the Adafruit
display, RGB, common anode).

To setup a new project with the display:
* Include the component project as a dependency to your project.
* Add a ST7565 Component to your schematic (from the Community tab, inside
  the Display folder).
* Assign positions to the 5 data pins of the display.
* Optionally connect something to drive the backlight to the BL_R, BL_G
  and BL_B of the schematic symbol. You can use either something like
  a control register or PWM for more colors.
  
Do NOT connect the backlight output pins directly to the display! They
sink much more than 8mA so you will burn out your PSoC's pin. I personally
used a couple of low power (200mA) MOSFETs.

The following code is the bare minimum this display requires to work:
```c
/* The display in this case is named LCD, in your own project probably
 * has a different name. */

int main() {
    /* The display uses interrupts internally for SPI */
    CyGlobalIntEnable; /* Enable global interrupts. */

    // Initialize display
    LCD_init(); // This also resets the display.
    LCD_refresh(); // Send the empty buffer.
    
    for (;;) {
        LCD_refresh_loop(); // Needs to be called on every loop!
    }
}
```

When you are finished drawing you should always call `_refresh()` to
signal that the LCD needs refreshing. Assuming that the SPI transfers
complete between each call of `_refresh_loop()` it will take approximately
16 calls to completely refresh the display.

By default, hardware SPI is clocked at 4MHz. If the display is not working
you can try lowering this clock in the component's schematic. I found out
that even 100kHz is working fine and has little impact on the refrest rate.

## License
The drawing part of this component (`drawing.h`, `drawing.c`) is heavily
inspired by the Adafruit GFX Library. Consider buying the LCD from them
to support the awesome software they release freely.

ST7565 PSoC Component Copyright Thanasis Georgiou 2015
This file is part of the ST7565 PSoC Component Project.

The ST7565 PSoC Component is free software: you can redistribute it
and/or modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation, either version
3 of the License, or (at your option) any later version.

The ST7565 PSoC Component is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
General Public License for more details.

Check the LICENSE.md file for a copy of the GNU Lesser General
Public License. If it isn't available, see <http://www.gnu.org/licenses/>.