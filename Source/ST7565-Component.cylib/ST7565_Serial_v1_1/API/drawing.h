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
 * \file drawing.h
 * \brief ST7565 Drawing methods
 * \author Thanasis Georgiou
 */

#include "cytypes.h"

/*! 
 * \def SCREEN_WIDTH
 *      The screen's width
 */
#define SCREEN_WIDTH    128
/*! 
 * \def SCREEN_WIDTH
 *      The screen's width
 */
#define SCREEN_HEIGHT   64

//! Draw a specific pixel.
/*!
 * \param x The X position of the pixel.
 * \param y The Y position of the pixel.
 * \param color 0 To clear the pixel, 1 to turn it on.
 */
void `$INSTANCE_NAME`_draw_pixel(uint8_t x, uint8_t y, uint8_t color);

//! Draw a circle.
/*!
 *  \param x0       The X position of the circle's center.
 *  \param y0       The Y position of the circle's center.
 *  \param r        The circle's radius.
 *  \param color    The color to use (0 clears, 1 sets).
 */
void `$INSTANCE_NAME`_draw_circle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color);

//! Draw a line.
/*!
 *  \param x0       X position of the line's first point.
 *  \param y0       Y position of the line's first point.
 *  \param x1       X position of the line's second point.
 *  \param y1       Y position of the line's second point.
 *  \param color    The color to use (0 clears, 1 sets).
 */
void `$INSTANCE_NAME`_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);

//! Draw a horizontal line. Faster than drawing a normal line.
/*!
 *  \param x        The X of the line's left point.
 *  \param y        The Y of the line's left point.
 *  \param w        The line's width.
 *  \param color    The line's color (0 clears, 1 sets).
 */
void `$INSTANCE_NAME`_draw_line_hoz(uint8_t x, uint8_t y, uint8_t w, uint8_t color);

//! Draw a vertical line. Faster than drawing a normal line.
/*!
 *  \param x        The X of the line's top point.
 *  \param y        The Y of the line's top point.
 *  \param h        The line's height.
 *  \param color    The line's color (0 clears, 1 sets).
 */
void `$INSTANCE_NAME`_draw_line_ver(uint8_t x, uint8_t y, uint8_t h, uint8_t color);

//! Draw a rectangle.
/*!
 *  \param x        The X of the rectangle's top left point.
 *  \param y        The Y of the rectangle's top left point.
 *  \param h        The rectangle's height.
 *  \param w        The rectangle's width.
 *  \param color    The line's color (0 clears, 1 sets).
 */
void `$INSTANCE_NAME`_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);

//! Draw a character.
/*!
 * \param x         Where to draw the character's first pixel (top-left) (X).
 * \param y         Where to draw the character's first pixel (top-left) (X).
 * \param color     The character's color (0 clears, 1 sets).
 * \param size      How large should the character be. 1 = 8 pixels height.
 * \param c         The character to draw.
*/
void `$INSTANCE_NAME`_draw_character(uint8_t x, uint8_t y, uint8_t color, uint8_t size, uint8_t c);

//! Fill a rectangle.
/*!
 *  \param x        The X of the rectangle's top left point.
 *  \param y        The Y of the rectangle's top left point.
 *  \param h        The rectangle's height.
 *  \param w        The rectangle's width.
 *  \param color    The line's color (0 clears, 1 sets).
 */
void `$INSTANCE_NAME`_fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);

//! Fill the whole screen.
/*!
 *  \param color    The color to fill (0 clears, 1 sets).
 */
void `$INSTANCE_NAME`_draw_fillScreen(uint8_t color);

//! Sets the cursor to a specific position. This is used by _write_*().
/*!
 *  \param x        The X position of the cursor (pixels).
 *  \param y        The Y position of the cursor (pixels).
 */
void `$INSTANCE_NAME`_set_cursor(uint8_t x, uint8_t y);

//! Sets the text color.
/*!
 *  \param color    The text' color (0 white, 1 black).
 */
void `$INSTANCE_NAME`_set_textColor(uint8_t color);

//! Sets the text size.
/*!
 *  \param size     The new text size (1 = 8 pixels height).
 */
void `$INSTANCE_NAME`_set_textSize(uint8_t size);

//! Writes a char to the cursor position.
/*!
 *  \param c        The char to write.
 */
void `$INSTANCE_NAME`_write_char(uint8_t c);

//! Writes a string to the cursor position.
/*!
 *  \param c        The string to write (terminated by \0).
 */
void `$INSTANCE_NAME`_write_string(char* c);

/* [] END OF FILE */