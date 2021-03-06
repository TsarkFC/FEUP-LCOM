// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "graphics.h"
#include "macro_graphics.h"
#include "macro.h"
#include "keyboard.h"

extern void* video_mem;

extern uint8_t code;
extern int ih_fail;
extern int bit_id_kbc;

extern int counter;
extern int bit_id;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  //set specified video mode 
  if ((vg_init)(mode) == NULL) return 1;

  //wait delay time
  tickdelay(micros_to_ticks(delay * 1000000)); //1MB

  //reset video mode
  if (vg_exit()) return 1;

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  //Keyboard interrupt variable initialization
  message msg;
  int ipc_status, r;

  uint8_t bit_num = 0;
  uint32_t irq_set = BIT(bit_num);

  uint8_t array[2];
  int two_byte_check = 0;


  //Initialize pretended graphics mode                          
  if ((vg_init(mode) == NULL)) return 1;

  //Draw rectangle
  if ((vg_draw_rectangle)(x, y, width, height, color)) return 1;

  sleep(3); //Allow time to visualize what is done

  //Check if user inputs ESC key
  if (kbc_subscribe_int(&bit_num)) return 1;

  while(code != ESQ_KEY) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */
                if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                    kbc_ih();
                    if (ih_fail) return 1;

                    if (two_byte_check){
                      array[0] = TWO_BYTE;
                      array[1] = code;
                      kbd_print_scancode(!(code & BREAK_MAKE_CODE), 2, array);
                      two_byte_check = 0;
                    }
                    else if (code == TWO_BYTE){
                      two_byte_check = 1;
                    }
                    else{
                      array[0] = code;
                      kbd_print_scancode(!(code & BREAK_MAKE_CODE), 1, array);
                    }
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

  if (kbc_unsubscribe_int()) return 1;
  
  //reset video mode
  if ((vg_exit())) return 1;

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  //Keyboard interrupt variable initialization
  message msg;
  int ipc_status, r;

  uint8_t bit_num = 0;
  uint32_t irq_set = BIT(bit_num);

  uint8_t array[2];
  int two_byte_check = 0;


  //Initialize pretended graphics mode                          
  if ((vg_init(mode) == NULL)) return 1;

  //Draw pattern
  if (vg_draw_pattern(mode, no_rectangles, first, step)) return 1;

  sleep(3); //Allow time to visualize what is done

  //Check if user inputs ESC key
  if (kbc_subscribe_int(&bit_num)) return 1;

  while(code != ESQ_KEY) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */
                if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                    kbc_ih();
                    if (ih_fail) return 1;

                    if (two_byte_check){
                      array[0] = TWO_BYTE;
                      array[1] = code;
                      kbd_print_scancode(!(code & BREAK_MAKE_CODE), 2, array);
                      two_byte_check = 0;
                    }
                    else if (code == TWO_BYTE){
                      two_byte_check = 1;
                    }
                    else{
                      array[0] = code;
                      kbd_print_scancode(!(code & BREAK_MAKE_CODE), 1, array);
                    }
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

  if (kbc_unsubscribe_int()) return 1;
  
  //reset video mode
  if ((vg_exit())) return 1;

  return 0;

}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  //Keyboard interrupt variable initialization
  message msg;
  int ipc_status, r;

  uint8_t bit_num = 0;
  uint32_t irq_set = BIT(bit_num);

  uint8_t array[2];
  int two_byte_check = 0;


  //Initialize pretended graphics mode                          
  if ((vg_init(INDEX_COLOR_MODE) == NULL)) return 1;

  //Draw pixmap
  if (vg_draw_pixmap(xpm, x, y)) return 1;

  sleep(3); //Allow time to visualize what is done

  //Check if user inputs ESC key
  if (kbc_subscribe_int(&bit_num)) return 1;

  while(code != ESQ_KEY) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */
                if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                    kbc_ih();
                    if (ih_fail) return 1;

                    if (two_byte_check){
                      array[0] = TWO_BYTE;
                      array[1] = code;
                      kbd_print_scancode(!(code & BREAK_MAKE_CODE), 2, array);
                      two_byte_check = 0;
                    }
                    else if (code == TWO_BYTE){
                      two_byte_check = 1;
                    }
                    else{
                      array[0] = code;
                      kbd_print_scancode(!(code & BREAK_MAKE_CODE), 1, array);
                    }
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

  if (kbc_unsubscribe_int()) return 1;
  
  //reset video mode
  if ((vg_exit())) return 1;

  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  //Keyboard interrupt variable initialization
  message msg;
  int ipc_status, r;

  uint8_t bit_num_key = 1;
  uint32_t irq_set_key = BIT(bit_num_key);

  uint8_t bit_num_tim = 0;
  uint32_t irq_set_tim = BIT(bit_num_tim);

  uint8_t array[2];
  int two_byte_check = 0;


  //Initialize pretended graphics mode                          
  if ((vg_init(INDEX_COLOR_MODE) == NULL)) return 1;

  //Check if user inputs ESC key and generate timer interrupts
  if (kbc_subscribe_int(&bit_num_key)) return 1;
  if (timer_subscribe_int(&bit_num_tim)) return 1;

  //Draw initial xpm
  vg_draw_pixmap(xpm, xi, yi);

  while(code != ESQ_KEY) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */
                if (msg.m_notify.interrupts & irq_set_tim){
                  timer_int_handler();
                  
                  //Ending condition
                  if (xi == xf && yi == yf) break;

                  //Positive speed situation
                  if (speed > 0){

                    if (counter == 60 / fr_rate){
                      speed_handler(xpm, &xi, &yi, xf, yf, speed);
                      counter = 0;
                    }
                  }

                  //Negative speed situation
                  else{

                    if (counter == abs(speed) * (60 / fr_rate)){
                      speed_handler(xpm, &xi, &yi, xf, yf, speed);
                      counter = 0;
                    }
                  }
                }
                
                if (msg.m_notify.interrupts & irq_set_key) { /* subscribed interrupt */
                    kbc_ih();
                    if (ih_fail) return 1;

                    if (two_byte_check){
                      array[0] = TWO_BYTE;
                      array[1] = code;
                      kbd_print_scancode(!(code & BREAK_MAKE_CODE), 2, array);
                      two_byte_check = 0;
                    }
                    else if (code == TWO_BYTE){
                      two_byte_check = 1;
                    }
                    else{
                      array[0] = code;
                      kbd_print_scancode(!(code & BREAK_MAKE_CODE), 1, array);
                    }
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

  if (kbc_unsubscribe_int()) return 1;
  if (timer_unsubscribe_int()) return 1;
  
  //reset video mode
  if ((vg_exit())) return 1;

  return 0;                     
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
