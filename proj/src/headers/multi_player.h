#ifndef _MULTI_PLAYER_H_
#define _MULTI_PLAYER_H_

/** @defgroup Multiplayer Multiplayer
 * @{
 * Contains functions that run multiplayer game
 */

/**
 * @brief Displays initial countdown before multiplayer game starts and keeps the game flowing over time
 * 
 * @param cursor_x - mouse x coordinates
 * @param cursor_y - mouse y coordinates
 * @return int 
 */
int display_initial_countdown_multi(uint16_t cursor_x, uint16_t cursor_y);

/**
 * @brief "Throws" a new object/pill at the beginning of the program or when a collision has been detected for player 1
 * Function keeps track of the pills that will be coming next so that user can think of a strategy to use the pill
 * 
 * @param previous - pill to be drawn
 * @param game_map - game map user1 is playing at
 * @param xi - left top corner of the map x coordinates
 * @param yi - left top corner of the map y coordinates
 * @param size - size of one object in pixels
 * @return int 
 */
int throw_pill_1(int *previous, char game_map[14][8], int xi, int yi, int size);

/**
 * @brief "Throws" a new object/pill at the beginning of the program or when a collision has been detected for player 2
 * Function keeps track of the pills that will be coming next so that user can think of a strategy to use the pill
 * 
 * @param previous - pill to be drawn
 * @param game_map - game map user2 is playing at
 * @param xi - left top corner of the map x coordinates
 * @param yi - left top corner of the map y coordinates
 * @param size - size of one object in pixels
 * @return int 
 */
int throw_pill_2(int *previous, char game_map[14][8], int xi, int yi, int size);

/**
 * @brief Draws numerical info during multiplayer game play
 * 
 */
void draw_numerical_info_multi();

/**
 * @brief Get the last rotation that was performed by player 1
 * 
 * @return int* - number of the rotation (0-3)
 */
int* get_rotation1();

/**
 * @brief Get the last rotation that was performed by player 2
 * 
 * @return int* - number of the rotation (0-3)
 */
int* get_rotation2();

/**
 * @brief Get the horizontal state of the object/pill for player 1
 * 
 * @return true - pill is at horizontal position
 * @return false - pill is at vertical position
 */
bool* get_horizontal1();

/**
 * @brief Get the horizontal state of the object/pill for player 2
 * 
 * @return true - pill is at horizontal position
 * @return false - pill is at vertical position
 */
bool* get_horizontal2();

/**
 * @brief Get the x coordinate the "falling pill" is at the moment for player 1
 * 
 * @return int* - memmory address of the x coordinate
 */
int* getx1();

/**
 * @brief Get the y coordinate the "falling pill" is at the moment for player 1
 * 
 * @return int* - memmory address of the y coordinate
 */
int* gety1();

/**
 * @brief Get the x coordinate the "falling pill" is at the moment for player 2
 * 
 * @return int* - memmory address of the x coordinate
 */
int* getx2();

/**
 * @brief Get the y coordinate the "falling pill" is at the moment for player 2
 * 
 * @return int* - memmory address of the y coordinate
 */
int* gety2();

/**
 * @brief Get the static boolean variable allow_key_movement1 (for player 1) 
 * 
 * @return true - pill movement is allowed
 * @return false - pill movement is not allowed
 */
bool* get_flag1();

/**
 * @brief Get the static boolean variable allow_key_movement2 (for player 2) 
 * 
 * @return true - pill movement is allowed
 * @return false - pill movement is not allowed
 */
bool* get_flag2();

/**
 * @brief Get the static bug_mov_single variable for player 1
 * 
 * @return true - bugs are drawn one way
 * @return false - bugs are drawn a different way
 */
bool* get_bug_mov_multi_1();

/**
 * @brief Get the static bug_mov_single variable for player 2
 * 
 * @return true - bugs are drawn one way
 * @return false - bugs are drawn a different way
 */
bool* get_bug_mov_multi_2();

/**
 * @brief Allows both players to accelerate pill movements by press a key with break code of same value as key_number
 * 
 * @param game_map - game map user is playing at
 * @param key_number - break code of the key that allows pill rotation
 * @param x - x coordenate of the "falling pill"
 * @param y - y coordenate of the "falling pill"
 * @param xi - left top corner of the map x coordinates
 * @param yi - left top corner of the map y coordinates
 * @param allow_key - boolean flag that is true is rotation is allowed 
 * @param array - keyboard array containning make/break codes
 * @param horizontal - true if pill at a horizontal position, false if it is at vertical
 * @param size - size of one object in pixels
 */
void accelerate_mov(char game_map[14][8], int key_number, int *x, int *y, int xi, int yi, bool allow_key, uint8_t array[], bool horizontal, int size);

/**
 * @brief Checks if game has reached its end for player1, either if the user has lost (he is no longer able to throw more pills on to the game map) either he won (there no bugs left in the game map)
 * 
 */
void check_ending_conditions_1();

/**
 * @brief Checks if game has reached its end for player2, either if the user has lost (he is no longer able to throw more pills on to the game map) either he won (there no bugs left in the game map)
 * 
 */
void check_ending_conditions_2();

/**
 * @brief Resets all variables that have changed once multiplayer game play has ended
 * 
 */
void reset_multi_player();

/**@} end of Multiplayer*/

#endif
