#ifndef _SINGLE_PLAYER_H_
#define _SINGLE_PLAYER_H_

/** @defgroup Singleplayer Singleplayer
 * @{
 * Contains functions that run Singleplayer game
 */

/**
 * @brief Displays initial countdown before singleplayer game starts and keeps the game flowing over time
 * 
 * @param cursor_x - mouse x coordinates
 * @param cursor_y - mouse y coordinates
 * @return int 
 */
int display_initial_countdown(uint16_t cursor_x, uint16_t cursor_y);

/**
 * @brief Draws the game map on the screen
 * 
 * @param game_map - game map to be drawn
 * @param xi - left top corner of the map x coordinates
 * @param yi - left top corner of the map y coordinates
 * @param size - size of one object in pixels
 * @param bug_movement - boolean flags that changes its value every half of a second so that the player can recognize the objects he is suposed to eliminate
 * @return int 
 */
int draw_game_board(char game_map[14][8], int xi, int yi, int size, bool *bug_movement);

/**
 * @brief "Throws" a new object/pill at the beginning of the program or when a collision has been detected.
 * Function keeps track of the pills that will be coming next so that user can think of a strategy to use the pill
 * 
 * @param previous - pill to be drawn
 * @param game_map - game map user is playing at
 * @param xi - left top corner of the map x coordinates
 * @param yi - left top corner of the map y coordinates
 * @param size - size of one object in pixels
 * @return int 
 */
int throw_pill(int *previous, char game_map[14][8], int xi, int yi, int size);

/**
 * @brief Moves the pill down 1 coordinate over the time, and checks if any collisions occured
 * 
 * @param collision - true if a collision occured, false otherwise
 * @param horizontal - true if pill at a horizontal position, false if it is at vertical
 * @param game_map - game map user is playing at
 * @param x - x coordinate of the pill that is falling
 * @param y - y coordinate of the pill that is falling
 * @param xi - left top corner of the map x coordinates
 * @param yi - left top corner of the map y coordinates
 * @param size - size of one object in pixels
 * @return int 
 */
int pill_movement(int collision, bool horizontal, char game_map[14][8], int *x, int *y, int xi, int yi, int size);

/**
 * @brief Rotates pill when user press a key with a break code that has the same value as the variable key_number
 * 
 * @param array - keyboard array containning make/break codes
 * @param allow_key - boolean flag that is true is rotation is allowed
 * @param game_map - game map user is playing at
 * @param xi - left top corner of the map x coordinates
 * @param yi - left top corner of the map y coordinates
 * @param size - size of one object in pixels
 * @param x - x coordinate of the pill that is falling
 * @param y - y coordinate of the pill that is falling
 * @param rotation - number of the roation to be realized (can only have values ranging from 0 to 3)
 * @param horizontal - true if pill at a horizontal position, false if it is at vertical
 * @param key_number - break code of the key that allow roationnal movements
 * @return int 
 */
int pill_rotation(uint8_t array[], bool allow_key, char game_map[14][8], int xi, int yi, int size, int *x, int *y, int *rotation, bool *horizontal, int key_number);

/**
 * @brief Get the static boolean variable allow_key_movement 
 * 
 * @return true - pill movement is allowed
 * @return false - pill movement is not allowed
 */
bool get_flag();
/**
 * @brief Get the x coordinate the "falling pill" is at the moment
 * 
 * @return int* - memmory address of the x coordinate
 */
int* get_x();
/**
 * @brief Get the y coordinate the "falling pill" is at the moment
 * 
 * @return int* - memmory address of the y coordinate
 */
int* get_y();
/**
 * @brief Get the horizontal state of the object/pill
 * 
 * @return true - pill is at horizontal position
 * @return false - pill is at vertical position
 */
bool* get_horizontal();
/**
 * @brief Get the time the pill was thrown at the game board
 * 
 * @return int* - memmory address of the time variable
 */
int* get_throw_time();
/**
 * @brief Get the last rotation that was performed
 * 
 * @return int* - number of the rotation (0-3)
 */
int* get_rotation();
/**
 * @brief Get the static bug_mov_single variable
 * 
 * @return true - bugs are drawn one way
 * @return false - bugs are drawn a different way
 */
bool* get_bug_mov_single();
/**
 * @brief Gets the first half of the pill that is at the stack
 * 
 * @return char - represents the first half of the pill that is at the stack
 */
char get_stack_1();
/**
 * @brief Gets the second half of the pill that is at the stack
 * 
 * @return char - represents the second half of the pill that is at the stack
 */
char get_stack_2();
/**
 * @brief Get the time the user completed the game with
 * 
 * @return int - time the user completed the game with 
 */
int get_possible_record();

/**
 * @brief Handles keyboard events that move the "falling pill" horizontally
 *
 * @param array - keyboard array containning make/break codes
 * @param allow_key - boolean flag that is true is rotation is allowed
 * @param game_map - game map user is playing at
 * @param xi - left top corner of the map x coordinates
 * @param yi - left top corner of the map y coordinates
 * @param size - size of one object in pixels
 * @param array_pos - has value 1 if keys that perform game commands have two byte break codes, has value 0 if they have one byte break codes
 * @param left_mov - break code of the key that performs left horizontal movements
 * @param right_mov -  break code of the key that performs right horizontal movements
 * @param x - x coordinate of the pill that is falling
 * @param y - y coordinate of the pill that is falling
 * @param horizontal - true if pill at a horizontal position, false if it is at vertical
 * @return int 
 */
int key_playing(uint8_t array[], bool allow_key, char game_map[14][8], int xi, int yi, int size, int array_pos, int left_mov, int right_mov, int *x, int *y, bool horizontal);

/**
 * @brief Moves the "falling pill" vertically when it is in horizontal position
 * 
 * @param x - x coordinate of the pill that is falling
 * @param y - y coordinate of the pill that is falling
 * @param game_map - game map user is playing at
 * @param xi - left top corner of the map x coordinates
 * @param yi - left top corner of the map y coordinates
 * @param size - size of one object in pixels
 */
void update_array_horizontal(int *x, int *y, char game_map[14][8], int xi, int yi, int size);

/**
 * @brief Moves the "falling pill vertically" when it is in vertical position
 * 
 * @param x - x coordinate of the pill that is falling
 * @param y - y coordinate of the pill that is falling
 * @param game_map - game map user is playing at
 * @param xi - left top corner of the map x coordinates
 * @param yi - left top corner of the map y coordinates
 * @param size - size of one object in pixels
 */
void update_array_vertical(int *x, int *y, char game_map[14][8], int xi, int yi, int size);

/**
 * @brief Get the pill color of an object
 * 
 * @param c - char that represents the object the color will be taken from
 * @return int - number of the color (1 for blue, 2 for red, 3 for yellow)
 */
int get_pill_color(char c);

/**
 * @brief Detects collisions between objects
 * 
 * @param game_map - game map user is playing at
 * @param xi - left top corner of the map x coordinates
 * @param yi - left top corner of the map y coordinates
 * @param size - size of one object in pixels
 * @param horizontal - true if pill at a horizontal position, false if it is at vertical
 * @param x - x coordinate of the pill that is falling
 * @param y - y coordinate of the pill that is falling
 * @param first_throw - is set to true after the function is called if there was any collisions, so that a new pill is thrown and game keeps flowing, false otherwise
 * @return true - if any collision was detected
 * @return false - if there was no collisions
 */
bool collision_detector(char game_map[14][8], int xi, int yi, int size, bool horizontal, int *x, int *y, bool *first_throw);

/**
 * @brief Calls function "collision_detector" only in specific times of the game 
 * 
 * @param game_map - game map user is playing at
 * @param xi - left top corner of the map x coordinates
 * @param yi - left top corner of the map y coordinates
 * @param size - size of one object in pixels
 * @param horizontal - true if pill at a horizontal position, false if it is at vertical
 * @param x - x coordinate of the pill that is falling
 * @param y - y coordinate of the pill that is falling
 * @param first_throw - is set to true after the function is called if there was any collisions, so that a new pill is thrown and game keeps flowing, false otherwise
 * @return true - if any collision was detected
 * @return false - if there was no collisions
 */
bool timed_collision_detector(char game_map[14][8], int xi, int yi, int size, bool horizontal, int *x, int *y, bool *first_throw);

/**
 * @brief Checks if there are any four consecutive objects in horizontal directions
 * 
 * @param game_map - game map user is playing at
 */
void check_horizontal(char game_map[14][8]);

/**
 * @brief Checks if there are any four consecutive objects in vertical directions
 * 
 * @param game_map - game map user is playing at
 */
void check_vertical(char game_map[14][8]);

/**
 * @brief Makes pills that are isolated in the game map fall, as if there was gravity
 * 
 * @param game_map - game map user is playing at
 */
void update_lost_pills(char game_map[14][8]);

/**
 * @brief Calls function "update_lost_pills" only in specific times of the game 
 * 
 * @param game_map - game map user is playing at
 */
void timed_fall_lost_pills(char game_map[14][8]);

/**
 * @brief Displays pill that will be thrown when next collision is detected
 * 
 * @param pill_no - number representing the pill that will be drawn next
 * @param x - x coordinate where next pill is drawn
 * @param y - y coordinate where next pill is drawn
 * @param pill_length - size of the pill in pixels
 */
void show_next(int pill_no, int x, int y, int pill_length);

/**
 * @brief Displays a number on the screen
 * 
 * @param number - number to be displayed
 * @param x - x coordinate where number will be displayed
 * @param y - y coordinate where number will be displayed
 * @param num_size - size of one digit in pixels
 */
void display_number(int number, int x, int y, int num_size);

/**
 * @brief Keeps track of the number of bugs that are on the game map
 * 
 * @param game_map - game map user is playing at
 * @return int - number of bugs that are on the game map
 */
int bug_counter(char game_map[14][8]);

/**
 * @brief Draws numerical info during singleplayer game play
 * 
 */
void draw_numerical_info();

/**
 * @brief Checks if game has reached its end, either if the user has lost (he is no longer able
 * to throw more pills on to the game map) either he won (there no bugs left in the game map)
 * 
 */
void check_ending_conditions();

/**
 * @brief Allows user to accelerate a pill falling movement by realizing a descendent movement with the mouse while holding the left mouse button
 * 
 * @param left_click - if true left click is pressed
 * @param x - x coordinate of the pill that is falling
 * @param y - y coordinate of the pill that is falling
 * @param previous_x - previous x mouse coordinate
 * @param previous_y - previous y mouse coordinate
 * @param new_x - new x mouse coordinate
 * @param new_y - new y mouse coordinate
 * @param xi - left top corner of the map x coordinates
 * @param yi - left top corner of the map y coordinates
 * @param allow_key - boolean flag that is true is rotation is allowed
 * @param horizontal - true if pill at a horizontal position, false if it is at vertical 
 * @param size - size of one object in pixels
 * @param game_map - game map user is playing at
 */
void mouse_accelerate(bool left_click, int *x, int *y, int previous_x, int previous_y, int new_x, int new_y, int xi, int yi, bool allow_key, bool horizontal, int size, char game_map[14][8]);

/**
 * @brief 
 * 
 * @param game_map 
 * @param xi 
 * @param yi 
 * @param x 
 * @param y 
 * @param size 
 * @param previous_x 
 * @param previous_y 
 * @param new_x 
 * @param new_y 
 * @param left_click 
 * @param allow_key 
 * @param horizontal 
 * @return true 
 * @return false 
 */
bool mouse_send_to_stack(char game_map[14][8], int xi, int yi, int *x, int *y, int size, int previous_x, int previous_y, int new_x, int new_y, bool left_click, bool allow_key, bool *horizontal);

/**
 * @brief Displays pill that is at stack
 * 
 * @param c1 - first part of the pill that is at the stack
 * @param c2 - second part of the pill that is at the stack
 */
void draw_stack(char c1, char c2);

/**
 * @brief Resets all variables that have changed once game play has ended
 * 
 */
void reset_single_player();

/**@} end of Singleplayer*/

#endif
