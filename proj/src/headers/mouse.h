#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

/** @defgroup Mouse Mouse
 * @{
 * Contains functions to set up the video graphics device.
 */

/**
 * @brief Handles mouse interrupts
 * 
 */
void (mouse_ih)();

/**
 * @brief Subscribes the mouse interrupts
 * 
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes the mouse interrupts
 * 
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Set the struct representing the mouse packet
 * 
 * @param array - array containing information
 * @param pp - mouse packet to be constructed
 */
void set_struct(uint8_t array[], struct packet *pp);

/**
 * @brief Sends a mouse command
 * 
 * @param com - command to be sent
 * @return int 
 */
int mouse_command(uint8_t com);

/**
 * @brief Receives a generic command from the output buffer
 * 
 * @param command - received command
 * @return int 
 */
int receive_command(uint8_t *command);

/**
 * @brief Sends generic command to the desired port
 * 
 * @param port - port that will receive the command
 * @param command - command that will be sent
 * @return int 
 */
int send_command(uint8_t port, uint8_t command);

/**
 * @brief Disables mouse
 * 
 * @return int 
 */
int disable_mouse();

/**
 * @brief Enables mouse
 * 
 * @return int 
 */
int enable_mouse_kbc();

int mouse_poll(uint16_t period, uint8_t cnt);

/**@} end of Mouse*/

#endif
