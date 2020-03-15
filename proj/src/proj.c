// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

// Any header files included below this line should have been created by you
#include "headers/display.h"
#include "headers/macro.h"
#include "headers/macro_graphics.h"
#include "headers/macro_mouse.h"
#include "headers/mouse.h"
#include "headers/keyboard.h"
#include "headers/timer.h"
#include "headers/graphics.h"
#include "headers/rtc.h"
#include "headers/single_player.h"
#include "headers/multi_player.h"
#include "headers/settings.h"
#include "headers/serial_port.h"

/** @defgroup Proj Proj
 * @{
 * Contains the main(), proj_main_loop() funtions.
 */

/**
 * @brief Program's main function
 * 
 */
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

//GLOBAL VARIABLES

extern int ih_fail; //flag that detects if kbc interrupt handler function has failed
extern uint8_t code; //used to get scancodes from keyboard input
extern int counter; //counter that is incremented in each timer interrupt
extern int bit_id_mouse;
extern unsigned bits_per_pixel; //video mem bits per pixel

extern void* video_mem;
extern uint8_t *double_buffer; //DOUBLE DUFFERING

extern xpm_image_t im[]; //ALL IMAGES

extern enum select_device device_sel;

int ih_mouse_fail; //checks if interrupt handler function was called properly
uint8_t read_byte; //variable containing byte read in mouse_ih()

//STATE OF PROGRAM RECOGNITION
enum display menu = MAIN_MENU; //MENU STATE CHECKER
enum display_single single_menu = SINGLE_MENU;
enum display_multi multi_menu = MULTI_MENU;
enum state_of_program state = IN_MAIN_MENU; //PROGRAM STARTS AT MAIN MENU
enum select_pause pause_sel = NOTHING;




// USE FOR PROJECT MENU CALLS 
/**
 * @brief Function which contains the driver receive loop and keeps the program running
 * 
 */
int(proj_main_loop)(int argc, char *argv[]) {

  //IMAGES LOADING
  load_images();
    
  //VARIABLE DECLARATION FOR INTERRUPTS LOOP (HOOK ID'S, etc.)

  message msg;
  int ipc_status, r;

  uint8_t bit_num_key = 1;
  uint32_t irq_set_key = BIT(bit_num_key);

  uint8_t bit_num_tim = 0;
  uint32_t irq_set_tim = BIT(bit_num_tim);

  uint8_t bit_num_mou = 12;
  uint32_t irq_set_mou = BIT(bit_num_mou);

  uint8_t bit_num_rtc = 8;
  uint32_t irq_set_rtc = BIT(bit_num_rtc);

  uint8_t bit_num_ser_1 = 4;

  uint8_t bit_num_ser_2 = 3;
  uint32_t irq_set_ser = BIT(bit_num_ser_2) | BIT(bit_num_ser_1);

  //KEYBOARD
  uint8_t array[2];
  int two_byte_check = 0;

  //MOUSE
  uint8_t packet[3];
  struct packet pp;
  int icounter = 0;
  
  //CURSOR COORDINATES
  int cursor_x = 1152/2;
  int cursor_y = 864/2;

  //GAME FREQUENCY
  unsigned game_frequency = 2;

  //SERIAL PORT
  bool subscribe = true;

  //Initialize pretended graphics mode                          
  if ((vg_init(0x14c) == NULL)) return 1;

  //DOUBLE BUFFERING INITIALIZATION
  double_buffer = (uint8_t *) malloc(1152 * 864 * (bits_per_pixel + 7) / 8);

  //SUBSCRIBE INTERRUPTS FOR EACH DEVICE
  if (mouse_subscribe_int(&bit_num_mou)) return 1;

  sys_irqdisable(&bit_id_mouse);
  if (mouse_command(EN_DATA_REPOR)) return 1;
  sys_irqenable(&bit_id_mouse);

  if (kbc_subscribe_int(&bit_num_key)) return 1;
  if (timer_subscribe_int(&bit_num_tim)) return 1;
  if (rtc_subscribe_int(&bit_num_rtc)) return 1;

  //INTERRUPTS LOOP
  while(code != 0xa7 && !get_out_f()) {

    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */

                //HANDLE TIMER INTERRUPTS

                if (msg.m_notify.interrupts & irq_set_tim){
                  timer_int_handler();

                  if ((unsigned)counter % game_frequency == 0){
          
                    //CHECK STATE OF PROGRAM
                    switch(state){
                      case IN_MAIN_MENU:
                        //MENU CALLS
                        display_menu((uint16_t)cursor_x, (uint16_t)cursor_y);
                        break;
                      case IN_S_PLAYING_1:
                        display_initial_countdown((uint16_t)cursor_x, (uint16_t)cursor_y);
                        break;
                      case IN_S_PLAYING_2:
                        display_initial_countdown((uint16_t)cursor_x, (uint16_t)cursor_y);
                        break;
                      case IN_S_PLAYING_3:
                        display_initial_countdown((uint16_t)cursor_x, (uint16_t)cursor_y);
                        break;
                      case IN_M_PLAYING_1:
                        display_initial_countdown_multi((uint16_t)cursor_x, (uint16_t)cursor_y);
                        break;
                      case IN_M_PLAYING_2:
                        display_initial_countdown_multi((uint16_t)cursor_x, (uint16_t)cursor_y);
                        break;
                      case IN_M_PLAYING_3:
                        display_initial_countdown_multi((uint16_t)cursor_x, (uint16_t)cursor_y);
                        break;

                      case SEL_DEVICE:{
                        xpm_image_t img;
                        int ind;

                        switch(device_sel){
                          case ONE:
                            img = im[58];
                            ind = 58;
                            vg_draw_pixmap(img, select_1_xpm, 0, 0);
                            break;
                          case TWO:
                            img = im[59];
                            ind = 59;
                            vg_draw_pixmap(img, select_2_xpm, 0, 0);
                            break;
                        }
                        if (mouse_cursor(img, ind, cursor_x, cursor_y)) return 1;
                        memcpy(video_mem, double_buffer, 1152 * 864 * ((bits_per_pixel + 7)/8));
                        break;
                      }
                      
                      case WAIT_CONNECTION:
                        //display waiting connection
                        vg_draw_pixmap(im[60], waiting_connection_xpm, 0, 0);
                        memcpy(video_mem, double_buffer, 1152 * 864 * ((bits_per_pixel + 7)/8));

                        if (subscribe) {
                          serial_subscribe_int(get_device(), &bit_num_ser_1, &bit_num_ser_2);
                          subscribe = false;
                        }

                        if (connection_linker(get_device())) state = IN_SERIAL_PLAYING;

                        break;

                      case IN_SERIAL_PLAYING:
                        display_initial_countdown_multi((uint16_t)cursor_x, (uint16_t)cursor_y);
                        break;

                      case IN_LEVEL_SEL_S:
                        display_single_menu((uint16_t)cursor_x, (uint16_t)cursor_y);
                        break;
                      case IN_LEVEL_SEL_M:
                        display_multi_menu((uint16_t)cursor_x, (uint16_t)cursor_y);
                        break;
                      case S_WON_GAME:
                        vg_draw_pixmap(im[51], success_xpm, 0, 0);
                        display_number(get_possible_record(), 596, 554, 27);
                        memcpy(video_mem, double_buffer, 1152 * 864 * ((bits_per_pixel + 7)/8));
                        reset_single_player();
                        break;
                      case S_LOST_GAME:
                        vg_draw_pixmap(im[50], gameover_xpm, 0, 0);
                        memcpy(video_mem, double_buffer, 1152 * 864 * ((bits_per_pixel + 7)/8));
                        reset_single_player();
                        break;
                      case M1_WON_GAME:
                        vg_draw_pixmap(im[52], player1_won_xpm, 0, 0);
                        memcpy(video_mem, double_buffer, 1152 * 864 * ((bits_per_pixel + 7)/8));
                        reset_multi_player();
                        break;
                      case M2_WON_GAME:
                        vg_draw_pixmap(im[53], player2_won_xpm, 0, 0);
                        memcpy(video_mem, double_buffer, 1152 * 864 * ((bits_per_pixel + 7)/8));
                        reset_multi_player();
                        break;
                      case PAUSE_S1:
                        display_pause();
                        break;
                      case PAUSE_S2:
                        display_pause();
                        break;
                      case PAUSE_S3:
                        display_pause();
                        break;
                      case PAUSE_M1:
                        display_pause();
                        break;
                      case PAUSE_M2:
                        display_pause();
                        break;
                      case PAUSE_M3:
                        display_pause();
                        break;
                      case SETTINGS:
                        run_settings((uint16_t) cursor_x, (uint16_t) cursor_y);
                        break;
                      case NEW_HOUR:
                        run_hour_update((uint16_t) cursor_x, (uint16_t) cursor_y);
                        break;        
                    }
                  }
                }

                //HANDLE KEYBOARD INTERRUPTS

                if (msg.m_notify.interrupts & irq_set_key) { /* subscribed interrupt */

                  kbc_ih();

                  if (ih_fail) return 1;

                  if (two_byte_check){
                    array[0] = TWO_BYTE;
                    array[1] = code;

                    two_byte_check = 0;

                    //kbd_print_scancode(!(code & BREAK_MAKE_CODE), 2, array);
                    key_handler(array);
                  }

                  else if (code == TWO_BYTE){
                    two_byte_check = 1;
                  }

                  else{
                    array[0] = code;

                    //kbd_print_scancode(!(code & BREAK_MAKE_CODE), 1, array);
                    key_handler(array);
                  }
                }

                //HANDLE MOUSE INTERRUPTS

                if (msg.m_notify.interrupts & irq_set_mou){
                  mouse_ih();

                  if (ih_mouse_fail) return 1;

                  //checks if read_byte is the first byte of the packet
                  if (icounter == 0){
                    if (!(read_byte & FIRST_BYTE)) break;
                  }

                  packet[icounter] = read_byte;
                  icounter ++;

                  if (icounter == 3){ //mouse packet is complete
                    //set packet's struct
                    set_struct(packet, &pp);

                    //hadle mouse movement / clicks
                    mouse_handler(pp, &cursor_x, &cursor_y);
                    //resets counter value
                    icounter = 0;
                  }
                }
                if (msg.m_notify.interrupts & irq_set_rtc){
                  rtc_ih();
                }
                if (msg.m_notify.interrupts & irq_set_ser){
                  serial_ih(get_device());
                  printf("GOT INTERRUPTS! \n");
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */
        }
    }
    else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }

    
  }

  sys_irqdisable(&bit_id_mouse);
  if (mouse_command(DISABLE_DATA_REPOR)) return 1;
  sys_irqenable(&bit_id_mouse);

  if (mouse_unsubscribe_int()) return 1;
  if (kbc_unsubscribe_int()) return 1;
  if (timer_unsubscribe_int()) return 1;
  if (rtc_unsubscribe_int()) return 1;

  if (subscribe == false) serial_unsubscribe_int(get_device());

  if (vg_exit()) return 1;

  return 0;
}

/**@} end of Proj*/
