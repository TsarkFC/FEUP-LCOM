// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

//Include header file
#include "headers/display.h"
#include "headers/single_player.h"
#include "headers/multi_player.h"
#include "headers/settings.h"
#include "headers/rtc.h"
#include "headers/serial_port.h"

extern uint8_t *double_buffer; //DOUBLE BUFFERING DECALRED IN MAIN LOOP

extern enum display menu;
extern enum display_single single_menu;
extern enum display_multi multi_menu;
extern enum state_of_program state;
extern enum select_pause pause_sel;
extern enum settings set;
enum select_device device_sel = ONE;

extern void* video_mem;
extern unsigned bits_per_pixel;
extern unsigned h_res;
extern unsigned v_res;
extern int counter;

extern char single_map_1[14][8];
extern char single_map_2[14][8];
extern char single_map_3[14][8];

extern char player1_map_1[14][8];
extern char player2_map_1[14][8];
extern char player1_map_2[14][8];
extern char player2_map_2[14][8];
extern char player1_map_3[14][8];
extern char player2_map_3[14][8];

extern uint32_t last_received1;
extern uint32_t last_received2;

static bool get_out = false;
bool get_out_f(){return get_out;}

static int save_counter;

static int device = 1;
int get_device(){return device;}
void set_device(int d){device = d;}

//ARRAY CONTAINING ALL INFORMATION FROM IMAGES XPM'S
xpm_map_t const all_images[] = {main_menu_xpm,              //0
                              main_menu_14c_first_sel_xpm,  //1
                              main_menu_14c_second_sel_xpm, //2
                              main_menu_14c_third_sel_xpm,  //3

                              cursor_xpm,                   //4

                              singleplayer_menu_xpm,        //5
                              singleplayer_menu_level1_xpm, //6
                              singleplayer_menu_level2_xpm, //7
                              singleplayer_menu_level3_xpm, //8
                              singleplayer_menu_go_back_xpm,//9

                              multiplayer_menu_xpm,         //10
                              multiplayer_menu_level1_xpm,  //11
                              multiplayer_menu_level2_xpm,  //12
                              multiplayer_menu_level3_xpm,  //13
                              multiplayer_menu_go_back_xpm, //14

                              singleplayer_game_3_xpm, //15
                              singleplayer_game_2_xpm, //16
                              singleplayer_game_1_xpm, //17
                              singleplayer_game_GO_xpm,//18
                              singleplayer_game_xpm,   //19

                              blue_bug_1_xpm,   //20
                              blue_bug_2_xpm,   //21
                              red_bug_1_xpm,    //22
                              red_bug_2_xpm,    //23
                              yellow_bug_1_xpm, //24
                              yellow_bug_2_xpm, //25

                              blue_pill_xpm,     //26
                              red_pill_xpm,      //27
                              yellow_pill_xpm,   //28

                              blue_pill_big_xpm,  //29
                              red_pill_big_xpm,   //30
                              yellow_pill_big_xpm,//31

                              zero_xpm,  //32
                              one_xpm,   //33
                              two_xpm,   //34
                              three_xpm, //35
                              four_xpm,  //36
                              five_xpm,  //37
                              six_xpm,   //38
                              seven_xpm, //39
                              eight_xpm, //40
                              nine_xpm,  //41

                              multiplayer_game_xpm,    //42
                              multiplayer_game_1_xpm,  //43
                              multiplayer_game_2_xpm,  //44
                              multiplayer_game_3_xpm,  //45
                              multiplayer_game_go_xpm, //46

                              pause_xpm,         //47
                              pause_continue_xpm,//48
                              pause_exit_xpm,    //49

                              gameover_xpm,    //50
                              success_xpm,     //51
                              player1_won_xpm, //52
                              player2_won_xpm, //53

                              hour_change_xpm,         //54
                              hour_change_time_xpm,    //55
                              hour_change_go_back_xpm, //56
                              settings_xpm,            //57

                              select_1_xpm,           //58
                              select_2_xpm,           //59
                              waiting_connection_xpm, //60

                              instructions_multi_xpm, //61
                              instructions_single_xpm //62
                              };

//ARRAY CONTAINING ALL INFORMATION FROM LOAD OPERATIONS                          
xpm_image_t im[sizeof(all_images) / sizeof(all_images[0])];

int load_images(){
  enum xpm_image_type type = XPM_8_8_8_8;

  for (unsigned i = 0; i < sizeof(all_images) / sizeof(all_images[0]); i++){
    xpm_image_t img;
    if (xpm_load(all_images[i], type, &img) == NULL) return 1;
    im[i] = img;
  }

  return 0;
}

int display_menu(uint16_t cursor_x, uint16_t cursor_y){
  xpm_image_t img;
  int ind;

  switch (menu){
    
    case MAIN_MENU:
      if (vg_draw_pixmap(im[0], main_menu_xpm, 0, 0)) return 1;
      img = im[0];
      ind = 0;
      break;
    
    case SINGLE_PLAYER:
      if (vg_draw_pixmap(im[1], main_menu_14c_first_sel_xpm, 0, 0)) return 1;
      img = im[1];
      ind = 1;
      break;
    
    case MULTI_PLAYER:
      if (vg_draw_pixmap(im[2], main_menu_14c_second_sel_xpm, 0, 0)) return 1;
      img = im[2];
      ind = 2;
      break;

    case SERIAL:
      if (vg_draw_pixmap(im[3], main_menu_14c_third_sel_xpm, 0, 0)) return 1;
      img = im[3];
      ind = 3;
      break;
  }

  if (mouse_cursor(img, ind, cursor_x, cursor_y)) return 1;
  memcpy(video_mem, double_buffer, 1152 * 864 * ((bits_per_pixel + 7)/8));
  return 0;
}

int display_single_menu(uint16_t cursor_x, uint16_t cursor_y){
  xpm_image_t img;
  int ind;

  switch(single_menu){
    case SINGLE_MENU:
      if (vg_draw_pixmap(im[5], singleplayer_menu_xpm, 0, 0)) return 1;
      img = im[5];
      ind = 5;
      break;

    case LEVEL1:
      if (vg_draw_pixmap(im[6], singleplayer_menu_level1_xpm, 0, 0)) return 1;
      img = im[6];
      ind = 6;
      break;

    case LEVEL2:
      if (vg_draw_pixmap(im[7], singleplayer_menu_level2_xpm, 0, 0)) return 1;
      img = im[7];
      ind = 7;
      break;

    case LEVEL3:
      if (vg_draw_pixmap(im[8], singleplayer_menu_level3_xpm, 0, 0)) return 1;
      img = im[8];
      ind = 8;

      break;
    case GO_BACK:
      if (vg_draw_pixmap(im[9], singleplayer_menu_go_back_xpm, 0, 0)) return 1;
      img = im[9];
      ind = 9;
      break;

    
  }

  if (mouse_cursor(img, ind, cursor_x, cursor_y)) return 1;
  memcpy(video_mem, double_buffer, 1152 * 864 * ((bits_per_pixel + 7)/8));

  return 0;
}

int display_multi_menu(uint16_t cursor_x, uint16_t cursor_y){
  xpm_image_t img;
  int ind;

  switch(multi_menu){
    case MULTI_MENU:
      if (vg_draw_pixmap(im[10], multiplayer_menu_xpm, 0, 0)) return 1;
      img = im[10];
      ind = 10;
      break;

    case LEVEL1_M:
      if (vg_draw_pixmap(im[11], multiplayer_menu_level1_xpm, 0, 0)) return 1;
      img = im[11];
      ind = 11;
      break;

    case LEVEL2_M:
      if (vg_draw_pixmap(im[12], multiplayer_menu_level2_xpm, 0, 0)) return 1;
      img = im[12];
      ind = 12;
      break;

    case LEVEL3_M:
      if (vg_draw_pixmap(im[13], multiplayer_menu_level3_xpm, 0, 0)) return 1;
      img = im[13];
      ind = 13;

      break;
    case GO_BACK_M:
      if (vg_draw_pixmap(im[14], multiplayer_menu_go_back_xpm, 0, 0)) return 1;
      img = im[14];
      ind = 14;
      break;
  }

  if (mouse_cursor(img, ind, cursor_x, cursor_y)) return 1;
  memcpy(video_mem, double_buffer, 1152 * 864 * ((bits_per_pixel + 7)/8));

  return 0;
}


int mouse_handler(struct packet pp, int *cursor_x, int *cursor_y){
  int previous_x = *cursor_x;
  int previous_y = *cursor_y; 

  //CURSOR IMPLEMENTATION
  *cursor_x += pp.delta_x;
  *cursor_y -= pp.delta_y;

  //ALLOW CURSOR PARTIAL VISUALIZATION WHEN IT HAS SAME COORDINATES AS EDGES
  if (*cursor_x >= (int)h_res) *cursor_x = h_res - 5;
  if (*cursor_y >= (int)v_res) *cursor_y = v_res - 5; 

  if (*cursor_x < 0) *cursor_x = 0;
  if (*cursor_y < 0) *cursor_y = 0;

  if (state == IN_MAIN_MENU){
    if (*cursor_x > 1013 && *cursor_x < 1152 && *cursor_y > 729 && *cursor_y < 864 && pp.lb)
      state = SETTINGS;
      
    if (*cursor_x > 266 && *cursor_x < 816 && *cursor_y > 508 && *cursor_y < 567){
      if (pp.lb) state = IN_LEVEL_SEL_S;
      else menu = SINGLE_PLAYER;
    }
    else if (*cursor_x > 298 && *cursor_x < 800 && *cursor_y > 604 && *cursor_y < 666){
      if (pp.lb) state = IN_LEVEL_SEL_M;
      else menu = MULTI_PLAYER;
    }
    else if (*cursor_x > 304 && *cursor_x < 767 && *cursor_y > 707 && *cursor_y < 758){
      if (pp.lb) state = SEL_DEVICE;
      else menu = SERIAL;
    }
    else menu = MAIN_MENU;

    if (*cursor_x > 13 && *cursor_x < 132 && *cursor_y > 729 && *cursor_y < 848 && pp.lb) 
      get_out = true;
  }

  else if (state == IN_LEVEL_SEL_S){
    if (*cursor_x > 329 && *cursor_x < 804 && *cursor_y > 347 && *cursor_y < 424){
      if (pp.lb) state = IN_S_PLAYING_1;
      else single_menu = LEVEL1;
    }
    else if (*cursor_x > 329 && *cursor_x < 804 && *cursor_y > 462 && *cursor_y < 537){
      if (pp.lb) state = IN_S_PLAYING_2;
      else single_menu = LEVEL2;
    }
    else if (*cursor_x > 329 && *cursor_x < 804 && *cursor_y > 577 && *cursor_y < 665){
      if (pp.lb) state = IN_S_PLAYING_3;
      else single_menu = LEVEL3;
    }
    else if (*cursor_x > 329 && *cursor_x < 804 && *cursor_y > 695 && *cursor_y < 773){
      if (pp.lb) state = IN_MAIN_MENU;
      else single_menu = GO_BACK;
    }
    else single_menu = SINGLE_MENU;
  }

  else if (state == IN_LEVEL_SEL_M){
    if (*cursor_x > 329 && *cursor_x < 804 && *cursor_y > 347 && *cursor_y < 424){
      if (pp.lb) state = IN_M_PLAYING_1;
      else multi_menu = LEVEL1_M;
    }
    else if (*cursor_x > 329 && *cursor_x < 804 && *cursor_y > 462 && *cursor_y < 537){
      if (pp.lb) state = IN_M_PLAYING_2;
      else multi_menu = LEVEL2_M;
    }
    else if (*cursor_x > 329 && *cursor_x < 804 && *cursor_y > 577 && *cursor_y < 665){
      if (pp.lb) state = IN_M_PLAYING_3;
      else multi_menu = LEVEL3_M;
    }
    else if (*cursor_x > 329 && *cursor_x < 804 && *cursor_y > 695 && *cursor_y < 773){
      if (pp.lb) state = IN_MAIN_MENU;
      else multi_menu = GO_BACK_M;
    }
    else multi_menu = MULTI_MENU;
  }

  else if (state == IN_S_PLAYING_1){
    mouse_accelerate(pp.lb, get_x(), get_y(), previous_x, previous_y, *cursor_x, *cursor_y, 434, 289, get_flag(), *get_horizontal(), 35, single_map_1);

    mouse_send_to_stack(single_map_1, 434, 289, get_x(), get_y(), 35, previous_x, previous_y, *cursor_x, *cursor_y, pp.rb, get_flag(), get_horizontal());
  }
  else if (state == IN_S_PLAYING_2){
    mouse_accelerate(pp.lb, get_x(), get_y(), previous_x, previous_y, *cursor_x, *cursor_y, 434, 289, get_flag(), *get_horizontal(), 35, single_map_2);

    mouse_send_to_stack(single_map_2, 434, 289, get_x(), get_y(), 35, previous_x, previous_y, *cursor_x, *cursor_y, pp.rb, get_flag(), get_horizontal());
  }
  else if (state == IN_S_PLAYING_3){
    mouse_accelerate(pp.lb, get_x(), get_y(), previous_x, previous_y, *cursor_x, *cursor_y, 434, 289, get_flag(), *get_horizontal(), 35, single_map_3);

    mouse_send_to_stack(single_map_3, 434, 289, get_x(), get_y(), 35, previous_x, previous_y, *cursor_x, *cursor_y, pp.rb, get_flag(), get_horizontal());
  }

  else if (state == SETTINGS){
    if (*cursor_x > 263 && *cursor_x < 851 && *cursor_y > 455 && *cursor_y < 520){
      if (pp.lb) state = NEW_HOUR;
      else set = TIME_CHANGE;
    }
    else if (*cursor_x > 371 && *cursor_x < 755 && *cursor_y > 581 && *cursor_y < 635){
      if (pp.lb) state = IN_MAIN_MENU;
      else set = GO_BACK_S;
    }
    else set = NOTHING_S;
  }

  return 0;
}

int mouse_cursor(xpm_image_t img, int ind, uint16_t cursor_x, uint16_t cursor_y){
  static int initial = 2;
  static int initial1 = 2;
  static int initial2 = 2;

  //DRAW BACKGROUND COLOR DURING GAME TIME
  draw_background_color();

  //DRAW BACKGOUND
  if (vg_draw_pixmap(img, all_images[ind], 0, 0)) return 1;

  if (cursor_x > 976 && cursor_x < 1113 && cursor_y > 706 && cursor_y < 826 && (state == IN_LEVEL_SEL_S)){
      vg_draw_pixmap(im[62], instructions_single_xpm, 0, 0);
    }
  else if (cursor_x > 976 && cursor_x < 1113 && cursor_y > 706 && cursor_y < 826 && (state == IN_LEVEL_SEL_M)){
      vg_draw_pixmap(im[61], instructions_multi_xpm, 0, 0);
    }
  
  //DRAW CLOCK
  if (state == SETTINGS) draw_clock();

  //CHECKS IF SINGLE PLAYER IS INSIDE GAME (DRAW GAME MAP)
  if ((state == IN_S_PLAYING_1) && !ind){
    draw_game_board(single_map_1, 434, 289, 35, get_bug_mov_single());
    throw_pill(&initial, single_map_1, 434, 289, 35);
    draw_stack(get_stack_1(), get_stack_2());
  }
  else if ((state == IN_S_PLAYING_2) && !ind){
    draw_game_board(single_map_2, 434, 289, 35, get_bug_mov_single());
    throw_pill(&initial, single_map_2, 434, 289, 35);
    draw_stack(get_stack_1(), get_stack_2());
  }
  else if ((state == IN_S_PLAYING_3) && !ind){
    draw_game_board(single_map_3, 434, 289, 35, get_bug_mov_single());
    throw_pill(&initial, single_map_3, 434, 289, 35);
    draw_stack(get_stack_1(), get_stack_2());
  }

  if ((state == IN_M_PLAYING_1) && !ind){
    draw_game_board(player1_map_1, 145, 290, 35, get_bug_mov_multi_1());
    draw_game_board(player2_map_1, 720, 290, 35, get_bug_mov_multi_2());
    draw_numerical_info_multi();
    throw_pill_1(&initial1, player1_map_1, 145, 290, 35);
    throw_pill_2(&initial2, player2_map_1, 720, 290, 35);
  }
  else if ((state == IN_M_PLAYING_2) && !ind){
    draw_game_board(player1_map_2, 145, 290, 35, get_bug_mov_multi_1());
    draw_game_board(player2_map_2, 720, 290, 35, get_bug_mov_multi_2());
    draw_numerical_info_multi();
    throw_pill_1(&initial1, player1_map_2, 145, 290, 35);
    throw_pill_2(&initial2, player2_map_2, 720, 290, 35);
  }
  else if ((state == IN_M_PLAYING_3) && !ind){
    draw_game_board(player1_map_3, 145, 290, 35, get_bug_mov_multi_1());
    draw_game_board(player2_map_3, 720, 290, 35, get_bug_mov_multi_2());
    draw_numerical_info_multi();
    throw_pill_1(&initial1, player1_map_3, 145, 290, 35);
    throw_pill_2(&initial2, player2_map_3, 720, 290, 35);
  }

  //DRAW CURSOR
  if (vg_draw_pixmap(im[4], cursor_xpm, cursor_x, cursor_y)) return 1;

  return 0;
}

int key_handler(uint8_t array[]){

  switch(state){

    case IN_MAIN_MENU:
      key_main_menu(array);
      break;

    case IN_LEVEL_SEL_S:
      key_level_menu_s(array);
      break;

    case IN_LEVEL_SEL_M:
      key_level_menu_m(array);
      break;

    case IN_S_PLAYING_1:
      if (array[0] == ESQ_KEY) {
        state = PAUSE_S1;
        save_counter = counter;
      }

      key_playing(array, get_flag(), single_map_1, 434, 289, 35, 1, 203, 205, get_x(), get_y(), *get_horizontal());

      pill_rotation(array, get_flag(), single_map_1, 434, 289, 35, get_x(), get_y(), get_rotation(), get_horizontal(), 200);
      break;
    
    case IN_S_PLAYING_2:
      if (array[0] == ESQ_KEY){
        state = PAUSE_S2;
        save_counter = counter;
      }

      key_playing(array, get_flag(), single_map_2, 434, 289, 35, 1, 203, 205, get_x(), get_y(), *get_horizontal());

      pill_rotation(array, get_flag(), single_map_2, 434, 289, 35, get_x(), get_y(), get_rotation(), get_horizontal(), 200);
      break;

    case IN_S_PLAYING_3:
      if (array[0] == ESQ_KEY){
        state = PAUSE_S3;
        save_counter = counter;
      }

      key_playing(array, get_flag(), single_map_3, 434, 289, 35, 1, 203, 205, get_x(), get_y(), *get_horizontal());
      
      pill_rotation(array, get_flag(), single_map_3, 434, 289, 35, get_x(), get_y(), get_rotation(), get_horizontal(), 200);
      break;
    
    case IN_M_PLAYING_1:
      if (array[0] == ESQ_KEY){
        state = PAUSE_M1;
        save_counter = counter;
      }

      //player1
      key_playing(array, *get_flag1(), player1_map_1, 145, 290, 35, 0, 158, 160, getx1(), gety1(), *get_horizontal1());
      
      pill_rotation(array, *get_flag1(), player1_map_1, 145, 290, 35, getx1(), gety1(), get_rotation1(), get_horizontal1(), 145);
      
      accelerate_mov(player1_map_1, 159, getx1(), gety1(), 145, 290, *get_flag1(), array, *get_horizontal1(), 35);

      //player2
      key_playing(array, *get_flag2(), player2_map_1, 720, 290, 35, 1, 203, 205, getx2(), gety2(), *get_horizontal2());
      
      pill_rotation(array, *get_flag2(), player2_map_1, 720, 290, 35, getx2(), gety2(), get_rotation2(), get_horizontal2(), 200);

      accelerate_mov(player2_map_1, 208, getx2(), gety2(), 720, 290, *get_flag2(), array, *get_horizontal2(), 35);

      break;

    case IN_M_PLAYING_2:
      if (array[0] == ESQ_KEY){
        state = PAUSE_M2;
        save_counter = counter;
      }

      //player1
      key_playing(array, *get_flag1(), player1_map_2, 145, 290, 35, 0, 158, 160, getx1(), gety1(), *get_horizontal1());
      
      pill_rotation(array, *get_flag1(), player1_map_2, 145, 290, 35, getx1(), gety1(), get_rotation1(), get_horizontal1(), 145);

      accelerate_mov(player1_map_2, 159, getx1(), gety1(), 145, 290, *get_flag1(), array, *get_horizontal1(), 35);

      //player2
      key_playing(array, *get_flag2(), player2_map_2, 720, 290, 35, 1, 203, 205, getx2(), gety2(), *get_horizontal2());
      
      pill_rotation(array, *get_flag2(), player2_map_2, 720, 290, 35, getx2(), gety2(), get_rotation2(), get_horizontal2(), 200);

      accelerate_mov(player2_map_2, 208, getx2(), gety2(), 720, 290, *get_flag2(), array, *get_horizontal2(), 35);

      break;

    case IN_M_PLAYING_3:
      if (array[0] == ESQ_KEY){
        state = PAUSE_M3;
        save_counter = counter;
      }

      //player1
      key_playing(array, *get_flag1(), player1_map_3, 145, 290, 35, 0, 158, 160, getx1(), gety1(), *get_horizontal1());
      
      pill_rotation(array, *get_flag1(), player1_map_3, 145, 290, 35, getx1(), gety1(), get_rotation1(), get_horizontal1(), 145);

      accelerate_mov(player1_map_3, 159, getx1(), gety1(), 145, 290, *get_flag1(), array, *get_horizontal1(), 35);

      //player2
      key_playing(array, *get_flag2(), player2_map_3, 720, 290, 35, 1, 203, 205, getx2(), gety2(), *get_horizontal2());
      
      pill_rotation(array, *get_flag2(), player2_map_3, 720, 290, 35, getx2(), gety2(), get_rotation2(), get_horizontal2(), 200);

      accelerate_mov(player2_map_3, 208, getx2(), gety2(), 720, 290, *get_flag2(), array, *get_horizontal2(), 35);

      break;

    case WAIT_CONNECTION:
      if (array[0] == ENTER_KEY_BREAK) state = IN_MAIN_MENU;
      break;

    case IN_SERIAL_PLAYING:
      key_handler_serial(array, get_device());
      break;

    case SEL_DEVICE:
      if (device_sel == ONE && array[0] == TWO_BYTE && array[1] == RIGHT_ARROW_BREAK) device_sel = TWO;
      else if (device_sel == TWO && array[0] == TWO_BYTE && array[1] == LEFT_ARROW_BREAK) device_sel = ONE;

      else if (device_sel == ONE && array[0] == ENTER_KEY_BREAK){
        device = 1;
        state = WAIT_CONNECTION;
      }
      else if (device_sel == TWO && array[0] == ENTER_KEY_BREAK){
        device = 2;
        state = WAIT_CONNECTION;
      }
      break;

    case S_WON_GAME:
      if (array[0] == ENTER_KEY_BREAK) state = IN_MAIN_MENU;
      reset_single_player();
      reset_multi_player();
      break;

    case S_LOST_GAME:
      if (array[0] == ENTER_KEY_BREAK) state = IN_MAIN_MENU;
      reset_single_player();
      reset_multi_player();
      break;
    
    case M1_WON_GAME:
      if (array[0] == ENTER_KEY_BREAK) state = IN_MAIN_MENU;
      reset_single_player();
      reset_multi_player();
      break;
    
    case M2_WON_GAME:
      if (array[0] == ENTER_KEY_BREAK) state = IN_MAIN_MENU;
      reset_single_player();
      reset_multi_player();
      break;

    case PAUSE_S1:
      key_pause('s', 1, array);
      break;
    case PAUSE_S2:
      key_pause('s', 2, array);
      break;
    case PAUSE_S3:
      key_pause('s', 3, array);
      break;
    case PAUSE_M1:
      key_pause('m', 1, array);
      break;
    case PAUSE_M2:
      key_pause('m', 2, array);
      break;
    case PAUSE_M3:
      key_pause('m', 3, array);
      break;
    case SETTINGS:
      key_settings(array);
      break;
    case NEW_HOUR:{
      handle_input(array);
      break;
    }
  }
  return 0;
}

int key_main_menu(uint8_t array[]){
  //ANALYSE BREAK CODE FOR UP ARROW KEY
  if (array[0] == TWO_BYTE && array[1] == UP_KEY_BREAK){
    
    switch (menu){
      case SINGLE_PLAYER:
        break;
      case MAIN_MENU:
        menu = SINGLE_PLAYER;
        break;
      case MULTI_PLAYER:
        menu = SINGLE_PLAYER;
        break;
      case SERIAL:
        menu = MULTI_PLAYER;
        break;
    }
  }

  //ANALYSE BREAK CODE FOR DOWN ARROW KEY
  if (array[0] == TWO_BYTE && array[1] == DOWN_KEY_BREAK){
    
    switch (menu){
      case MAIN_MENU:
        menu = SINGLE_PLAYER;
        break;
      case MULTI_PLAYER:
        menu = SERIAL;
        break;
      case SINGLE_PLAYER:
        menu = MULTI_PLAYER;
        break;
      case SERIAL:
        break;
    }
  }

  //ANALYSE ENTER PRESS
  if (array[0] == ENTER_KEY_BREAK){
    
    switch (menu){
      case MAIN_MENU:
        state = IN_MAIN_MENU;
        break;
      case MULTI_PLAYER:
        state = IN_LEVEL_SEL_M;
        break;
      case SINGLE_PLAYER:
        state = IN_LEVEL_SEL_S;
        break;
      case SERIAL:
        state = SEL_DEVICE;
        break;
    }
  }
  return 0;
}

int key_level_menu_s(uint8_t array[]){
  //ANALYSE BREAK CODE FOR UP ARROW KEY
  if (array[0] == TWO_BYTE && array[1] == UP_KEY_BREAK){
    
    switch (single_menu){
      case SINGLE_MENU:
        break;
      case LEVEL1:
        single_menu = LEVEL1;
        break;
      case LEVEL2:
        single_menu = LEVEL1;
        break;
      case LEVEL3:
        single_menu = LEVEL2;
        break;
      case GO_BACK:
        single_menu = LEVEL3;
        break;
    }
  }

  //ANALYSE BREAK CODE FOR DOWN ARROW KEY
  if (array[0] == TWO_BYTE && array[1] == DOWN_KEY_BREAK){
    
    switch (single_menu){
      case SINGLE_MENU:
        single_menu = LEVEL1;
        break;
      case LEVEL1:
        single_menu = LEVEL2;
        break;
      case LEVEL2:
        single_menu = LEVEL3;
        break;
      case LEVEL3:
        single_menu = GO_BACK;
        break;
      case GO_BACK:
        break;
    }
  }

  //ANALYSE ENTER PRESS
  if (array[0] == ENTER_KEY_BREAK){
    
    switch (single_menu){
      case SINGLE_MENU:
        state =  IN_LEVEL_SEL_S;
        break;
      case LEVEL1:
        state = IN_S_PLAYING_1;
        break;
      case LEVEL2:
        state = IN_S_PLAYING_2;
        break;
      case LEVEL3:
        state = IN_S_PLAYING_3;
        break;
      case GO_BACK:
        state = IN_MAIN_MENU;
        break;
    }
  }
  return 0;
}

int key_level_menu_m(uint8_t array[]){
  //ANALYSE BREAK CODE FOR UP ARROW KEY
  if (array[0] == TWO_BYTE && array[1] == UP_KEY_BREAK){
    
    switch (multi_menu){
      case MULTI_MENU:
        break;
      case LEVEL1_M:
        multi_menu = LEVEL1_M;
        break;
      case LEVEL2_M:
        multi_menu = LEVEL1_M;
        break;
      case LEVEL3_M:
        multi_menu = LEVEL2_M;
        break;
      case GO_BACK_M:
        multi_menu = LEVEL3_M;
        break;
    }
  }

  //ANALYSE BREAK CODE FOR DOWN ARROW KEY
  if (array[0] == TWO_BYTE && array[1] == DOWN_KEY_BREAK){
    
    switch (multi_menu){
      case MULTI_MENU:
        multi_menu = LEVEL1_M;
        break;
      case LEVEL1_M:
        multi_menu = LEVEL2_M;
        break;
      case LEVEL2_M:
        multi_menu = LEVEL3_M;
        break;
      case LEVEL3_M:
        multi_menu = GO_BACK_M;
        break;
      case GO_BACK_M:
        break;
    }
  }

  //ANALYSE ENTER PRESS
  if (array[0] == ENTER_KEY_BREAK){
    
    switch (multi_menu){
      case MULTI_MENU:
        state =  IN_LEVEL_SEL_M;
        break;
      case LEVEL1_M:
        state = IN_M_PLAYING_1;
        break;
      case LEVEL2_M:
        state = IN_M_PLAYING_2;
        break;
      case LEVEL3_M:
        state = IN_M_PLAYING_3;
        break;
      case GO_BACK_M:
        state = IN_MAIN_MENU;
        break;
    }
  }
  return 0;
}

void display_pause(){
  if (pause_sel == PLAY){
    vg_draw_pixmap(im[48], pause_continue_xpm, 250, 250);
  } 
  else if (pause_sel == EXIT){
    vg_draw_pixmap(im[49], pause_exit_xpm, 250, 250);
  } 
  else if (pause_sel == NOTHING){
    vg_draw_pixmap(im[47], pause_xpm, 250, 250);
  } 

  memcpy(video_mem, double_buffer, 1152 * 864 * ((bits_per_pixel + 7)/8));
}

void key_pause(char s_m, int num, uint8_t array[]){
  //ANALIZE ARROW KEY PRESS
  if (pause_sel == NOTHING && ((array[0] == TWO_BYTE && array[1] == DOWN_KEY_BREAK) || (array[0] == TWO_BYTE && array[1] == UP_KEY_BREAK)))
    pause_sel = PLAY;
  else if (pause_sel == PLAY && array[0] == TWO_BYTE && array[1] == DOWN_KEY_BREAK) pause_sel = EXIT;
  else if (pause_sel == EXIT && array[0] == TWO_BYTE && array[1] == UP_KEY_BREAK) pause_sel = PLAY;

  //ANALYSE ENTER PRESS
  if (pause_sel == PLAY && array[0] == ENTER_KEY_BREAK){
    counter = save_counter;
    if (s_m == 's' && num == 1){
      state = IN_S_PLAYING_1;
      pause_sel = NOTHING;
    }
    else if (s_m == 's' && num == 2){
      state = IN_S_PLAYING_2;
      pause_sel = NOTHING;
    }
    else if (s_m == 's' && num == 3){
      state = IN_S_PLAYING_3;
      pause_sel = NOTHING;
    }
    else if (s_m == 'm' && num == 1){
      state = IN_M_PLAYING_1;
      pause_sel = NOTHING;
    }
    else if (s_m == 'm' && num == 2){
      state = IN_M_PLAYING_2;
      pause_sel = NOTHING;
    }
    else if (s_m == 'm' && num == 3){
      state = IN_M_PLAYING_3;
      pause_sel = NOTHING;
    }
  }
  else if (pause_sel == EXIT && array[0] == ENTER_KEY_BREAK){
    pause_sel = NOTHING;
    state = IN_MAIN_MENU;

    reset_single_player();
    reset_multi_player();
  }
}

void key_handler_serial(uint8_t array[], int device){

  if (device == 1){
    //player1
    key_playing(array, *get_flag1(), player1_map_2, 145, 290, 35, 0, 158, 160, getx1(), gety1(), *get_horizontal1());
    
    pill_rotation(array, *get_flag1(), player1_map_2, 145, 290, 35, getx1(), gety1(), get_rotation1(), get_horizontal1(), 145);

    accelerate_mov(player1_map_2, 159, getx1(), gety1(), 145, 290, *get_flag1(), array, *get_horizontal1(), 35);

    //player2
    if (last_received2 == 0) return;

    key_playing(array, *get_flag2(), player2_map_2, 720, 290, 35, 1, 158, 160, getx2(), gety2(), *get_horizontal2());
    
    pill_rotation(array, *get_flag2(), player2_map_2, 720, 290, 35, getx2(), gety2(), get_rotation2(), get_horizontal2(), 145);

    accelerate_mov(player2_map_2, 159, getx2(), gety2(), 720, 290, *get_flag2(), array, *get_horizontal2(), 35);

    last_received2 = 0;
  }
  else if (device == 2){
    //player1
    if (last_received1 == 0) return;

    key_playing(array, *get_flag1(), player1_map_2, 145, 290, 35, 0, 158, 160, getx1(), gety1(), *get_horizontal1());
    
    pill_rotation(array, *get_flag1(), player1_map_2, 145, 290, 35, getx1(), gety1(), get_rotation1(), get_horizontal1(), 145);

    accelerate_mov(player1_map_2, 159, getx1(), gety1(), 145, 290, *get_flag1(), array, *get_horizontal1(), 35);

    last_received1 = 0;

    //player2
    key_playing(array, *get_flag2(), player2_map_2, 720, 290, 35, 1, 158, 160, getx2(), gety2(), *get_horizontal2());
    
    pill_rotation(array, *get_flag2(), player2_map_2, 720, 290, 35, getx2(), gety2(), get_rotation2(), get_horizontal2(), 145);

    accelerate_mov(player2_map_2, 159, getx2(), gety2(), 720, 290, *get_flag2(), array, *get_horizontal2(), 35);
  }
}
