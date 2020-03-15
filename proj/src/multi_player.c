// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "headers/display.h"
#include "headers/multi_player.h"
#include "headers/single_player.h"
#include "headers/rtc.h"
#include "headers/serial_port.h"

extern int counter;
extern xpm_image_t im[];

extern void* video_mem;
extern unsigned bits_per_pixel;
extern uint8_t *double_buffer; //DOUBLE BUFFERING DECALRED IN MAIN LOOP

extern enum display menu;
extern enum display_single single_menu;
extern enum display_multi multi_menu;
extern enum state_of_program state;  
extern enum select_pause pause_sel;

char player1_map_1[14][8] = {{'0','0','0','0','0','0','0','0'}, //Non visiible line if there
                                                                 //is any peice in here player has 
                                                                 //lost the game
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','3','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'2','0','0','2','2','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','2'},
                              {'1','0','0','2','0','3','0','1'},
                              };

char player2_map_1[14][8] = {{'0','0','0','0','0','0','0','0'}, //Non visiible line if there
                                                                 //is any peice in here player has 
                                                                 //lost the game
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','3','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'2','0','0','2','2','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','2'},
                              {'1','0','0','2','0','3','0','1'},
                              };

static char safe_map_1[14][8] = {{'0','0','0','0','0','0','0','0'}, //Non visiible line if there
                                                                 //is any peice in here player has 
                                                                 //lost the game
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','3','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'2','0','0','2','2','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','2'},
                              {'1','0','0','2','0','3','0','1'},
                              };                              

char player1_map_2[14][8] = {{'0','0','0','0','0','0','0','0'}, //Non visiible line if there
                                                                 //is any peice in here player has 
                                                                 //lost the game
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'1','0','0','0','0','3','0','0'},
                              {'1','0','0','0','1','0','0','0'},
                              {'1','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','2','0','0','0','2','0'},
                              {'3','0','0','0','0','1','3','2'},
                              {'1','0','0','2','0','0','0','2'},
                              {'1','0','0','2','1','3','0','1'},
                              }; 

char player2_map_2[14][8] = {{'0','0','0','0','0','0','0','0'}, //Non visiible line if there
                                                                 //is any peice in here player has 
                                                                 //lost the game
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'1','0','0','0','0','3','0','0'},
                              {'1','0','0','0','1','0','0','0'},
                              {'1','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','2','0','0','0','2','0'},
                              {'3','0','0','0','0','1','3','2'},
                              {'1','0','0','2','0','0','0','2'},
                              {'1','0','0','2','1','3','0','1'},
                              }; 

char safe_map_2[14][8] = {{'0','0','0','0','0','0','0','0'}, //Non visiible line if there
                                                                 //is any peice in here player has 
                                                                 //lost the game
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'1','0','0','0','0','3','0','0'},
                              {'1','0','0','0','1','0','0','0'},
                              {'1','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','2','0','0','0','2','0'},
                              {'3','0','0','0','0','1','3','2'},
                              {'1','0','0','2','0','0','0','2'},
                              {'1','0','0','2','1','3','0','1'},
                              };                              

char player1_map_3[14][8] = {{'0','0','0','0','0','0','0','0'}, //Non visiible line if there
                                                                 //is any peice in here player has 
                                                                 //lost the game
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'3','0','0','0','0','0','0','0'},
                              {'0','0','3','0','0','0','2','0'},
                              {'1','0','0','0','0','3','0','1'},
                              {'1','0','0','0','1','0','0','0'},
                              {'2','0','0','2','2','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','2','0','0','0','2','0'},
                              {'3','0','0','0','0','1','3','3'},
                              {'0','0','1','0','0','0','0','2'},
                              {'1','0','0','2','1','3','0','1'},
                              }; 

char player2_map_3[14][8] = {{'0','0','0','0','0','0','0','0'}, //Non visiible line if there
                                                                 //is any peice in here player has 
                                                                 //lost the game
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'3','0','0','0','0','0','0','0'},
                              {'0','0','3','0','0','0','2','0'},
                              {'1','0','0','0','0','3','0','1'},
                              {'1','0','0','0','1','0','0','0'},
                              {'2','0','0','2','2','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','2','0','0','0','2','0'},
                              {'3','0','0','0','0','1','3','3'},
                              {'0','0','1','0','0','0','0','2'},
                              {'1','0','0','2','1','3','0','1'},
                              };

char safe_map_3[14][8] = {{'0','0','0','0','0','0','0','0'}, //Non visiible line if there
                                                                 //is any peice in here player has 
                                                                 //lost the game
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'3','0','0','0','0','0','0','0'},
                              {'0','0','3','0','0','0','2','0'},
                              {'1','0','0','0','0','3','0','1'},
                              {'1','0','0','0','1','0','0','0'},
                              {'2','0','0','2','2','0','0','0'},
                              {'0','0','0','0','0','0','0','0'},
                              {'0','0','2','0','0','0','2','0'},
                              {'3','0','0','0','0','1','3','3'},
                              {'0','0','1','0','0','0','0','2'},
                              {'1','0','0','2','1','3','0','1'},
                              };                              


static bool first_time_initial = true; //boolean variable to check the first time function
                                       //-> display_initial_countdown is called

static int initial_time; //initial time for countdown

static int rotation1 = 0;
static int rotation2 = 0;
int* get_rotation1(){return &rotation1;}
int* get_rotation2(){return &rotation2;}

static bool first_throw1 = true;
static bool first_throw2 = true;

static bool bug_movement_multi_1 = true;
bool* get_bug_mov_multi_1() {return &bug_movement_multi_1;}
static bool bug_movement_multi_2 = true;
bool* get_bug_mov_multi_2() {return &bug_movement_multi_2;}

static bool horizontal1 = true;
static bool horizontal2 = true;
bool* get_horizontal1(){return &horizontal1;}
bool* get_horizontal2(){return &horizontal2;}

static int throw_time1;
static int throw_time2;

//PILL COORDINATES FOR BOTH PLAYERS
static int x1 = 145 + 3*35;
static int y1 = 290;
int* getx1(){return &x1;}
int* gety1(){return &y1;}

static int x2 = 720 + 3*35;
static int y2 = 290;
int* getx2(){return &x2;}
int* gety2(){return &y2;}

//flag to allow key movement
static bool allow_key_movement1 = false;
static bool allow_key_movement2 = false;
bool* get_flag1(){return &allow_key_movement1;}
bool* get_flag2(){return &allow_key_movement2;}

int display_initial_countdown_multi(uint16_t cursor_x, uint16_t cursor_y){

  /*display background based on time of the day*/
  draw_background_color();


  if (first_time_initial){
    initial_time = counter; 
    first_time_initial = false;
  }

  xpm_image_t img;
  int ind = 0;

  //DISPLAY NUMBER 3 ON COUNTDOWN
  if (initial_time + 60 > counter){
    vg_draw_pixmap(im[45], multiplayer_game_3_xpm,0,0);
    img = im[45];
    ind = 45;
  }

  //DISPLAY NUMBER 2 ON COUNTDOWN
  else if (initial_time + 120 > counter){
    vg_draw_pixmap(im[44], multiplayer_game_2_xpm,0,0);
    img = im[44];
    ind = 44;
  }

  //DISPLAY NUMBER 1 ON COUNTDOWN
  else if (initial_time + 180 > counter){
    vg_draw_pixmap(im[43], multiplayer_game_1_xpm,0,0);
    img = im[43];
    ind = 43;
  }

  //SHOWN "GO!" MESSAGE TO PLAYER
  else if (initial_time + 200 > counter){
    vg_draw_pixmap(im[46], multiplayer_game_go_xpm,0,0);
    img = im[46];
    ind = 46;
  }

  //STARTS GAME
  else if (state == IN_M_PLAYING_1){
    //Update to draw game
    vg_draw_pixmap(im[42], multiplayer_game_xpm, 0, 0);
    pill_movement(timed_collision_detector(player1_map_1, 145, 290, 35, horizontal1, &x1, &y1, &first_throw1), horizontal1, player1_map_1, &x1, &y1, 145, 290, 35);
    pill_movement(timed_collision_detector(player2_map_1, 720, 290, 35, horizontal2, &x2, &y2, &first_throw2), horizontal2, player2_map_1, &x2, &y2, 720, 290, 35);
    img = im[42];
    ind = 0;
  }
  else if (state == IN_M_PLAYING_2){
    //Update to draw game
    vg_draw_pixmap(im[42], multiplayer_game_xpm, 0, 0);
    pill_movement(timed_collision_detector(player1_map_2, 145, 290, 35, horizontal1, &x1, &y1, &first_throw1), horizontal1, player1_map_2, &x1, &y1, 145, 290, 35);
    pill_movement(timed_collision_detector(player2_map_2, 720, 290, 35, horizontal2, &x2, &y2, &first_throw2), horizontal2, player2_map_2, &x2, &y2, 720, 290, 35);
    img = im[42];
    ind = 0;
  }

  else if (state == IN_M_PLAYING_3){
    //Update to draw game
    vg_draw_pixmap(im[42], multiplayer_game_xpm, 0, 0);
    pill_movement(timed_collision_detector(player1_map_3, 145, 290, 35, horizontal1, &x1, &y1, &first_throw1), horizontal1, player1_map_3, &x1, &y1, 145, 290, 35);
    pill_movement(timed_collision_detector(player2_map_3, 720, 290, 35, horizontal2, &x2, &y2, &first_throw2), horizontal2, player2_map_3, &x2, &y2, 720, 290, 35);
    img = im[42];
    ind = 0;
  }

  else if (state == IN_SERIAL_PLAYING){
    vg_draw_pixmap(im[42], multiplayer_game_xpm, 0, 0);
    pill_movement(timed_collision_detector(player1_map_2, 145, 290, 35, horizontal1, &x1, &y1, &first_throw1), horizontal1, player1_map_2, &x1, &y1, 145, 290, 35);
    pill_movement(timed_collision_detector(player2_map_2, 720, 290, 35, horizontal2, &x2, &y2, &first_throw2), horizontal2, player2_map_2, &x2, &y2, 720, 290, 35);
    img = im[42];
    ind = 0;
  }

  //Board is drawn inside mouse_cursor
  if (mouse_cursor(img, ind, cursor_x, cursor_y)) return 1;

  memcpy(video_mem, double_buffer, 1152 * 864 * ((bits_per_pixel + 7)/8));

  return 0;
}

int throw_pill_1(int *previous, char game_map[14][8], int xi, int yi, int size){
  static int pill_no;
  static int drawing;

  if (first_throw1){
    //reset variables for new movement
    rotation1 = 0;
    horizontal1 = true;
    throw_time1 = counter;
    allow_key_movement1 = false;

    //check four in line and solo pills
    check_horizontal(game_map);
    check_vertical(game_map);
    timed_fall_lost_pills(game_map);
    
    first_throw1 = false;
    pill_no = rand() % 9; //generates new pill to be drawn in map

    drawing = *previous; //gets pill that will be drawn in game map
    *previous = pill_no; //generates new pill that will be displayed in top right corner

    //check ending conditions
    check_ending_conditions_1();
  } 

  //draw next pill
  show_next(*previous, 30, 53, 80);

  //draw beginning pill for 1 sec
  if (throw_time1 + 60 > counter){
    if (drawing == 0){
      vg_draw_pixmap(im[26], blue_pill_xpm, xi + 3*size, yi - size);
      vg_draw_pixmap(im[26], blue_pill_xpm, xi + 4*size, yi - size);
    }
    else if (drawing == 1){
      vg_draw_pixmap(im[26], blue_pill_xpm, xi + 3*size, yi - size);
      vg_draw_pixmap(im[27], red_pill_xpm, xi + 4*size, yi - size);
    }
    else if (drawing == 2){
      vg_draw_pixmap(im[26], blue_pill_xpm, xi + 3*size, yi - size);
      vg_draw_pixmap(im[28], yellow_pill_xpm, xi + 4*size, yi - size);
    }
    else if (drawing == 3){
      vg_draw_pixmap(im[27], red_pill_xpm, xi + 3*size, yi - size);
      vg_draw_pixmap(im[26], blue_pill_xpm, xi + 4*size, yi - size);
    }
    else if (drawing == 4){
      vg_draw_pixmap(im[27], red_pill_xpm, xi + 3*size, yi - size);
      vg_draw_pixmap(im[27], red_pill_xpm, xi + 4*size, yi - size);
    }
    else if (drawing == 5){
      vg_draw_pixmap(im[27], red_pill_xpm, xi + 3*size, yi - size);
      vg_draw_pixmap(im[28], yellow_pill_xpm, xi + 4*size, yi - size);
    }
    else if (drawing == 6){
      vg_draw_pixmap(im[28], yellow_pill_xpm, xi + 3*size, yi - size);
      vg_draw_pixmap(im[26], blue_pill_xpm, xi + 4*size, yi - size);
    }
    else if (drawing == 7){
      vg_draw_pixmap(im[28], yellow_pill_xpm, xi + 3*size, yi - size);
      vg_draw_pixmap(im[27], red_pill_xpm, xi + 4*size, yi - size);
    }
    else if (drawing == 8){
      vg_draw_pixmap(im[28], yellow_pill_xpm, xi + 3*size, yi - size);
      vg_draw_pixmap(im[28], yellow_pill_xpm, xi + 4*size, yi - size);
    }
  }

  else if (throw_time1 + 61 > counter){

    if (drawing == 0){
      game_map[1][3] = 'b';
      game_map[1][4] = 'b';
    }
    else if (drawing == 1){
      game_map[1][3] = 'b';
      game_map[1][4] = 'r';
    }
    else if (drawing == 2){
      game_map[1][3] = 'b';
      game_map[1][4] = 'y';
    }
    else if (drawing == 3){
      game_map[1][3] = 'r';
      game_map[1][4] = 'b';
    }
    else if (drawing == 4){
      game_map[1][3] = 'r';
      game_map[1][4] = 'r';
    }
    else if (drawing == 5){
      game_map[1][3] = 'r';
      game_map[1][4] = 'y';
    }
    else if (drawing == 6){
      game_map[1][3] = 'y';
      game_map[1][4] = 'b';
    }
    else if (drawing == 7){
      game_map[1][3] = 'y';
      game_map[1][4] = 'r';
    }
    else if (drawing == 8){
      game_map[1][3] = 'y';
      game_map[1][4] = 'y';
    }

    //Flag to allow keyboard events
    allow_key_movement1 = true;
  }

  return 0;
}

int throw_pill_2(int *previous, char game_map[14][8], int xi, int yi, int size){
  static int pill_no;
  static int drawing;

  if (first_throw2){
    //reset variables for new movement
    rotation2 = 0;
    horizontal2 = true;
    throw_time2 = counter;
    allow_key_movement2 = false;

    //check four in line and solo pills
    check_horizontal(game_map);
    check_vertical(game_map);
    timed_fall_lost_pills(game_map);
    
    first_throw2 = false;
    pill_no = rand() % 9; //generates new pill to be drawn in map

    drawing = *previous; //gets pill that will be drawn in game map
    *previous = pill_no; //generates new pill that will be displayed in top right corner

    //check ending conditions
    check_ending_conditions_2();
  } 

  //draw next pill
  show_next(*previous, 965, 53, 80);

  //draw beginning pill for 1 sec
  if (throw_time2 + 60 > counter){
    if (drawing == 0){
      vg_draw_pixmap(im[26], blue_pill_xpm, xi + 3*size, yi - size);
      vg_draw_pixmap(im[26], blue_pill_xpm, xi + 4*size, yi - size);
    }
    else if (drawing == 1){
      vg_draw_pixmap(im[26], blue_pill_xpm, xi + 3*size, yi - size);
      vg_draw_pixmap(im[27], red_pill_xpm, xi + 4*size, yi - size);
    }
    else if (drawing == 2){
      vg_draw_pixmap(im[26], blue_pill_xpm, xi + 3*size, yi - size);
      vg_draw_pixmap(im[28], yellow_pill_xpm, xi + 4*size, yi - size);
    }
    else if (drawing == 3){
      vg_draw_pixmap(im[27], red_pill_xpm, xi + 3*size, yi - size);
      vg_draw_pixmap(im[26], blue_pill_xpm, xi + 4*size, yi - size);
    }
    else if (drawing == 4){
      vg_draw_pixmap(im[27], red_pill_xpm, xi + 3*size, yi - size);
      vg_draw_pixmap(im[27], red_pill_xpm, xi + 4*size, yi - size);
    }
    else if (drawing == 5){
      vg_draw_pixmap(im[27], red_pill_xpm, xi + 3*size, yi - size);
      vg_draw_pixmap(im[28], yellow_pill_xpm, xi + 4*size, yi - size);
    }
    else if (drawing == 6){
      vg_draw_pixmap(im[28], yellow_pill_xpm, xi + 3*size, yi - size);
      vg_draw_pixmap(im[26], blue_pill_xpm, xi + 4*size, yi - size);
    }
    else if (drawing == 7){
      vg_draw_pixmap(im[28], yellow_pill_xpm, xi + 3*size, yi - size);
      vg_draw_pixmap(im[27], red_pill_xpm, xi + 4*size, yi - size);
    }
    else if (drawing == 8){
      vg_draw_pixmap(im[28], yellow_pill_xpm, xi + 3*size, yi - size);
      vg_draw_pixmap(im[28], yellow_pill_xpm, xi + 4*size, yi - size);
    }
  }

  else if (throw_time2 + 61 > counter){

    if (drawing == 0){
      game_map[1][3] = 'b';
      game_map[1][4] = 'b';
    }
    else if (drawing == 1){
      game_map[1][3] = 'b';
      game_map[1][4] = 'r';
    }
    else if (drawing == 2){
      game_map[1][3] = 'b';
      game_map[1][4] = 'y';
    }
    else if (drawing == 3){
      game_map[1][3] = 'r';
      game_map[1][4] = 'b';
    }
    else if (drawing == 4){
      game_map[1][3] = 'r';
      game_map[1][4] = 'r';
    }
    else if (drawing == 5){
      game_map[1][3] = 'r';
      game_map[1][4] = 'y';
    }
    else if (drawing == 6){
      game_map[1][3] = 'y';
      game_map[1][4] = 'b';
    }
    else if (drawing == 7){
      game_map[1][3] = 'y';
      game_map[1][4] = 'r';
    }
    else if (drawing == 8){
      game_map[1][3] = 'y';
      game_map[1][4] = 'y';
    }

    //Flag to allow keyboard events
    allow_key_movement2 = true;
  }

  return 0;
}

void accelerate_mov(char game_map[14][8], int key_number, int *x, int *y, int xi, int yi, bool allow_key, uint8_t array[], bool horizontal, int size){
  if (!allow_key) return;

  if (array[0] == TWO_BYTE){if (array[1] != key_number) return;}
  else if (array[0] != key_number) return;

  if (state == IN_SERIAL_PLAYING){
    if (get_device() == 1) send_info(key_number, 1);
    else if (get_device() == 2) send_info(key_number, 2);
  }

  bool temp = false;
  int aux_x = *x;
  int aux_y = *y + 35;

  if (collision_detector(game_map, xi, yi, size, horizontal, &aux_x, &aux_y, &temp)) return;
  if (collision_detector(game_map, xi, yi, size, horizontal, x, y, &temp)) return;

  if (horizontal){
    if ((*y - yi) / size == 13) return;

    //Set new coordinates
    game_map[(*y - yi)/size + 1][(*x - xi)/size] = game_map[(*y - yi)/size][(*x - xi)/size];
    game_map[(*y - yi)/size + 1][(*x - xi)/size + 1] = game_map[(*y - yi)/size][(*x - xi)/size + 1];
    
    //Zero previous coordinates
    game_map[(*y - yi)/size][(*x - xi)/size] = '0';
    game_map[(*y - yi)/size][(*x - xi)/size + 1] = '0';
  }

  else{
    if ((*y - yi) / size + 1 == 13) return;
    //Update array during vertical movement
    game_map[(*y - yi)/size + 2][(*x - xi)/size] = game_map[(*y - yi)/size + 1][(*x - xi)/size];
    game_map[(*y - yi)/size + 1][(*x - xi)/size] = game_map[(*y - yi)/size][(*x - xi)/size];
    game_map[(*y - yi)/size][(*x - xi)/size] = '0';
  }

  *y += 35;
}

void check_ending_conditions_1(){
  
  //WINNING CONDITION
  if (state == IN_M_PLAYING_1){
    if (bug_counter(player1_map_1) == 0) state = M1_WON_GAME;
  }
  else if (state == IN_M_PLAYING_2 || state == IN_SERIAL_PLAYING) {
    if (bug_counter(player1_map_2) == 0) state = M1_WON_GAME;
  }
  else if (state == IN_M_PLAYING_3){
    if (bug_counter(player1_map_3) == 0) state = M1_WON_GAME;
  }

  //LOOSING CONDITION
  if (state == IN_M_PLAYING_1){
    if (player1_map_1[1][3] != '0' || player1_map_1[1][4] != '0') state = M2_WON_GAME;
  }
  else if (state == IN_M_PLAYING_2 || state == IN_SERIAL_PLAYING){
    if (player1_map_2[1][3] != '0' || player1_map_2[1][4] != '0') state = M2_WON_GAME;
  }
  else if (state == IN_M_PLAYING_3){
    if (player1_map_3[1][3] != '0' || player1_map_3[1][4] != '0') state = M2_WON_GAME;
  }  
}

void check_ending_conditions_2(){
  
  //WINNING CONDITION
  if (state == IN_M_PLAYING_1){
    if (bug_counter(player2_map_1) == 0) state = M2_WON_GAME;
  }
  else if (state == IN_M_PLAYING_2 || state == IN_SERIAL_PLAYING) {
    if (bug_counter(player2_map_2) == 0) state = M2_WON_GAME;
  }
  else if (state == IN_M_PLAYING_3){
    if (bug_counter(player2_map_3) == 0) state = M2_WON_GAME;
  }

  //LOOSING CONDITION
  if (state == IN_M_PLAYING_1){
    if (player2_map_1[1][3] != '0' || player2_map_1[1][4] != '0') state = M1_WON_GAME;
  }
  else if (state == IN_M_PLAYING_2 || state == IN_SERIAL_PLAYING){
    if (player2_map_2[1][3] != '0' || player2_map_2[1][4] != '0') state = M1_WON_GAME;
  }
  else if (state == IN_M_PLAYING_3){
    if (player2_map_3[1][3] != '0' || player2_map_3[1][4] != '0') state = M1_WON_GAME;
  }  
}

void draw_numerical_info_multi(){
  //DISPLAY GAME TIME
  display_number((counter-initial_time-3*60) / 60, 653, 370, 27);

  //DISPLAY LEVEL NUMBER
  if (state == IN_M_PLAYING_1) display_number(1, 560, 151, 27);
  if (state == IN_M_PLAYING_2) display_number(2, 560, 151, 27);
  if (state == IN_M_PLAYING_3) display_number(3, 560, 151, 27);

  //DISPLAY BUGS LEFT
  if (state == IN_M_PLAYING_1) {
    display_number(bug_counter(player1_map_1), 540, 500, 27);
    display_number(bug_counter(player2_map_1), 653, 500, 27);
  }
  if (state == IN_M_PLAYING_2) {
    display_number(bug_counter(player1_map_2), 540, 500, 27);
    display_number(bug_counter(player2_map_2), 653, 500, 27);
  }
  if (state == IN_M_PLAYING_3) {
    display_number(bug_counter(player1_map_3), 540, 500, 27);
    display_number(bug_counter(player2_map_3), 653, 500, 27);
  }
}

void reset_multi_player(){
  memcpy(player1_map_1, safe_map_1, 14*8*sizeof(char));
  memcpy(player2_map_1, safe_map_1, 14*8*sizeof(char));

  memcpy(player1_map_2, safe_map_2, 14*8*sizeof(char));
  memcpy(player2_map_2, safe_map_2, 14*8*sizeof(char));

  memcpy(player1_map_3, safe_map_3, 14*8*sizeof(char));
  memcpy(player2_map_3, safe_map_3, 14*8*sizeof(char));

  first_time_initial = true;

  rotation1 = 0;
  rotation2 = 0;

  first_throw1 = true;
  first_throw2 = true;

  horizontal1 = true;
  horizontal2 = true;

  allow_key_movement1 = false;
  allow_key_movement2 = false;

  x1 = 145 + 3*35;
  y1 = 290;

  x2 = 720 + 3*35;
  y2 = 290;
}
