/* ========================================
 * ST7565 PSoC Component
 * Copyright Thanasis Georgiou 2015
 *
 * This file is part of the ST7565 PSoC Component Project.
 *
 * The ST7565 PSoC Component is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either version
 * 3 of the License, or (at your option) any later version.
 *
 * The ST7565 PSoC Component is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the ST7565 PSoC Component.
 * If not, see <http://www.gnu.org/licenses/>.
 * ========================================
 */

/**
 * \file ST7565.h
 * \brief ST7565 Control
 * \author Thanasis Georgiou
 */

#include "cytypes.h"
#include "cyfitter.h"

#include <stdint.h>
#define HIGH    1
#define LOW     0

#include "`$INSTANCE_NAME`_drawing.h"

/*! 
 * \def CMD_DISPLAY_OFF
 *      Turns the display off.
 */
#define CMD_DISPLAY_OFF 0xAE
/*! 
 * \def CMD_DISPLAY_ON
 *      Turns the display on.
 */
#define CMD_DISPLAY_ON  0xAF

/*! 
 * \def CMD_SET_DISP_START_LINE
 *      Sets the start line.
 */
#define CMD_SET_DISP_START_LINE  0x40
/*! 
 * \def CMD_SET_PAGE
 *      Selects a page.
 */
#define CMD_SET_PAGE  0xB0


/*! 
 * \def CMD_SET_COLUMN_UPPER
 *      Set upper column.
 */
#define CMD_SET_COLUMN_UPPER  0x10
/*! 
 * \def CMD_SET_COLUMN_LOWER
 *      Set lower column.
 */
#define CMD_SET_COLUMN_LOWER  0x00

/*! 
 * \def CMD_SET_ADC_NORMAL
 *      Set ADC to normal mode.
 */
#define CMD_SET_ADC_NORMAL  0xA0
/*! 
 * \def CMD_SET_ADC_REVERSE
 *      Set ADC to reverse mode.
 */
#define CMD_SET_ADC_REVERSE 0xA1

/*! 
 * \def CMD_SET_DISP_NORMAL
 *      Set the display to normal mode.
 */
#define CMD_SET_DISP_NORMAL 0xA6

/*! 
 * \def CMD_SET_DISP_REVERSE
 *      Set the display to reverse mode (useful for flashing).
 */
#define CMD_SET_DISP_REVERSE 0xA7

#define CMD_SET_ALLPTS_NORMAL 0xA4
#define CMD_SET_ALLPTS_ON  0xA5
#define CMD_SET_BIAS_9 0xA2 
#define CMD_SET_BIAS_7 0xA3

#define CMD_RMW  0xE0
#define CMD_RMW_CLEAR 0xEE
#define CMD_INTERNAL_RESET  0xE2
#define CMD_SET_COM_NORMAL  0xC0
#define CMD_SET_COM_REVERSE  0xC8
#define CMD_SET_POWER_CONTROL  0x28
#define CMD_SET_RESISTOR_RATIO  0x20
#define CMD_SET_CONTRAST  0x81
#define CMD_SET_STATIC_OFF  0xAC
#define CMD_SET_STATIC_ON  0xAD
#define CMD_SET_STATIC_REG  0x0
#define CMD_SET_BOOSTER_FIRST  0xF8
#define CMD_SET_BOOSTER_234  0
#define CMD_SET_BOOSTER_5  1
#define CMD_SET_BOOSTER_6  3
#define CMD_NOP  0xE3
#define CMD_TEST  0xF0

/* Bit manipulation help */
#define MSB_POSITION   0x80u
#define SHIFT_BY_1     0x01u

/* Pin Macros */
#define `$INSTANCE_NAME`_unselect() `$INSTANCE_NAME`_Pin_CS_Write(HIGH);
#define `$INSTANCE_NAME`_select() `$INSTANCE_NAME`_Pin_CS_Write(LOW);
#define `$INSTANCE_NAME`_mode_control() `$INSTANCE_NAME`_Pin_A0_Write(LOW);
#define `$INSTANCE_NAME`_mode_data() `$INSTANCE_NAME`_Pin_A0_Write(HIGH);

//! Resets the LCD.
/*! Resets the LCD by pulling the RST pin LOW for 2uS.
 *  This call is takes about 4uS.
 */
void `$INSTANCE_NAME`_reset();

//! Initialize the LCD.
/*!
 *  Initializes the LCD. In detail:
 *  - Initializes the hardware SPI port and enables interrupts.
 *  - Resets the LCD to a known state.
 *  - Sets the LCD Bias level, selects the ADC and the SHL.
 *  - Turns on the power circuits of the controller.
 *  - Sets the operating voltage and the contrast.
 *  - Empty the memory buffer.
 *  - Turns the display on.
 * 
 * For this display, a 1024 byte buffer is allocated in the PSoC's SRAM.
 * There is no need to do double buffering since a refresh is only carried
 * out when instructed by _refresh().
 */
void `$INSTANCE_NAME`_init();

//! Sets the display contrast
/*!
 * Sets the display contrast by sending two bytes.
 * \param value The new contrast value (0-63). Values outside the range are clipped to 63.
 */
void `$INSTANCE_NAME`_set_contrast(uint8_t value);

//! Instructs that a refresh takes place.
/*!
 *  Sets the "refresh" flag so the display can start refreshing on the next
 *  main loop. Assuming each SPI transfer finishes before the main loop ends
 *  it will take 8 + 8 main loops to refresh the whole display.
 */
void `$INSTANCE_NAME`_refresh();

//! Refresh the display if required. PLACE IN MAIN LOOP.
/*! 
 *  This will refresh the display depanding on if it's required
 *  and the last refresh state of the display.
 *  It is REQUIRED to place this inside the main loop of your application,
 *  differently the display will never refresh.
 */
void `$INSTANCE_NAME`_refresh_loop();

//! Checks if the display is currently refreshing.
/*!
 * /returns 1 if the display is refreshing, 0 if not. 
 */
int8 `$INSTANCE_NAME`_is_refreshing();

//! Returns the display buffer.
/*!
 * /returns The pointer to the display buffer. The length is 1024 bytes.
 */
uint8_t* `$INSTANCE_NAME`_get_buffer();
/* [] END OF FILE */
