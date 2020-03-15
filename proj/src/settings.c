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
#include "headers/single_player.h"
#include "headers/graphics.h"
#include "headers/rtc.h"
#include "headers/macro.h"


extern void* video_mem;
extern unsigned bits_per_pixel;
extern uint8_t *double_buffer;
extern xpm_image_t im[];
extern enum state_of_program state;
extern uint32_t relogio[3];

enum settings set = NOTHING_S;

void run_settings(uint16_t cursor_x, uint16_t cursor_y){
  xpm_image_t img;
  int ind;

  //DRAW BACKGROUND
  if (set == NOTHING_S){
    img = im[54];
    ind = 54;
    vg_draw_pixmap(img, hour_change_xpm, 0, 0);
  }
  else if (set == GO_BACK_S){
    img = im[56];
    ind = 56;
    vg_draw_pixmap(img, hour_change_go_back_xpm, 0, 0);
  }
  else{
    img = im[55];
    ind = 55;
    vg_draw_pixmap(img, hour_change_time_xpm, 0, 0);
  }

  //DRAW CLOCK IN MOUSE CURSOR

  if (mouse_cursor(img, ind, cursor_x, cursor_y)) return;
  memcpy(video_mem, double_buffer, 1152 * 864 * ((bits_per_pixel + 7)/8));
}

void ask_for_hours(uint8_t array[], int *num){
  if (array[0] == key_1) *num = (*num)*10 + 1;
  else if (array[0] == key_2) *num = (*num)*10 + 2;
  else if (array[0] == key_3) *num = (*num)*10 + 3;
  else if (array[0] == key_4) *num = (*num)*10 + 4;
  else if (array[0] == key_5) *num = (*num)*10 + 5;
  else if (array[0] == key_6) *num = (*num)*10 + 6;
  else if (array[0] == key_7) *num = (*num)*10 + 7;
  else if (array[0] == key_8) *num = (*num)*10 + 8;
  else if (array[0] == key_9) *num = (*num)*10 + 9;
  else if (array[0] == key_0) *num = (*num)*10 + 0;
}

void key_settings(uint8_t array[]){
  if (set == NOTHING_S){
    if (array[0] == TWO_BYTE && (array[1] == UP_KEY_BREAK || array[1] == DOWN_KEY_BREAK)){
      set = TIME_CHANGE;
    }
  }
  else if (set == TIME_CHANGE){
    if (array[0] == TWO_BYTE && array[1] == DOWN_KEY_BREAK) set = GO_BACK_S;
    else if (array[0] == ENTER_KEY_BREAK){
      state = NEW_HOUR;
      set = NOTHING_S;
    }
  }
  else if (set == GO_BACK_S){
    if (array[0] == TWO_BYTE && array[1] == UP_KEY_BREAK) set = TIME_CHANGE;
    else if (array[0] == ENTER_KEY_BREAK){
      state = IN_MAIN_MENU;
      set = NOTHING_S;
    }
  }
}

int hours_number(int *num){
  if (*num >= 23) return 23;
  else if (*num < 0) return 0;
  return *num;
}

int minutes_number(int *num){
  if (*num >= 59) return 59;
  else if (*num < 0) return 0;
  return *num;
}

void draw_clock(){
  //display seconds
  if (relogio[2] < 10){
    display_number(relogio[2], 923-80, 256, 27);
    display_number(0, 923-27-80, 256, 27);
  }
  else display_number(relogio[2], 923-80, 256, 27);

  vg_draw_rectangle(805, 256 + 14, 6, 6, 0x00ff0000);
  vg_draw_rectangle(805, 256 + 30, 6, 6, 0x00ff0000);

  //display minutes
  if (relogio[1] < 10){
    display_number(relogio[1], 855-80, 256, 27);
    display_number(0, 855-27-80, 256, 27);
  }
  else display_number(relogio[1], 855-80, 256, 27);

  vg_draw_rectangle(740, 256 + 14, 6, 6, 0x00ff0000);
  vg_draw_rectangle(740, 256 + 30, 6, 6, 0x00ff0000);

  //display hours
  if (relogio[0] < 10){
    display_number(relogio[0], 787-80, 256, 27);
    display_number(0, 787-27-80, 256, 27);
  }
  else display_number(relogio[0], 787-80, 256, 27);
}

static int num = 0;
static int save_hour = 0;
static int enter_count = 0;

void run_hour_update(uint16_t cursor_x, uint16_t cursor_y){
  //DISPLAY BACKGROUND
  
  vg_draw_pixmap(im[57], hour_change_xpm, 0, 0);

  if (enter_count == 0) display_number(hours_number(&num), 715, 385, 27);
  else if (enter_count == 1){
    display_number(save_hour, 715, 385, 27);
    display_number(minutes_number(&num), 840, 513, 27);
  }

  memcpy(video_mem, double_buffer, 1152 * 864 * ((bits_per_pixel + 7)/8));
}

void handle_input(uint8_t array[]){
  
  ask_for_hours(array, &num);
  
  if (array[0] == ENTER_KEY_BREAK){
    enter_count ++;

    if (enter_count == 1) {
      set_hour(hours_number(&num));
      save_hour = hours_number(&num);
      num = 0;
    }
    else if (enter_count == 2){
      set_minutes(minutes_number(&num));
      enter_count = 0;
      num = 0;
      state = SETTINGS;
    }
  }
}
