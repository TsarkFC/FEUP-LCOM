// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "headers/display.h"
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

static bool first_time_initial = true; //boolean variable to check the first time function
                                       //-> display_initial_countdown is called

static int initial_time; //initial time for countdown

static int rotation = 0;
int* get_rotation(){return &rotation;}

static bool first_throw = true;

static bool horizontal = true;
bool* get_horizontal(){return &horizontal;}

static int throw_time;
int* get_throw_time(){return &throw_time;}

//flag to allow key movement
static bool allow_key_movement = false;
bool get_flag(){return allow_key_movement;}

//initial coordinates
static int x = 434 + 3*35;
static int y = 289; 

int* get_x(){return &x;}
int* get_y(){return &y;}

//PILL STACK
static char stack[2] = {'0', '0'};
char get_stack_1(){return stack[0];}
char get_stack_2(){return stack[1];}
static bool movement = false;
static bool stack_detector = false;

//RECORD
static int record = 0;
int get_possible_record(){return record;}


//GAME MAPS
// 0 -> place is empty
// 1 -> place has blue bug
// 2 -> place has red bug
// 3 -> place has yellow bug
// y -> place has yellow single pill
// b -> place has blue single pill
// r -> place has red single pill

//Game maps for level 1, 2, 3
char single_map_3[14][8] = {{'0','0','0','0','0','0','0','0'}, //Non visiible line if there
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

static char safe_single_map_3[14][8] = {{'0','0','0','0','0','0','0','0'}, //Non visiible line if there
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

char single_map_2[14][8] = {{'0','0','0','0','0','0','0','0'}, //Non visiible line if there
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

static char safe_single_map_2[14][8] = {{'0','0','0','0','0','0','0','0'}, //Non visiible line if there
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

char single_map_1[14][8] = {{'0','0','0','0','0','0','0','0'}, //Non visiible line if there
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
static char safe_single_map_1[14][8] = {{'0','0','0','0','0','0','0','0'}, //Non visiible line if there
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

int display_initial_countdown(uint16_t cursor_x, uint16_t cursor_y){

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
    vg_draw_pixmap(im[15], multiplayer_game_3_xpm,0,0);
    img = im[15];
    ind = 15;
  }

  //DISPLAY NUMBER 2 ON COUNTDOWN
  else if (initial_time + 120 > counter){
    vg_draw_pixmap(im[16], multiplayer_game_2_xpm,0,0);
    img = im[16];
    ind = 16;
  }

  //DISPLAY NUMBER 1 ON COUNTDOWN
  else if (initial_time + 180 > counter){
    vg_draw_pixmap(im[17], multiplayer_game_1_xpm,0,0);
    img = im[17];
    ind = 17;
  }

  //SHOWN "GO!" MESSAGE TO PLAYER
  else if (initial_time + 200 > counter){
    vg_draw_pixmap(im[18], multiplayer_game_go_xpm,0,0);
    img = im[18];
    ind = 18;
  }

  //STARTS GAME
  else if (state == IN_S_PLAYING_1){
    //Update to draw game
    vg_draw_pixmap(im[19], multiplayer_game_xpm, 0, 0);

    pill_movement(timed_collision_detector(single_map_1, 434, 289, 35, horizontal, &x, &y, &first_throw), horizontal, single_map_1, &x, &y, 434, 289, 35);
    
    img = im[19];
    ind = 0;
  }
  else if (state == IN_S_PLAYING_2){
    //Update to draw game
    vg_draw_pixmap(im[19], singleplayer_game_xpm, 0, 0);

    pill_movement(timed_collision_detector(single_map_2, 434, 289, 35, horizontal, &x, &y, &first_throw), horizontal, single_map_2, &x, &y, 434, 289, 35);
    
    img = im[19];
    ind = 0;
  }
  else if (state == IN_S_PLAYING_3){
    //Update to draw game
    vg_draw_pixmap(im[19], singleplayer_game_xpm, 0, 0);

    pill_movement(timed_collision_detector(single_map_3, 434, 289, 35, horizontal, &x, &y, &first_throw), horizontal, single_map_3, &x, &y, 434, 289, 35);

    img = im[19];
    ind = 0;
  }

  //Board is drawn inside mouse_cursor
  if (mouse_cursor(img, ind, cursor_x, cursor_y)) return 1;

  memcpy(video_mem, double_buffer, 1152 * 864 * ((bits_per_pixel + 7)/8));

  return 0;
}

static bool bug_movement = true; //Variable that allows small bug movement over time
bool* get_bug_mov_single() {return &bug_movement;}

int draw_game_board(char game_map[14][8], int xi, int yi, int size, bool *bug_movement){

  //Allow bug movement
  if (counter % 30 == 0) *bug_movement = !(*bug_movement);


  //Draw board game
  for (unsigned i = 1; i < 14; i++){
    for (unsigned j = 0; j < 8; j++){

      if (game_map[i][j] == '0') continue;

      switch(game_map[i][j]){
        case '1':
          if (*bug_movement) vg_draw_pixmap(im[20], blue_bug_1_xpm, xi + j*size, yi + (i-1)*size);
          else vg_draw_pixmap(im[21], blue_bug_2_xpm, xi + j*size, yi + (i-1)*size);
          break;
        case '2':
          if (*bug_movement) vg_draw_pixmap(im[22], red_bug_1_xpm, xi + j*size, yi + (i-1)*size);
          else vg_draw_pixmap(im[23], red_bug_2_xpm, xi + j*size, yi + (i-1)*size);
          break;
        case '3':
          if (*bug_movement) vg_draw_pixmap(im[24], yellow_bug_1_xpm, xi + j*size, yi + (i-1)*size);
          else vg_draw_pixmap(im[25], yellow_bug_2_xpm, xi + j*size, yi + (i-1)*size);
          break;

        case 'y':
          vg_draw_pixmap(im[28], yellow_pill_xpm, xi + j*size, yi + (i-1)*size);
          break;
        case 'b':
          vg_draw_pixmap(im[26], blue_pill_xpm, xi + j*size, yi + (i-1)*size);
          break;
        case 'r':
          vg_draw_pixmap(im[27], red_pill_xpm, xi + j*size, yi + (i-1)*size);
          break;
      }
    }
  }
  return 0;
}

int throw_pill(int *previous, char game_map[14][8], int xi, int yi, int size){
  static int pill_no;
  static int drawing;

  if (first_throw){
    //reset variables for new movement
    rotation = 0;
    horizontal = true;
    throw_time = counter;
    allow_key_movement = false;

    //check four in line and solo pills
    check_horizontal(game_map);
    check_vertical(game_map);
    timed_fall_lost_pills(game_map);
    
    first_throw = false;
    pill_no = rand() % 9; //generates new pill to be drawn in map

    drawing = *previous; //gets pill that will be drawn in game map
    *previous = pill_no; //generates new pill that will be displayed in top right corner

    //check ending conditions
    check_ending_conditions();
  } 

  //draw next pill
  show_next(*previous, 120, 560, 80);

  //display numerical information
  draw_numerical_info();

  //draw beginning pill for 1 sec
  if (throw_time + 60 > counter){
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

  else if (throw_time + 61 > counter){

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
    allow_key_movement = true;
  }

  return 0;
}

//Returns 1 if movement was not realised and 0 otherwise
int pill_movement(int collision, bool horizontal, char game_map[14][8], int *x, int *y, int xi, int yi, int size){
  
  //Reset initial movement coordenates
  if (collision) return 1;

  if (stack_detector) {
    stack_detector = false;
    return 1;
  }

  //speed movement -> depending on how much time player has been playing for

  if (initial_time + 60*60 > counter){ //1 minute of game time
    if (counter % 60 == 0){
      
      if (horizontal) update_array_horizontal(x, y, game_map, xi, yi, size);
      else update_array_vertical(x, y, game_map, xi, yi, size);

      //update coordinates
      *y += 35;
    }
  }

  else if (initial_time + 60*120 > counter){ //2 minutes of game time
    if (counter % 50 == 0){

      if (horizontal) update_array_horizontal(x, y, game_map, xi, yi, size);
      else update_array_vertical(x, y, game_map, xi, yi, size);

      //update coordinates
      *y += 35;
    }
  }

  else { //3 minutes of game time
    if (counter % 40 == 0){

      if (horizontal) update_array_horizontal(x, y, game_map, xi, yi, size);
      else update_array_vertical(x, y, game_map, xi, yi, size);

      //update coordinates
      *y += 35;
    }
  }

  return 0;
}

//Handle pill rotation - space key press
int pill_rotation(uint8_t array[], bool allow_key, char game_map[14][8], int xi, int yi, int size, int *x, int *y, int *rotation, bool *horizontal, int key_number){

  if (!allow_key) return 1;

  if (array[0] == TWO_BYTE){if (array[1] != key_number) return 1;}
  else if (array[0] != key_number) return 1;

  if (state == IN_SERIAL_PLAYING){
    if (get_device() == 1) send_info(key_number, 1);
    else if (get_device() == 2) send_info(key_number, 2);
  }

  //Rotate DOWN
  if ((*rotation) == 0){

    //Cannot rotate down if pill is already in lowest position
    if ((*y-yi)/size == 13) return 1;

    //Check if destiny position is empty
    if (game_map[(*y - yi)/size + 1][(*x - xi)/size] == '0'){
      game_map[(*y - yi)/size + 1][(*x - xi)/size] = game_map[(*y - yi)/size][(*x - xi)/size + 1];
      game_map[(*y - yi)/size][(*x - xi)/size + 1] = '0';

      *horizontal = false;
      (*rotation)++;
    }
  }

  //Rotate LEFT
  else if ((*rotation) == 1){
    //Cannot rotate left if pill is in left border
    if ((*x-xi)/size == 0) return 1;

    //Check if destiny position is empty
    if (game_map[(*y - yi)/size][(*x - xi)/size - 1] == '0'){
      game_map[(*y - yi)/size][(*x - xi)/size - 1] = game_map[(*y - yi)/size + 1][(*x - xi)/size];
      game_map[(*y - yi)/size + 1][(*x - xi)/size] = '0';

      *horizontal = true;
      *x -= size;
      (*rotation)++;
    }
  }

  //Rotate UP
  else if ((*rotation) == 2){
    //Cannot rotate left if pill is in top border
    if ((*y-yi)/size == 0) return 1;

    //Check if destiny position is empty
    if (game_map[(*y - yi)/size - 1][(*x - xi)/size + 1] == '0'){
      game_map[(*y - yi)/size - 1][(*x - xi)/size + 1] = game_map[(*y - yi)/size][(*x - xi)/size];
      game_map[(*y - yi)/size][(*x - xi)/size] = '0';

      *horizontal = false;
      *x += size;
      *y -= size;
      (*rotation)++;
    }
  }

  //Rotate RIGHT
  else if ((*rotation) == 3){
    //Cannot rotate left if pill is in top border
    if ((*x-xi)/size == 7) return 1;

    if (game_map[(*y - yi)/size + 1][(*x - xi)/size + 1] == '0'){
      game_map[(*y - yi)/size + 1][(*x - xi)/size + 1] = game_map[(*y - yi)/size][(*x - xi)/size];
      game_map[(*y - yi)/size][(*x - xi)/size] = '0';
      
      *horizontal = true;
      *y += size;
      *rotation = 0;
    }
  }

  return 0;
}

void update_array_vertical(int *x, int *y, char game_map[14][8], int xi, int yi, int size){
  //Update array during vertical movement
  game_map[(*y - yi)/size + 2][(*x - xi)/size] = game_map[(*y - yi)/size + 1][(*x - xi)/size];
  game_map[(*y - yi)/size + 1][(*x - xi)/size] = game_map[(*y - yi)/size][(*x - xi)/size];
  game_map[(*y - yi)/size][(*x - xi)/size] = '0';
}

void update_array_horizontal(int *x, int *y, char game_map[14][8], int xi, int yi, int size){
  //update game map array

  //Set new coordinates
  game_map[(*y - yi)/size + 1][(*x - xi)/size] = game_map[(*y - yi)/size][(*x - xi)/size];
  game_map[(*y - yi)/size + 1][(*x - xi)/size + 1] = game_map[(*y - yi)/size][(*x - xi)/size + 1];
  
  //Zero previous coordinates
  game_map[(*y - yi)/size][(*x - xi)/size] = '0';
  game_map[(*y - yi)/size][(*x - xi)/size + 1] = '0';

  //Zero intial pill
  game_map[1][3] = '0';
  game_map[1][4] = '0';
}


int key_playing(uint8_t array[], bool allow_key, char game_map[14][8], int xi, int yi, int size, int array_pos, int left_mov, int right_mov, int *x, int *y, bool horizontal){
  if (!allow_key) return 1;

  if (array[0] == TWO_BYTE) array_pos = 1;
  else array_pos = 0;

  //left horizontal movement
  if (array[array_pos] == left_mov){

    if (state == IN_SERIAL_PLAYING && get_device() == 1) send_info(left_mov, 1);
    else if (state == IN_SERIAL_PLAYING && get_device() == 2) send_info(left_mov, 2);

    if (*x > xi){
      if (game_map[(*y - yi)/size][(*x - xi)/size - 1] == '0'){
        if (horizontal){
          //Set new coordinates
          game_map[(*y - yi)/size][(*x - xi)/size - 1] = game_map[(*y - yi)/size][(*x - xi)/size];
          game_map[(*y - yi)/size][(*x - xi)/size] = game_map[(*y - yi)/size][(*x - xi)/size + 1];
          
          //Zero previous coordinates
          game_map[(*y - yi)/size][(*x - xi)/size + 1] = '0';

          //update coordinates
          *x -= 35;
        }
        else if (game_map[(*y - yi)/size + 1][(*x - xi)/size - 1] == '0'){
          //Set new coordinates
          game_map[(*y - yi)/size][(*x - xi)/size - 1] = game_map[(*y - yi)/size][(*x - xi)/size];
          game_map[(*y - yi)/size + 1][(*x - xi)/size - 1] = game_map[(*y - yi)/size + 1][(*x - xi)/size];
          
          //Zero previous coordinates
          game_map[(*y - yi)/size][(*x - xi)/size] = '0';
          game_map[(*y - yi)/size + 1][(*x - xi)/size] = '0';

          //update coordinates
          *x -= 35;
        }
      }
    }
  }

  //right horizontal movement
  if (array[array_pos] == right_mov){

    if (state == IN_SERIAL_PLAYING && get_device() == 1) send_info(right_mov, 1);
    else if (state == IN_SERIAL_PLAYING && get_device() == 2) send_info(right_mov, 2);

    if (*x < xi + 6*size && horizontal){
      if (game_map[(*y - yi)/size][(*x - xi)/size + 2] == '0'){
        //Set new coordinates
        game_map[(*y - yi)/size][(*x - xi)/size + 2] = game_map[(*y - yi)/size][(*x - xi)/size + 1];
        game_map[(*y - yi)/size][(*x - xi)/size + 1] = game_map[(*y - yi)/size][(*x - xi)/size];
        
        //Zero previous coordinates
        game_map[(*y - yi)/size][(*x - xi)/size] = '0';

        //update coordinates
        *x += 35;
      }
    }
    else if (*x < xi + 7*size && !horizontal){
      if (game_map[(*y - yi)/size][(*x - xi)/size + 1] == '0' && game_map[(*y - yi)/size + 1][(*x - xi)/size + 1] == '0'){
        //Set new coordinates
        game_map[(*y - yi)/size][(*x - xi)/size + 1] = game_map[(*y - yi)/size][(*x - xi)/size];
        game_map[(*y - yi)/size + 1][(*x - xi)/size + 1] = game_map[(*y - yi)/size + 1][(*x - xi)/size];
        
        //Zero previous coordinates
        game_map[(*y - yi)/size][(*x - xi)/size] = '0';
        game_map[(*y - yi)/size + 1][(*x - xi)/size] = '0';

        //update coordinates
        *x += 35;
      }
    }
  }
  return 0;
}

bool collision_detector(char game_map[14][8], int xi, int yi, int size, bool horizontal, int *x, int *y, bool *first_throw){
  if (horizontal){
    if ((game_map[(*y - yi)/size + 1][(*x - xi)/size] != '0' || 
        game_map[(*y - yi)/size + 1][(*x - xi)/size + 1] != '0') || (*y - yi)/size == 13) {
      
      *x = xi + 3*size;
      *y = yi; 
      *first_throw = true;
      return true;
    }
  }
  else{
    if (game_map[(*y - yi)/size + 2][(*x - xi)/size] != '0' || (*y - yi)/size + 1 == 13){
      *x = xi + 3*size;
      *y = yi;
      *first_throw = true;
      return true;
    }
  }

  return false;
}

bool timed_collision_detector(char game_map[14][8], int xi, int yi, int size, bool horizontal, int *x, int *y, bool *first_throw){
  if (initial_time + 60*60 > counter){ //1 minute of game time
    if (counter % 60 == 0) return collision_detector(game_map, xi, yi, size, horizontal, x, y, first_throw);
  }

  else if (initial_time + 60*120 > counter){ //2 minutes of game time
    if (counter % 50 == 0) return collision_detector(game_map, xi, yi, size, horizontal, x, y, first_throw);
  }

  else { //3 minutes of game time
    if (counter % 40 == 0) return collision_detector(game_map, xi, yi, size, horizontal, x, y, first_throw);
  }

  return false;
}

int get_pill_color(char c){
  if (c == '1') return 1;
  else if (c == 'b') return 1;
  else if (c == '2') return 2;
  else if (c == 'r') return 2;
  else if (c == '3') return 3;
  else if (c == 'y') return 3;
  return 0;
}

void check_horizontal(char game_map[14][8]){

  for (size_t line = 1; line < 14; line++){
    for (size_t column = 0; column < 5; column++){

      if (get_pill_color(game_map[line][column]) == get_pill_color(game_map[line][column+1]) &&
          get_pill_color(game_map[line][column]) == get_pill_color(game_map[line][column+2]) &&
          get_pill_color(game_map[line][column]) == get_pill_color(game_map[line][column+3])){
          game_map[line][column] = '0';
          game_map[line][column+1] = '0';
          game_map[line][column+2] = '0';
          game_map[line][column+3] = '0';
      }
    }
  }
}

void check_vertical(char game_map[14][8]){
  for (size_t line = 1; line < 11; line++){
    for (size_t column = 0; column < 8; column++){

      if (get_pill_color(game_map[line][column]) == get_pill_color(game_map[line+1][column]) &&
          get_pill_color(game_map[line][column]) == get_pill_color(game_map[line+2][column]) &&
          get_pill_color(game_map[line][column]) == get_pill_color(game_map[line+3][column])){
          game_map[line][column] = '0';
          game_map[line+1][column] = '0';
          game_map[line+2][column] = '0';
          game_map[line+3][column] = '0';
      }
    }
  }
}

void update_lost_pills(char game_map[14][8]){
  for (size_t line = 1; line < 13; line++){
    for (size_t column = 0; column < 8; column++){

      //LEFT MOST COLUMN
      if (column == 0){
        if (game_map[line][column+1] == '0' && game_map[line+1][column] == '0'){
          int num = line;
          while (game_map[num][column] != '0' &&
                game_map[num][column] != '1' && 
                game_map[num][column] != '2' && 
                game_map[num][column] != '3'){
            game_map[num+1][column] = game_map[num][column];
            num--;
          }
          num++;
          game_map[num][column] = '0';
        }
      }

      //RIGHT MOST COLUMN
      else if (column == 7){
        if (game_map[line][column-1] == '0' && game_map[line+1][column] == '0'){
          int num = line;
          while (game_map[num][column] != '0' &&
                game_map[num][column] != '1' && 
                game_map[num][column] != '2' && 
                game_map[num][column] != '3'){
            game_map[num+1][column] = game_map[num][column];
            num--;
          }
          num++;
          game_map[num][column] = '0';
        }
      }

      else{
        if (game_map[line][column+1] == '0' && 
            game_map[line+1][column] == '0' && 
            game_map[line][column-1] == '0'){
          int num = line;
          while (game_map[num][column] != '0' &&
                game_map[num][column] != '1' && 
                game_map[num][column] != '2' && 
                game_map[num][column] != '3'){
            game_map[num+1][column] = game_map[num][column];
            num--;
          }
          num++;
          game_map[num][column] = '0';
        }
      }
    }
  }
}

void timed_fall_lost_pills(char game_map[14][8]){

  if (initial_time + 60*60 > counter){ //1 minute of game time
    if (counter % 60 == 0)update_lost_pills(game_map);
  }

  else if (initial_time + 60*120 > counter){ //2 minutes of game time
    if (counter % 50 == 0) update_lost_pills(game_map);
  }

  else { //3 minutes of game time
    if (counter % 40 == 0) update_lost_pills(game_map);
  }
}

void show_next(int drawing, int x, int y, int pill_length){
  if (drawing == 0){
    vg_draw_pixmap(im[29], blue_pill_xpm, x, y);
    vg_draw_pixmap(im[29], blue_pill_xpm, x + pill_length, y);
  }
  else if (drawing == 1){
    vg_draw_pixmap(im[29], blue_pill_xpm, x, y);
    vg_draw_pixmap(im[30], red_pill_xpm, x + pill_length, y);
  }
  else if (drawing == 2){
    vg_draw_pixmap(im[29], blue_pill_xpm, x, y);
    vg_draw_pixmap(im[31], yellow_pill_xpm, x + pill_length, y);
  }
  else if (drawing == 3){
    vg_draw_pixmap(im[30], red_pill_xpm, x, y);
    vg_draw_pixmap(im[29], blue_pill_xpm, x + pill_length, y);
  }
  else if (drawing == 4){
    vg_draw_pixmap(im[30], red_pill_xpm, x, y);
    vg_draw_pixmap(im[30], red_pill_xpm, x + pill_length, y);
  }
  else if (drawing == 5){
    vg_draw_pixmap(im[30], red_pill_xpm, x, y);
    vg_draw_pixmap(im[31], yellow_pill_xpm, x + pill_length, y);
  }
  else if (drawing == 6){
    vg_draw_pixmap(im[31], yellow_pill_xpm, x, y);
    vg_draw_pixmap(im[29], blue_pill_xpm, x + pill_length, y);
  }
  else if (drawing == 7){
    vg_draw_pixmap(im[31], yellow_pill_xpm, x, y);
    vg_draw_pixmap(im[30], red_pill_xpm, x + pill_length, y);
  }
  else if (drawing == 8){
    vg_draw_pixmap(im[31], yellow_pill_xpm, x, y);
    vg_draw_pixmap(im[31], yellow_pill_xpm, x + pill_length, y);
  }
}

void display_number(int number, int x, int y, int num_size){
  int digit;
  int counter = 0;

  if (number == 0) vg_draw_pixmap(im[32], zero_xpm, x, y);

  while (number != 0){
    digit = number % 10;
    number = number / 10;

    if (digit == 0) vg_draw_pixmap(im[32], zero_xpm, x - num_size*counter, y);
    else if (digit == 1) vg_draw_pixmap(im[33], one_xpm, x - num_size*counter,y);
    else if (digit == 2) vg_draw_pixmap(im[34], two_xpm, x - num_size*counter, y);
    else if (digit == 3) vg_draw_pixmap(im[35], three_xpm, x - num_size*counter, y);
    else if (digit == 4) vg_draw_pixmap(im[36], four_xpm, x - num_size*counter, y);   
    else if (digit == 5) vg_draw_pixmap(im[37], five_xpm, x - num_size*counter, y); 
    else if (digit == 6) vg_draw_pixmap(im[38], six_xpm, x - num_size*counter, y);
    else if (digit == 7) vg_draw_pixmap(im[39], seven_xpm, x - num_size*counter, y);
    else if (digit == 8) vg_draw_pixmap(im[40], eight_xpm, x - num_size*counter, y);
    else if (digit == 9) vg_draw_pixmap(im[41], nine_xpm, x - num_size*counter, y);                    
    counter ++;                                                      
  }
}

int bug_counter(char game_map[14][8]){
  int bug_counter = 0;

  for (size_t line = 1; line < 14; line++){
    for (size_t column = 0; column < 8; column++){
      if (game_map[line][column] == '1' || 
          game_map[line][column] == '2' || 
          game_map[line][column] == '3') bug_counter++;
    }
  }
  return bug_counter;
}

void draw_numerical_info(){
  //DISLPAY ALL TIME BEST
  if (state == IN_S_PLAYING_1) display_number(get_record1(), 287, 232, 27);
  if (state == IN_S_PLAYING_2) display_number(get_record2(), 287, 232, 27);
  if (state == IN_S_PLAYING_3) display_number(get_record3(), 287, 232, 27);

  //DISPLAY GAME TIME
  display_number((counter-initial_time-3*60) / 60, 287, 330, 27);

  //DISPLAY LEVEL NUMBER
  if (state == IN_S_PLAYING_1) display_number(1, 1006, 561, 27);
  if (state == IN_S_PLAYING_2) display_number(2, 1006, 561, 27);
  if (state == IN_S_PLAYING_3) display_number(3, 1006, 561, 27);

  //DISPLAY BUGS LEFT
  if (state == IN_S_PLAYING_1) display_number(bug_counter(single_map_1), 1006, 707, 27);
  if (state == IN_S_PLAYING_2) display_number(bug_counter(single_map_2), 1006, 707, 27);
  if (state == IN_S_PLAYING_3) display_number(bug_counter(single_map_3), 1006, 707, 27);

}

void check_ending_conditions(){
  
  //WINNING CONDITION
  if (state == IN_S_PLAYING_1){
    if (bug_counter(single_map_1) == 0) {
      state = S_WON_GAME;
      record = (counter-initial_time-3*60) / 60;
      update_record(record, RECORD);
    }
  }
  else if (state == IN_S_PLAYING_2) {
    if (bug_counter(single_map_2) == 0) {
      state = S_WON_GAME;
      record = (counter-initial_time-3*60) / 60;
      update_record(record, RECORD2);
    }
  }
  else if (state == IN_S_PLAYING_3){
    if (bug_counter(single_map_3) == 0) {
      state = S_WON_GAME;
      record = (counter-initial_time-3*60) / 60;
      update_record(record, RECORD3);
    }
  }

  //LOOSING CONDITION
  if (state == IN_S_PLAYING_1){
    if (single_map_1[1][3] != '0' || single_map_1[1][4] != '0') state = S_LOST_GAME;
  }
  else if (state == IN_S_PLAYING_2){
    if (single_map_2[1][3] != '0' || single_map_2[1][4] != '0') state = S_LOST_GAME;
  }
  else if (state == IN_S_PLAYING_3){
    if (single_map_3[1][3] != '0' || single_map_3[1][4] != '0') state = S_LOST_GAME;
  }  
}

void mouse_accelerate(bool left_click, int *x, int *y, int previous_x, int previous_y, int new_x, int new_y, int xi, int yi, bool allow_key, bool horizontal, int size, char game_map[14][8]){
  //Only allow acceleration if left mouse button is pressed
  if (!left_click) return;

  //Only accepts movements with the y coordinate increasing
  if (previous_y >= new_y) return;

  if (!allow_key) return;

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

bool mouse_send_to_stack(char game_map[14][8], int xi, int yi, int *x, int *y, int size, int previous_x, int previous_y, int new_x, int new_y, bool right_click, bool allow_key, bool *horizontal){

  if (!allow_key) return false;

  //Only accepts movements with the y coordinate increasing
  if (right_click){
    if (previous_x >= new_x) movement = false;
    else  movement = true;
  }

  //GESTURE MOVEMENT HAS BEEN RECOGNIZED
  if (movement && !right_click){
    movement = false;

    //EMPTY STACK
    if (stack[0] == '0' && stack[1] == '0'){
      //HORIZONTAL MOVEMENT
      if (*horizontal){
        stack[0] = game_map[(*y - yi)/size][(*x - xi)/size];
        stack[1] = game_map[(*y - yi)/size][(*x - xi)/size + 1];

        game_map[(*y - yi)/size][(*x - xi)/size] = '0';
        game_map[(*y - yi)/size][(*x - xi)/size + 1] = '0';

        *x = xi + 3*size;
        *y = yi;
        first_throw = true;

        stack_detector = true;
      }
      //VERTICAL MOVEMENT
      else{
        stack[0] = game_map[(*y - yi)/size][(*x - xi)/size];
        stack[1] = game_map[(*y - yi)/size + 1][(*x - xi)/size];

        game_map[(*y - yi)/size][(*x - xi)/size] = '0';
        game_map[(*y - yi)/size + 1][(*x - xi)/size] = '0';

        *x = xi + 3*size;
        *y = yi;
        first_throw = true;
        *horizontal = true;

        stack_detector = true;
      }
    }

    //STACK HAS ALREADY BEEN UPDATED
    else{
      //HORIZONTAL MOVEMENT
      if (*horizontal){
        char aux_1 = stack[0];
        char aux_2 = stack[1];

        stack[0] = game_map[(*y - yi)/size][(*x - xi)/size];
        stack[1] = game_map[(*y - yi)/size][(*x - xi)/size + 1];

        game_map[(*y - yi)/size][(*x - xi)/size] = aux_1;
        game_map[(*y - yi)/size][(*x - xi)/size + 1] = aux_2;

      }
      //VERTICAL MOVEMENT
      else{
        char aux_1 = stack[0];
        char aux_2 = stack[1];

        stack[0] = game_map[(*y - yi)/size][(*x - xi)/size];
        stack[1] = game_map[(*y - yi)/size + 1][(*x - xi)/size];

        game_map[(*y - yi)/size][(*x - xi)/size] = aux_1;
        game_map[(*y - yi)/size + 1][(*x - xi)/size] = aux_2;
      
      }
    }
  }
  return true;
}

void draw_stack(char c1, char c2){
  if (c1 == '0' && c2 == '0') return;

  else if (c1 == 'b' && c2 == 'b'){
    vg_draw_pixmap(im[29], blue_pill_xpm, 846, 289);
    vg_draw_pixmap(im[29], blue_pill_xpm, 846 + 80, 289);
  }
  else if (c1 == 'b' && c2 == 'r'){
    vg_draw_pixmap(im[29], blue_pill_xpm, 846, 289);
    vg_draw_pixmap(im[30], red_pill_xpm, 846 + 80, 289);
  }
  else if (c1 == 'b' && c2 == 'y'){
    vg_draw_pixmap(im[29], blue_pill_xpm, 846, 289);
    vg_draw_pixmap(im[31], yellow_pill_xpm, 846 + 80, 289);
  }
  else if (c1 == 'r' && c2 == 'b'){
    vg_draw_pixmap(im[30], red_pill_xpm, 846, 289);
    vg_draw_pixmap(im[29], blue_pill_xpm, 846 + 80, 289);
  }
  else if (c1 == 'r' && c2 == 'r'){
    vg_draw_pixmap(im[30], red_pill_xpm, 846, 289);
    vg_draw_pixmap(im[30], red_pill_xpm, 846 + 80, 289);
  }
  else if (c1 == 'r' && c2 == 'y'){
    vg_draw_pixmap(im[30], red_pill_xpm, 846, 289);
    vg_draw_pixmap(im[31], yellow_pill_xpm, 846 + 80, 289);
  }
  else if (c1 == 'y' && c2 == 'b'){
    vg_draw_pixmap(im[31], yellow_pill_xpm, 846, 289);
    vg_draw_pixmap(im[29], blue_pill_xpm, 846 + 80, 289);
  }
  else if (c1 == 'y' && c2 == 'r'){
    vg_draw_pixmap(im[31], yellow_pill_xpm, 846, 289);
    vg_draw_pixmap(im[30], red_pill_xpm, 846 + 80, 289);
  }
  else if (c1 == 'y' && c2 == 'y'){
    vg_draw_pixmap(im[31], yellow_pill_xpm, 846, 289);
    vg_draw_pixmap(im[31], yellow_pill_xpm, 846 + 80, 289);
  }
}

void reset_single_player(){
  first_time_initial = true;
  rotation = 0;
  first_throw = true;
  horizontal = true;
  allow_key_movement = false;
  bug_movement = true;

  x = 434 + 3*35;
  y = 289;

  stack[0] = '0';
  stack[1] = '0';

  movement = false;
  stack_detector = false;

  memcpy(single_map_1, safe_single_map_1, 14*8*sizeof(char));
  memcpy(single_map_2, safe_single_map_2, 14*8*sizeof(char));
  memcpy(single_map_3, safe_single_map_3, 14*8*sizeof(char));
}
