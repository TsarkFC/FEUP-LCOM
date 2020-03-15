#ifndef _MACRO_MOUSE_H_
#define _MACRO_MOUSE_H_

/** @defgroup Macro_Mouse Macro_Mouse
 * @{
 * Contains macros related to the mouse device.
 */


/**
 * @brief Mouse IRQ line
 * 
 */
#define MOUSE_IRQ 12
#define AUX BIT(5)

/**
 * @brief Mouse left button
 * 
 */
#define LB BIT(0)

/**
 * @brief Mouse right button
 * 
 */
#define RB BIT(1)

/**
 * @brief Mouse middle button
 * 
 */
#define MB BIT(2)

/**
 * @brief x axis displacement sign
 * 
 */
#define X_SIGN BIT(4)

/**
 * @brief y axis displacement sign
 * 
 */
#define Y_SIGN BIT(5)

/**
 * @brief Allows overflow detection of x displacement
 * 
 */
#define X_OVFL BIT(6)

/**
 * @brief Allows overflow detection of y displacement
 * 
 */
#define Y_OVFL BIT(7)

/**
 * @brief Allows to overcome overflow operation using 2 complement operation
 * 
 */
#define NEG_TO_POS BIT(8)

#define WRT_BYTE_MOUSE 0xD4

/**
 * @brief Error detection
 * 
 */
#define ACK 0xFA

/**
 * @brief Error detection
 * 
 */
#define NACK 0xFE

/**
 * @brief Error detection
 * 
 */
#define ERROR 0xFC

/**
 * @brief Enable mouse data reporting register
 * 
 */
#define EN_DATA_REPOR 0xF4

/**
 * @brief Disable mouse data reporting register
 * 
 */
#define DISABLE_DATA_REPOR 0xF5

/**
 * @brief Mouse disable
 * 
 */
#define MOU_DIS BIT(5)

/**
 * @brief First byte of mouse packet
 * 
 */
#define FIRST_BYTE BIT(3)

/**
 * @brief Helps on enabling interrupts 
 * 
 */
#define EN_MOU_KBC_INT BIT(0)|BIT(1)

/**
 * @brief Helpds on enabling mouse
 * 
 */
#define EN_MOU_KBC 0xCF

/**
 * @brief Mouse remote mode
 * 
 */
#define REMOTE_MODE 0xF0

/**
 * @brief Mouse stream mode
 * 
 */
#define STREAM_MODE 0xEA

/**
 * @brief Read data from mouse register
 * 
 */
#define READ_DATA 0xEB

/**@} end of Macro_Mouse*/

#endif
