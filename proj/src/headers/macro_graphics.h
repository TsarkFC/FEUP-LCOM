#ifndef _MACRO_GRAPHICS_H_
#define _MACRO_GRAPHICS_H_

/** @defgroup Macro_Graphics Macro_Graphics
 * @{
 * Contains macros related to the vbe.
 */

/**
 * @brief VBE interface used during lab5
 * 
 */
#define VBE_INTERFACE 0x10

/**
 * @brief Linear framebuffer
 * 
 */
#define LINEAR_FRAMEBUFFER BIT(14)

#define BIOS_AH 0x4F

/**
 * @brief Set bve function
 * 
 */
#define SET_VBE_MODE 0x02

/**
 * @brief Get vbe mode function
 * 
 */
#define GET_VBE_MODE 0x01

/**
 * @brief Size of 1 MB
 * 
 */
#define SIZE_MB 1024*1024

/**
 * @brief Indexed color mode
 * 
 */
#define INDEX_COLOR_MODE 0x105

/**
 * @brief RGB of the black color
 * 
 */
#define BLACK_RGB 0

/**@} end of Macro_Graphics*/

#endif
