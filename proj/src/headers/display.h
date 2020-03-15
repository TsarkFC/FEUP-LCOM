#ifndef _DISPLAY_H_
#define _DISPLAY_H_


//Pixmaps needed in this file
#include "images/main_menu.h"
#include "images/main_menu1.h"
#include "images/main_menu2.h"
#include "images/main_menu3.h"
#include "images/cursor.h"
#include "images/single_menu.h"
#include "images/single_menu_lv1.h"
#include "images/single_menu_lv2.h"
#include "images/single_menu_lv3.h"
#include "images/single_menu_go_back.h"
#include "images/multi_menu.h"
#include "images/multi_menu_lv1.h"
#include "images/multi_menu_lv2.h"
#include "images/multi_menu_lv3.h"
#include "images/multi_menu_go_back.h"
#include "images/single_player_3.h"
#include "images/single_player_2.h"
#include "images/single_player_1.h"
#include "images/single_player_GO.h"
#include "images/single_player.h"
#include "images/blue_bug_1.h"
#include "images/blue_bug_2.h"
#include "images/red_bug_1.h"
#include "images/red_bug_2.h"
#include "images/yellow_bug_1.h"
#include "images/yellow_bug_2.h"
#include "images/blue_pill.h"
#include "images/red_pill.h"
#include "images/yellow_pill.h"
#include "images/blue_big_pill.h"
#include "images/red_big_pill.h"
#include "images/yellow_big_pill.h"
#include "images/0.h"
#include "images/1.h"
#include "images/2.h"
#include "images/3.h"
#include "images/4.h"
#include "images/5.h"
#include "images/6.h"
#include "images/7.h"
#include "images/8.h"
#include "images/9.h"
#include "images/multi_player.h"
#include "images/multi_player_1.h"
#include "images/multi_player_2.h"
#include "images/multi_player_3.h"
#include "images/multi_player_go.h"
#include "images/pause.h"
#include "images/pause_continue.h"
#include "images/pause_exit.h"
#include "images/gameover.h"
#include "images/success.h"
#include "images/player1_won.h"
#include "images/player2_won.h"
#include "images/hour_change.h"
#include "images/hour_change_time.h"
#include "images/hour_change_back.h"
#include "images/settings.h"
#include "images/select_1.h"
#include "images/select_2.h"
#include "images/waiting_connection.h"
#include "images/instructions_multi.h"
#include "images/instructions_single.h"

//Macros
#include "macro.h"
#include "macro_graphics.h"
#include "macro_mouse.h"
#include "i8254.h"

//Files from labs
#include "graphics.h"
#include "timer.h"

/** @defgroup Display Display
 * @{
 * Displays menus.
 * Also handles mouse and keyboard information
 */

/**
 * @brief Loads all images at the beginning of the program
 * 
 * @return int (0)
 */
int load_images();

/**
 * @brief Displays the main menu of the project
 * 
 * @param cursor_x - mouse x coordinates
 * @param cursor_y - mouse y coordinates
 * @return int (0)
 */

int display_menu(uint16_t cursor_x, uint16_t cursor_y);
/**
 * @brief Displays the singleplayer menu of the program
 * 
 * @param cursor_x - mouse x coordinates
 * @param cursor_y - mouse y coordinates
 * @return int 
 */
int display_single_menu(uint16_t cursor_x, uint16_t cursor_y);

/**
 * @brief Displays the multiplayer menu of the program
 * 
 * @param cursor_x - mouse x coordinates
 * @param cursor_y - mouse y coordinates
 * @return int 
 */
int display_multi_menu(uint16_t cursor_x, uint16_t cursor_y);


/**
 * @brief Handles the interrupts caused by keyboard and calls secondary functions depending on which state the program is at
 * 
 * @param array - keyboard array containning make/break codes
 * @return int 
 */
int key_handler(uint8_t array[]);

/**
 * @brief Handles key press during the main menu and redirects user to secondary menus
 * 
 * @param array - keyboard array containning make/break codes
 * @return int 
 */
int key_main_menu(uint8_t array[]);

/**
 * @brief Handles key press during the singleplayer menu and redirects user to the game
 * 
 * @param array - keyboard array containning make/break codes
 * @return int 
 */
int key_level_menu_s(uint8_t array[]);

/**
 * @brief Handles key press during the multiplayer menu and redirects user to the game
 * 
 * @param array - keyboard array containning make/break codes
 * @return int 
 */
int key_level_menu_m(uint8_t array[]);

/**
 * @brief Handles key press during device selection in serial port game play
 * 
 * @param array - keyboard array containning make/break codes
 * @param device - device in which the user is playing (1 or 2)
 */
void key_handler_serial(uint8_t array[], int device);

/**
 * @brief Handles all mouse interrupts cause during the program, from button press to movement detection
 * 
 * @param pp - struct mouse packet
 * @param cursor_x - mouse x coordinates
 * @param cursor_y - mouse y coordinates
 * @return int 
 */
int mouse_handler(struct packet pp, int *cursor_x, int *cursor_y);

/**
 * @brief Draws the mouse cursor during all the program and continuously refreshes the background depending on the state of the game
 * 
 * @param img - backgound image to be drawn "behind" the mouse cursor
 * @param ind - index of the image in the array that contains all the images of the program
 * @param cursor_x - mouse x coordinates
 * @param cursor_y - mouse y coordinates
 * @return int 
 */
int mouse_cursor(xpm_image_t img, int ind, uint16_t cursor_x, uint16_t cursor_y);

/**
 * @brief Displays the pause menu
 * 
 */
void display_pause();

/**
 * @brief Handles key press during the pause menus
 * 
 * @param s_m - char that has the value 's' if user is in singleplayer mode and 'm' if user is in multiplayer
 * @param num - integer that represents the level the user is at
 * @param array - keyboard array containning make/break codes
 */
void key_pause(char s_m, int num, uint8_t array[]);

/**
 * @brief Gets the device the user is at during serial port gaming (1 or 2)
 * 
 * @return int - device number (1 or 2)
 */
int get_device();

/**
 * @brief Sets the device the user is at during serial port gaming (1 or 2)
 * 
 * @param d - number of the device to be set
 */
void set_device(int d);

/**
 * @brief Gets the static boolean variable that has true value if the program is going to close
 * 
 * @return true - program will shut down
 * @return false - program will continue to run
 */
bool get_out_f();

/**
 * @brief Enumeration that represents the state the program is at during main menu
 * 
 */
enum display{
  MAIN_MENU,
  SINGLE_PLAYER,
  MULTI_PLAYER,
  SERIAL
};

/**
 * @brief Enumeration that represents the state the program is at during singleplayer menu
 * 
 */
enum display_single{
  SINGLE_MENU,
  LEVEL1,
  LEVEL2,
  LEVEL3,
  GO_BACK
};

/**
 * @brief Enumeration that represents the state the program is at during multiplayer menu
 * 
 */
enum display_multi{
  MULTI_MENU,
  LEVEL1_M,
  LEVEL2_M,
  LEVEL3_M,
  GO_BACK_M
};

/**
 * @brief Enumeration that represents the state the program is at
 * 
 */
enum state_of_program{
  IN_MAIN_MENU,

  IN_S_PLAYING_1,
  IN_S_PLAYING_2,
  IN_S_PLAYING_3,

  IN_M_PLAYING_1,
  IN_M_PLAYING_2,
  IN_M_PLAYING_3,

  SEL_DEVICE,
  WAIT_CONNECTION,
  IN_SERIAL_PLAYING,

  IN_LEVEL_SEL_S,
  IN_LEVEL_SEL_M,
  S_WON_GAME,
  S_LOST_GAME,
  M1_WON_GAME,
  M2_WON_GAME,

  PAUSE_S1,
  PAUSE_S2,
  PAUSE_S3,
  PAUSE_M1,
  PAUSE_M2,
  PAUSE_M3,

  SETTINGS, 
  NEW_HOUR
};

/**
 * @brief Enumeration that represents the state the program is at during pause menu
 * 
 */
enum select_pause{
  PLAY,
  EXIT,
  NOTHING,
};

/**
 * @brief Enumeration that represents the state the program is when user is selecting the device he is at (Serial port)
 * 
 */
enum select_device{
  ONE,
  TWO
};

/**@} end of Display*/

#endif
