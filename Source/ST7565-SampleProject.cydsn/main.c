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
 * \file main.c
 * \brief ST7565 PSoC Component - Sample Project
 * \author Thanasis Georgiou
 */

#include "tests.h"

/* Timer used for drawing the next test */
uint8_t timerOverflow = 0;
CY_ISR(TimerInterrupt) {
    timerOverflow = 1;
}

/* The current test running */
uint8 currentTest = 0;

void setup() {
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    /* Enable test delay timer */
    ISR_TIMER_StartEx(TimerInterrupt);
    Timer_TestDelay_Start();

    // Initialize display
    LCD_init();
    LCD_set_textSize(3);
    LCD_refresh();
}

void loop() {
    if (timerOverflow == 1 && LCD_is_refreshing() == 0) {
        timerOverflow = 0;
        
        switch(currentTest) {
            case 0: if (testBacklight()) currentTest++; break;
            case 1: if (testLines()) currentTest++; break;
            case 2: if (testCircles()) currentTest++; break;
            case 3: if (testRectangles()) currentTest++; break;
            case 4: if (testPattern()) currentTest++; break;
            default: break;
        }
    }
    
    // Refresh the screen
    LCD_refresh_loop();
}

int main() {
    setup();
    
    for (;;) {
        loop();
    }
}
/* [] END OF FILE */
