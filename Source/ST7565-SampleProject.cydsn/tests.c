/* ========================================
 * ST7565 PSoC Component - Sample Project
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
 * \file tests.c
 * \brief ST7565 PSoC Component - Testing
 * \author Thanasis Georgiou
 */

#include "tests.h"

int8_t testState = -1;

uint8_t testBacklight() {
    LCD_set_cursor(0, 0);
    LCD_draw_fillScreen(0);
    
    testState++;
    switch (testState) {
        case 0: LCD_write_char('R'); Backlight_Reg_Write(0b00000001); LCD_refresh(); return 0;
        case 1: LCD_write_char('G'); Backlight_Reg_Write(0b00000010); LCD_refresh(); return 0;
        case 2: LCD_write_char('B'); Backlight_Reg_Write(0b00000100); LCD_refresh(); return 0;
        case 3: LCD_write_string("RG"); Backlight_Reg_Write(0b00000011); LCD_refresh(); return 0;
        case 4: LCD_write_string("GB"); Backlight_Reg_Write(0b00000110); LCD_refresh(); return 0;
        case 5: LCD_write_string("RB"); Backlight_Reg_Write(0b00000101); LCD_refresh(); return 0;
        case 6: LCD_write_string("RGB"); Backlight_Reg_Write(0b00000111); LCD_refresh(); return 0;
        default: testState = -1; return 1;
    }
}

uint8_t testLines() {
    testState++;
    switch (testState) {
        case 0:
            LCD_draw_fillScreen(0);
            LCD_set_cursor(0, 0);
            LCD_write_string("Lines");
            
            LCD_refresh();
            return 0;
        case 1:
            LCD_draw_line(64, 64, 71, 49, 1);
            LCD_draw_line(12, 14, 121, 24, 1);
            LCD_draw_line(32, 12, 100, 60, 1);
            LCD_draw_line(1, 64, 50, 27, 1);
            
            LCD_refresh();
            return 0;
        case 2:
            LCD_draw_line(0, 0, 10, 10, 1);
            LCD_draw_line(17, 42, 112, 12, 1);
            LCD_draw_line(3, 72, 120, 90, 1);
            LCD_draw_line(4, 37, 81, 51, 1);
            
            LCD_refresh();
            return 0;
        default:
            testState = -1;
            return 1;
    }
}

uint8_t testCircles() {
    testState++;
    switch(testState) {
        case 0:
            LCD_draw_fillScreen(0);
            LCD_set_cursor(0, 0);
            LCD_write_string("Circles");
            
            LCD_refresh();
            return 0;
        case 1:
            LCD_draw_circle(20, 20, 5, 1);
            LCD_draw_circle(40, 32, 5, 1);
            LCD_draw_circle(100, 40, 30, 1);
            
            LCD_refresh();
            return 0;
        case 2:
            LCD_draw_circle(60, 10, 40, 1);
            LCD_draw_circle(52, 24, 12, 1);
            
            LCD_refresh();
            return 0;
        default:
            testState = -1; return 1;
    }
}

uint8_t testRectangles() {
    testState++;
    switch(testState) {
        case 0:
            LCD_draw_fillScreen(0);
            LCD_set_textSize(2);
            LCD_set_cursor(0, 50);
            LCD_write_string("Rectangles");
            
            LCD_refresh();
            return 0;
        case 1:
            LCD_draw_rect(10, 10, 21, 17, 1);
            LCD_draw_rect(18, 42, 121, 19, 1);
            LCD_fill_rect(0, 0, 10, 10, 1);
            
            LCD_refresh();
            return 0;
        case 2:
            LCD_draw_rect(2, 2, 8, 8, 0);
            LCD_draw_rect(4, 4, 6, 6, 0);
            
            LCD_refresh();
            return 0;
        default:
            testState = -1;
            return 1;
    }
}

uint8_t testPattern() {
    uint8_t pattern;
    
    if (testState == -1) {
        pattern = 0xAA;
        testState = 0;
    } else {
        pattern = 0x55;
        testState = -1;
    }
    
    memset(LCD_get_buffer(), pattern, 1024);
    
    LCD_refresh();
    return 0;
}

/* [] END OF FILE */
