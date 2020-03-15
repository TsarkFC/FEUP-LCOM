 // IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <timer.h>
#include "keyboard.h"
#include "mouse.h"
#include "macro.h"
#include "macro_mouse.h"

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

extern int ih_fail, bit_id_mouse;
extern uint8_t read_byte;
extern int counter;


int (mouse_test_packet)(uint32_t cnt) {
    int ipc_status, r;
    message msg;

    uint8_t bit_no = 12;
    uint32_t irq_set = BIT(bit_no);


    uint8_t packet[3]; //saves bytes read from interrupt calls
    int icounter = 0; //keeps track of interrupt calls
    uint8_t  packets_cnt = 0;

    struct packet pp;

    if(mouse_command(EN_DATA_REPOR)) return 1;
    if(mouse_subscribe_int(&bit_no)) return 1;
    //if (mouse_enable_data_reporting()) return 1;


    while( packets_cnt < cnt ) {/*You may want to use a different condition*/
         /*Get a request message.*/
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);         
        continue;   
      }
      if (is_ipc_notify(ipc_status)) { /*received notification*/
        switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /*hardware interrupt notification*/

          if (msg.m_notify.interrupts & irq_set) { /*subscribed interrupt*/
            mouse_ih();

            //checks if read_byte is the first byte of the packet
            if (icounter == 0){
              if (!(read_byte & FIRST_BYTE)) break;
            }

            packet[icounter] = read_byte;
            icounter ++;

            if (icounter == 3){ //mouse packet is complete
              //set packet's struct and prints it
              set_struct(packet, &pp);
              mouse_print_packet(&pp);
              packets_cnt++; //increments variable that counts number of packets printed
              icounter = 0;
            }
          }
          break;
        default:            
          break; /*no other notifications expected: do nothing*/         
        }     
      } 
      else  { /*received a standard message, not a notification*/         
      /*no standard messages expected: do nothing*/
      } 
    }

  //disable data reporting and unsubscribe interrupts
  if (mouse_unsubscribe_int()) return 1;
  if (mouse_command(DISABLE_DATA_REPOR)) return 1;

  return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    //disable MINIX mouse
    if (disable_mouse()) return 1;

    //set remote mode
    if (mouse_command(DISABLE_DATA_REPOR)) return 1;
    if (mouse_command(REMOTE_MODE)) return 1;

    //poll KBC status register to read bytes
    if (mouse_poll(period, cnt)) return 1;

    //reset mouse/kbc state
    if (enable_mouse_kbc()) return 1;

    //set stream mode back again
    if (mouse_command(STREAM_MODE))return 1;
    if (mouse_command(DISABLE_DATA_REPOR)) return 1;

    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    /* To be completed */
    uint32_t time = 0;
    int icounter = 0;
    ///////.--.
    uint8_t bit_no2 = 0;
    uint32_t timer_irq_set = BIT(bit_no2); 
    timer_subscribe_int(&bit_no2);
    ////////.--.

    ///////////
    int ipc_status, r;
    message msg;

    uint8_t bit_no = 12;
    uint32_t irq_set = BIT(bit_no);


    uint8_t packet[3]; //saves bytes read from interrupt calls
    uint8_t  packets_cnt = 0;

    struct packet pp;

    if(mouse_command(EN_DATA_REPOR)) return 1;
    if(mouse_subscribe_int(&bit_no)) return 1;
    //if (mouse_enable_data_reporting()) return 1;


    while( time < idle_time ) {/*You may want to use a different condition*/
         /*Get a request message.*/
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);         
        continue;   
      }
      if (is_ipc_notify(ipc_status)) { /*received notification*/
        switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /*hardware interrupt notification*/

          if (msg.m_notify.interrupts & irq_set) { /*subscribed interrupt*/
            mouse_ih();
            counter = 0;

            //checks if read_byte is the first byte of the packet
            if (icounter == 0){
              if (!(read_byte & FIRST_BYTE)) break;
            }

            packet[icounter] = read_byte;
            icounter ++;


            if (icounter == 3){ //mouse packet is complete
              //set packet's struct and prints it
              set_struct(packet, &pp);
              mouse_print_packet(&pp);
              packets_cnt++; //increments variable that counts number of packets printed
              icounter = 0;
            }
          }
          if (msg.m_notify.interrupts & timer_irq_set){
              timer_int_handler();
              if(counter%sys_hz() == 0) time++;
          }
          break;
        default:            
          break; /*no other notifications expected: do nothing*/         
        }     
      } 
      else  { /*received a standard message, not a notification*/         
      /*no standard messages expected: do nothing*/
      } 
    }

  //disable data reporting and unsubscribe interrupts
  if (timer_unsubscribe_int()) return 1;
  if (mouse_unsubscribe_int()) return 1;
  if (mouse_command(DISABLE_DATA_REPOR)) return 1;

  return 0;






    //printf("%s(%u): under construction\n", __func__, idle_time);
    //return 1;
}





int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    int ipc_status, r;
    message msg;

    uint8_t bit_no = 12;
    uint32_t irq_set = BIT(bit_no);

    uint8_t packet[3]; //saves bytes read from interrupt calls
    uint8_t  packets_cnt = 0;
    int icounter = 0; //keeps track of interrupt calls

    struct packet pp;
    
    bool Complete = false;

    if(mouse_command(EN_DATA_REPOR)) return 1;
    if(mouse_subscribe_int(&bit_no)) return 1;

   /////////while

    while( !Complete ) {/*You may want to use a different condition*/
         /*Get a request message.*/
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);         
        continue;   
      }
      if (is_ipc_notify(ipc_status)) { /*received notification*/
        switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /*hardware interrupt notification*/

          if (msg.m_notify.interrupts & irq_set) { /*subscribed interrupt*/
            mouse_ih();

            //start\\ //checks if read_byte is the first byte of the packet
            if (icounter == 0){
              if (!(read_byte & FIRST_BYTE)) break;
            }
            //end
            packet[icounter] = read_byte;
            icounter ++;

            if (icounter == 3){ //mouse packet is complete
              //set packet's struct and prints it
              set_struct(packet, &pp);
              mouse_print_packet(&pp);
              packets_cnt++; //increments variable that counts number of packets printed
              icounter = 0;

              //Implement here * //
              Complete = event_state_processing(&pp,x_len,tolerance);
              //HERE
              //HERE
              //HERE
              //HERE
            }
          }
          break;
        default:            
          break; /*no other notifications expected: do nothing*/         
        }     
      } 
      else  { /*received a standard message, not a notification*/         
      /*no standard messages expected: do nothing*/
      } 
    }

    //////////end-while




    printf("%s: under construction\n", __func__);
    return 1;




    if (mouse_unsubscribe_int()) return 1;
  	if (mouse_command(DISABLE_DATA_REPOR)) return 1;

}
