#ifndef _TIMER_H_
#define _TIMER_H_

#include <lcom/lcf.h>


#include <stdbool.h>
#include <stdint.h>

/** @defgroup Timer Timer
 * @{
 * Contains functions to set up the timer device.
 */

int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribes the interrupts of the timer
 * 
 */
int (timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes the interrupts of the timer
 * 
 */
int (timer_unsubscribe_int)();

/**
 * @brief Handle timer interrupts
 * 
 */
void (timer_int_handler)();

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

/**@} end of Timer*/

#endif
