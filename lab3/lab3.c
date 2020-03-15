#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"
#include "macro.h"
#include "timer.h"
#include "i8254.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}
extern uint8_t stat; //status register
extern uint8_t code; //scancode
extern uint32_t cnt; //sys_inb calls counter
extern int ih_fail; 

extern int counter; //timer counter

int(kbd_test_scan)() {
  
  uint8_t array[2]; //array of size 2 to save scancode bytes
  uint8_t bit_num = 0;
  int ipc_status, r;
  message msg;
  int two_byte_check = 0; 

  uint32_t irq_set = BIT(bit_num); //32 bit mask, bit (bit_num = 0) has value 1

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

  //call once ESC is pressed
  kbd_print_no_sysinb(cnt);

  return 0;
}

int(kbd_test_poll)() {
  uint8_t array[2]; //array of size 2 to save scancode bytes
  int two_byte_check = 0;

  while (code != ESQ_KEY){;
    if (get_scancode()) return 1;

    if (two_byte_check){ //scancode has two bytes
      array[0] = TWO_BYTE;
      array[1] = code;
      kbd_print_scancode(!(code & BREAK_MAKE_CODE), 2, array);
      two_byte_check = 0;
    }
    else if (code == TWO_BYTE) {
      two_byte_check = 1;
    }
    else{ //scancode has one byte
      array[0] = code;
      kbd_print_scancode(!(code & BREAK_MAKE_CODE), 1, array);
    }
    tickdelay(micros_to_ticks(WAIT_KBC));
  }

  //call once ESC is pressed
  kbd_print_no_sysinb(cnt);

  if(enable_interrupts()) return 1; //enable keyboard after polling 

  return 0;
}

int (kbd_test_timed_scan)(uint8_t n) {
  //Variables used to subscribe interrupts which cannot have the same value
  uint8_t bit_no_timer = 0;
  uint8_t bit_no_kbc = 1;

  int two_byte_check = 0;
  uint8_t array[2];
  message msg;
  int r, ipc_status;

  if (timer_subscribe_int(&bit_no_timer)) return 1;
  if (kbc_subscribe_int(&bit_no_kbc)) return 1;

  while ((code != ESQ_KEY) && (counter / 60 < n)){ // n = IDLE
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /*hardware interrupt notification*/
          if (msg.m_notify.interrupts & BIT(TIMER0_IRQ) ) { /*subscribed interrupt*/
            /*process timer0 interrupt request*/              
            timer_int_handler();
          }
          if (msg.m_notify.interrupts & BIT(KBD_IRQ)) { /*subscribed interrupt*/
            /*process KBD interrupt request*/             
            kbc_ih();
            if (ih_fail) return 1;
            counter = 0;

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
          break; /*no other notifications expected: do nothing*/
      }
    }
  }

  if (timer_unsubscribe_int()) return 1;
  if (kbc_unsubscribe_int()) return 1;

  return 0;
}
