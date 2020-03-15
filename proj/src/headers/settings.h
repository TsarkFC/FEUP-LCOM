#ifndef _SETTINGS_H_
#define _SETTINGS_H_

/** @defgroup Settings Settings
 * @{
 * Contains functions that run settings, where player can see and change game time
 */

/**
 * @brief Displays settings menu
 * 
 * @param cursor_x - mouse cursor x coordinate
 * @param cursor_y - mouse cursor y coordinate
 */
void run_settings(uint16_t cursor_x, uint16_t cursor_y);

/**
 * @brief Handles key press while program is at settings state
 * 
 * @param array - keyboard array containning make/break codes
 */
void key_settings(uint8_t array[]);

/**
 * @brief Draws a digital clock representing the time of the day
 * 
 */
void draw_clock();

/**
 * @brief Displays the hour update menu
 * 
 * @param cursor_x - mouse cursor x coordinate
 * @param cursor_y - mouse cursor y coordinate 
 */
void run_hour_update(uint16_t cursor_x, uint16_t cursor_y);

/**
 * @brief Asks user input for an integer and handles his key press response, updating the value of the parameter num
 * 
 * @param array - keyboard array containning make/break codes
 * @param num - integer value updated accordingly to user's response
 */
void ask_for_hours(uint8_t array[], int *num);

/**
 * @brief Sets the range for the parameter num.
 * If num has a value higher that 23 its new value is 23
 * If num has value lower than 0 its new value is 0
 * 
 * @param num 
 * @return int - new value of variable num 
 */
int hours_number(int *num);

/**
 * @brief Sets the range for the parameter num.
 * If num has a value higher that 60 its new value is 60
 * If num has value lower than 0 its new value is 0
 * 
 * @param num 
 * @return int - new value of variable num 
 */
int minutes_number(int *num);

/**
 * @brief Handles input between asks_for_hours functions calls.
 * Allow program to advance from asking the new hours value to asking the new minutes value
 * and finally moving back to previous menu
 * 
 * @param array - keyboard array containning make/break codes
 */
void handle_input(uint8_t array[]);

/**
 * @brief Enumeration representing the state of the program when at the settings menu
 * 
 */
enum settings{
  NOTHING_S,
  TIME_CHANGE,
  GO_BACK_S,
};

/**@} end of Settings*/

#endif
