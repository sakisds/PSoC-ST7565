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
 * \file drawing.c
 * \brief ST7565 Drawing methods
 * \author Thanasis Georgiou
 */

#include "`$INSTANCE_NAME`_drawing.h"
#include "`$INSTANCE_NAME`_font.h"

/* The buffer */
uint8_t `$INSTANCE_NAME`_buffer[128*64/8];
/* Used for swaping variables */
uint8_t `$INSTANCE_NAME`_temp;

/* Text cursor position */
uint8_t `$INSTANCE_NAME`_cursor_x = 0;
uint8_t `$INSTANCE_NAME`_cursor_y = 0;

/* Text color & size*/
uint8_t `$INSTANCE_NAME`_txtcolor = 1;
uint8_t `$INSTANCE_NAME`_txtsize = 1;

void `$INSTANCE_NAME`_draw_pixel(uint8_t x, uint8_t y, uint8_t color) {
    if (x > SCREEN_WIDTH - 1 || y > SCREEN_HEIGHT - 1)
        return;
    
  // x is which column
  if (color) 
    `$INSTANCE_NAME`_buffer[x+ (y/8)*128] |= (1 << (7-(y%8)));  
  else
    `$INSTANCE_NAME`_buffer[x+ (y/8)*128] &= ~(1 << (7-(y%8))); 
}

void `$INSTANCE_NAME`_draw_circle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color) {
    int16_t f = 1 - r;
    uint8_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    uint8_t x = 0;
    uint8_t y = r;

    `$INSTANCE_NAME`_draw_pixel(x0  , y0+r, color);
    `$INSTANCE_NAME`_draw_pixel(x0  , y0-r, color);
    `$INSTANCE_NAME`_draw_pixel(x0+r, y0  , color);
    `$INSTANCE_NAME`_draw_pixel(x0-r, y0  , color);

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        `$INSTANCE_NAME`_draw_pixel(x0 + x, y0 + y, color);
        `$INSTANCE_NAME`_draw_pixel(x0 - x, y0 + y, color);
        `$INSTANCE_NAME`_draw_pixel(x0 + x, y0 - y, color);
        `$INSTANCE_NAME`_draw_pixel(x0 - x, y0 - y, color);
        `$INSTANCE_NAME`_draw_pixel(x0 + y, y0 + x, color);
        `$INSTANCE_NAME`_draw_pixel(x0 - y, y0 + x, color);
        `$INSTANCE_NAME`_draw_pixel(x0 + y, y0 - x, color);
        `$INSTANCE_NAME`_draw_pixel(x0 - y, y0 - x, color);
    }
}

void `$INSTANCE_NAME`_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color) {
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    // Swap x0 and y0
    `$INSTANCE_NAME`_temp = x0;
    x0 = y0;
    y0 = `$INSTANCE_NAME`_temp;
    
    // Swap x1 and y1
    `$INSTANCE_NAME`_temp = x1;
    x1 = y1;
    y1 = `$INSTANCE_NAME`_temp;
  }

  if (x0 > x1) {
    // Swap x0 with x1
    `$INSTANCE_NAME`_temp = x0;
    x0 = x1;
    x1 = `$INSTANCE_NAME`_temp;
    
    // Swap y0 and y1
    `$INSTANCE_NAME`_temp = y0;
    y0 = y1;
    y1 = `$INSTANCE_NAME`_temp;
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
      `$INSTANCE_NAME`_draw_pixel(y0, x0, color);
    } else {
      `$INSTANCE_NAME`_draw_pixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

void `$INSTANCE_NAME`_draw_line_hoz(uint8_t x, uint8_t y, uint8_t w, uint8_t color) {
    while (w != 0) {
        `$INSTANCE_NAME`_draw_pixel(x + w - 1, y, color);
        w--;
    }
}

void `$INSTANCE_NAME`_draw_line_ver(uint8_t x, uint8_t y, uint8_t h, uint8_t color) {
    while (h != 0) {
        `$INSTANCE_NAME`_draw_pixel(x, y + h - 1, color);
        h--;
    }
}

void `$INSTANCE_NAME`_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
    `$INSTANCE_NAME`_draw_line_hoz(x, y, w, color);
    `$INSTANCE_NAME`_draw_line_hoz(x, y+h-1, w, color);
    `$INSTANCE_NAME`_draw_line_ver(x, y, h, color);
    `$INSTANCE_NAME`_draw_line_ver(x+w-1, y, h, color);
}

void `$INSTANCE_NAME`_draw_character(uint8_t x, uint8_t y, uint8_t color, uint8_t size, uint8_t c) {
    // Check if screen is out of bounds
    if(
        (x >= SCREEN_WIDTH) ||
        (y >= SCREEN_HEIGHT) ||
        ((x + 6 * size - 1) < 0) ||
        ((y + 8 * size - 1) < 0)
    ) return;
    
    // Width = 6
    uint8_t i;
    for (i = 0; i < 6; i++) {
        uint8_t line;
        if (i == 5) line = 0x0; // Spacing
        else line = `$INSTANCE_NAME`_font[(c*5)+i];
      
        int8_t j;
        for (j = 0; j<8; j++) { // Height = 8
            if (line & 0x1) {
                if (size == 1) // default size
                    `$INSTANCE_NAME`_draw_pixel(x+i, y+j, color);
                else {  // big size
                    `$INSTANCE_NAME`_fill_rect(x+(i*size), y+(j*size), size, size, color);
                } 
            } else {
                if (size == 1) // default size
                    `$INSTANCE_NAME`_draw_pixel(x+i, y+j, 0);
                else {  // big size
                    `$INSTANCE_NAME`_fill_rect(x+i*size, y+j*size, size, size, 0);
                }
            }
            line >>= 1;
        }   
    }
}

void `$INSTANCE_NAME`_fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
    uint8_t i;
    
    for (i = y; i < y+h; i++) {
        `$INSTANCE_NAME`_draw_line(x, i, x + w, i, color);
    }
}

void `$INSTANCE_NAME`_draw_fillScreen(uint8_t color) {
    memset(`$INSTANCE_NAME`_buffer, color, 1024);
}

void `$INSTANCE_NAME`_set_cursor(uint8_t x, uint8_t y) {
    if (x < SCREEN_WIDTH) {
        `$INSTANCE_NAME`_cursor_x = x;
    } else {
        `$INSTANCE_NAME`_cursor_x = SCREEN_WIDTH - 1;
    }
    
    if (y < SCREEN_HEIGHT) {
        `$INSTANCE_NAME`_cursor_y = y;
    } else {
        `$INSTANCE_NAME`_cursor_y = SCREEN_HEIGHT - 1;
    }
}

void `$INSTANCE_NAME`_set_textColor(uint8_t color) {
    `$INSTANCE_NAME`_txtcolor = color;
}

void `$INSTANCE_NAME`_set_textSize(uint8_t size) {
    `$INSTANCE_NAME`_txtsize = size;
}

void `$INSTANCE_NAME`_write_char(uint8_t c) {
    if (c == '\n') {
        `$INSTANCE_NAME`_cursor_y += `$INSTANCE_NAME`_txtsize*8;
        `$INSTANCE_NAME`_cursor_x  = 0;
    } else if (c == '\r') {
        return; // Skip
    } else {
        `$INSTANCE_NAME`_draw_character(`$INSTANCE_NAME`_cursor_x, `$INSTANCE_NAME`_cursor_y, `$INSTANCE_NAME`_txtcolor, `$INSTANCE_NAME`_txtsize, c);
        `$INSTANCE_NAME`_cursor_x += `$INSTANCE_NAME`_txtsize*6;
        
        // New line if cursor outside of screen bounds
        if (`$INSTANCE_NAME`_cursor_x > SCREEN_WIDTH - 1) {
            `$INSTANCE_NAME`_cursor_x = 0;
            `$INSTANCE_NAME`_cursor_y += `$INSTANCE_NAME`_txtsize*8;
        }
    }
}

void `$INSTANCE_NAME`_write_string(char* c) {
    uint8_t index = 0;
    
    while(c[index] != '\0') {
        `$INSTANCE_NAME`_write_char(c[index]);
        index++;
    }
}

/* [] END OF FILE */
