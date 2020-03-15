#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_


#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

/** @defgroup Graphics Graphics
 * @{
 * Contains functions to set up the video graphics device.
 */

/**
 * @brief Set the intended vbe mode 
 * 
 * @param mode - mode to be set
 * @return int 
 */
int set_vbe_mode(uint16_t mode);

/**
 * @brief Colors one pixel
 * 
 * @param x - x coordinate of the pixel
 * @param y - y coordinate of the pixel
 * @param color - color to aply on the pixel
 * @return int 
 */
int vg_color_pixel(uint16_t x, uint16_t y, uint32_t color);


int vg_draw_line_of_rectangles(uint16_t x, uint16_t y, uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step, uint32_t color, uint16_t rec_x, uint16_t rec_y);

int vg_draw_matrix(uint16_t x, uint16_t y, uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step, uint32_t color);

int vg_draw_pattern(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);

uint32_t generate_red(uint32_t first, uint8_t step, int col);
uint32_t generate_green(uint32_t first, uint8_t step, int row);
uint32_t generate_blue(uint32_t first, uint8_t step, int row, int col);

/**
 * @brief Draws a pixmap on the screen
 * 
 * @param img - image to be drawn
 * @param xpm - pixmap
 * @param x - x coordinate where image will be drawn
 * @param y - y coordinate where image will be drawn
 * @return int 
 */
int vg_draw_pixmap(xpm_image_t img, xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Handles speed movement of pixmap (used in lab5)
 * 
 * @param img 
 * @param xpm 
 * @param xi 
 * @param yi 
 * @param xf 
 * @param yf 
 * @param speed 
 * @return int 
 */
int speed_handler(xpm_image_t img, xpm_map_t xpm, uint16_t *xi, uint16_t *yi, uint16_t xf, uint16_t yf, int16_t speed);

/**
 * @brief Get the information of the mode that is being used
 * 
 * @param mode - vbe mode
 * @param info - information to be changed
 * @return int 
 */
int get_mode_info(uint16_t mode, vbe_mode_info_t* info);

/**@} end of Graphics*/

#endif
