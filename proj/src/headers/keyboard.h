#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <lcom/lcf.h>


#include <stdbool.h>
#include <stdint.h>

/** @defgroup Keyboard Keyboard
 * @{
 * Contains functions to set up the keyboard device.
 */

/**
 * @brief Subscribes keyboard's interrupts
 * 
 */
int (kbc_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts
 * 
 */
int (kbc_unsubscribe_int)();

/**
 * @brief Keyboard interrupt handler
 * 
 */
void (kbc_ih)();

/**
 * @brief Gets a key scancode
 * 
 */
int (get_scancode)();

/**
 * @brief Enables interrupts after polling operations
 * 
 */
int (enable_interrupts)();

/**@} end of Keyboard*/

#endif
