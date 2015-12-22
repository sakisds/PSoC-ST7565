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
 * \file ST7565.c
 * \brief ST7565 Control
 * \author Thanasis Georgiou
 */

#include "`$INSTANCE_NAME`_st7565.h"

/* 
 * Internal display pagemap
 */
int `$INSTANCE_NAME`_pagemap[] = { 3, 2, 1, 0, 7, 6, 5, 4 };

/* 
 * Display Buffer
 */
extern uint8_t `$INSTANCE_NAME`_buffer[];

/* 
 * Refresh control flow
 */
int8_t `$INSTANCE_NAME`_txFinished = 0;
int8_t `$INSTANCE_NAME`_isRefreshing = 0;
int8_t `$INSTANCE_NAME`_currentPage = -1;
int8_t `$INSTANCE_NAME`_refreshStage = 0;

/* 
 * The select page command
 */
uint8_t `$INSTANCE_NAME`_selectPage[] = {0, CMD_SET_COLUMN_LOWER | (0x0 & 0xf), CMD_SET_COLUMN_UPPER | ((0x0 >> 4) & 0xf), CMD_RMW};

/*
 * The interrupt handler for SPI events
 */
CY_ISR(`$INSTANCE_NAME`_InterruptHandler) {    
    `$INSTANCE_NAME`_txFinished = 1;
}

/**
 * Resets the display
 */
void `$INSTANCE_NAME`_reset() {
    `$INSTANCE_NAME`_Pin_RST_Write(LOW);
    CyDelayUs(2);
    `$INSTANCE_NAME`_Pin_RST_Write(HIGH);
    CyDelayUs(2); // Wait for reboot
}

void `$INSTANCE_NAME`_init() {
    // Initialize and start hardware SPI
    `$INSTANCE_NAME`_SPI_Start();
    `$INSTANCE_NAME`_ISR_SPI_StartEx(`$INSTANCE_NAME`_InterruptHandler);
    
    // Set CLK low and reset
    `$INSTANCE_NAME`_reset();
    
    // Wait for a bit
    CyDelayUs(10);
    
    // Select display
    `$INSTANCE_NAME`_mode_control();
    
    // LCD Bias
    `$INSTANCE_NAME`_SPI_WriteTxData(CMD_SET_BIAS_7);
    // ADC Select
    `$INSTANCE_NAME`_SPI_WriteTxData(CMD_SET_ADC_NORMAL);
    // SHL Select
    `$INSTANCE_NAME`_SPI_WriteTxData(CMD_SET_COM_NORMAL);
    // Initial display line
    `$INSTANCE_NAME`_SPI_WriteTxData(CMD_SET_DISP_START_LINE);
    
    // Power
    `$INSTANCE_NAME`_SPI_WriteTxData(CMD_SET_POWER_CONTROL | 0x4); // Turn on voltage converter
    CyDelayUs(50000); // 500ms (Only 50 needed)
    `$INSTANCE_NAME`_SPI_WriteTxData(CMD_SET_POWER_CONTROL | 0x6); // Turn on voltage regulator
    CyDelayUs(50000); // 500ms
    `$INSTANCE_NAME`_SPI_WriteTxData(CMD_SET_POWER_CONTROL | 0x7); // Turn on voltage follower
    CyDelayUs(50000); // 500ms
   
    // Set operating voltage
    `$INSTANCE_NAME`_SPI_WriteTxData(CMD_SET_RESISTOR_RATIO | 0x6);
    CyDelayUs(10);
    
    // Set contrast
    `$INSTANCE_NAME`_set_contrast(20);
    
    // Clear screen
    memset(`$INSTANCE_NAME`_buffer, 0, 1024);
    
    // Turn on
    `$INSTANCE_NAME`_SPI_WriteTxData(CMD_DISPLAY_ON);
    `$INSTANCE_NAME`_SPI_WriteTxData(CMD_SET_ALLPTS_NORMAL);
}

void `$INSTANCE_NAME`_set_contrast(uint8_t value) {
    // Set in command mode
    `$INSTANCE_NAME`_mode_control();
    
    // Send contrast command
    `$INSTANCE_NAME`_SPI_WriteTxData(CMD_SET_CONTRAST);
    // Send contrast value
    if (value > 63) `$INSTANCE_NAME`_SPI_WriteTxData(63);
    else `$INSTANCE_NAME`_SPI_WriteTxData(value);
}

void `$INSTANCE_NAME`_refresh() {
    // Start refreshing
    `$INSTANCE_NAME`_isRefreshing = 1;
    `$INSTANCE_NAME`_txFinished = 1;
}

void `$INSTANCE_NAME`_refresh_loop() {
    if (`$INSTANCE_NAME`_isRefreshing == 1 && `$INSTANCE_NAME`_txFinished == 1) {
        // Reset interrupt flag
        `$INSTANCE_NAME`_txFinished = 0;
        
        // Check currect refresh stage
        if (`$INSTANCE_NAME`_refreshStage == 0) {
            `$INSTANCE_NAME`_refreshStage++; // Increment stage
            
            // Incement page
            `$INSTANCE_NAME`_currentPage++;
            if (`$INSTANCE_NAME`_currentPage == 8) {
                `$INSTANCE_NAME`_currentPage = -1;
                `$INSTANCE_NAME`_isRefreshing = 0;
                
                return;
            }
            
            // Control mode
            `$INSTANCE_NAME`_mode_control();
            
            // Enable interrupts
            
            // Select page
            `$INSTANCE_NAME`_selectPage[0] = CMD_SET_PAGE | `$INSTANCE_NAME`_pagemap[`$INSTANCE_NAME`_currentPage];
            `$INSTANCE_NAME`_SPI_PutArray(`$INSTANCE_NAME`_selectPage, 4);
        } else {
            `$INSTANCE_NAME`_refreshStage = 0; // Reset stage
            
            // Set mode to data and send the current page
            `$INSTANCE_NAME`_mode_data();
            `$INSTANCE_NAME`_SPI_WriteTxData(0xFF);
            
            uint8_t c;
            for(c = 0; c < 128; c++) {
                `$INSTANCE_NAME`_SPI_WriteTxData(
                    `$INSTANCE_NAME`_buffer[(128*`$INSTANCE_NAME`_currentPage)+c]
                );
            }
            
            //`$INSTANCE_NAME`_SPI_PutArray(buffer[128*currentPage], 128);
        }
    }
}

int8 `$INSTANCE_NAME`_is_refreshing() {
    return `$INSTANCE_NAME`_isRefreshing;
}

uint8_t* `$INSTANCE_NAME`_get_buffer() {
    return `$INSTANCE_NAME`_buffer;
}

/* [] END OF FILE */
