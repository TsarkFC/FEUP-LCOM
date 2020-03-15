#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "macro_mouse.h"
#include "keyboard.h"
#include "macro.h"
#include "mouse.h"

int bit_id_mouse = 12;
int ih_mouse_fail; //checks if interrupt handler function was called properly
uint8_t read_byte; //byte read during interrupts

int (mouse_subscribe_int)(uint8_t *bit_no) {
  //bit_no is preserved
  bit_id_mouse = (int) *bit_no;
  return (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &bit_id_mouse));
}

int (mouse_unsubscribe_int)() {
  return sys_irqrmpolicy(&bit_id_mouse);
}

void (mouse_ih)(){
  uint8_t stat; //status register

  while( 1 ) {
    if (util_sys_inb(STAT_REG, &stat)){ih_mouse_fail = 1; break;} //gets status register value
    if( stat & OBF ) { //checks if output buffer is set
      if (((stat &(PAR_ERR | TO_ERR )) == 0)) { //Parity, Time Out and Mouse errors check
        if(util_sys_inb(OUT_BUF, &read_byte)) ih_mouse_fail = 1; //Reads byte value
        else ih_mouse_fail = 0;
        break;
      } 
      else ih_mouse_fail = 1;
      break;
    }
  }
}

void set_struct(uint8_t array[], struct packet *pp){
  //set bytes
  pp->bytes[0] = array[0];
  pp->bytes[1] = array[1];
  pp->bytes[2] = array[2];

  //set delta variables
  if (array[0] & X_SIGN) pp->delta_x = array[1] - NEG_TO_POS; //x has negative sign
  else pp->delta_x = array[1]; //x has postive sign;

  if (array[0] & Y_SIGN) pp->delta_y = array[2] - NEG_TO_POS; //y has negative sign
  else pp->delta_y = array[2]; //y has positive sign

  //set first byte components
  pp->lb = array[0] & LB;
  pp->rb = (array[0] & RB);
  pp->mb = (array[0] & MB);
  pp->x_ov = (array[0] & X_OVFL);
  pp->y_ov = (array[0] & Y_OVFL);
}

int send_command(uint8_t port, uint8_t command){
  uint8_t stat; //status register

  while( 1 ) {
    if (util_sys_inb(STAT_REG, &stat)) return 1; /*assuming it returns OK*/
    /*loop while 8042 input buffer is not empty*/
    if( (stat & IBF) == 0 ) {
      sys_outb(port, command); //sends read command to port 0x64
      return 0;
    }
    else return 1;
  }
}

int receive_command(uint8_t* command){
  uint8_t stat; //status register

  while( 1 ) {
    if (util_sys_inb(STAT_REG, &stat)) return 1; //gets status register value
    if( (stat & OBF)) { //checks if output buffer is set
      if (((stat & (PAR_ERR | TO_ERR)) == 0)) { //Parity and Time Out errors check
        if(util_sys_inb(OUT_BUF, command)) return 1; //reads command byte from port 0x60
        return 0;
      } 
      else return 1;
    }
  }
  return 0;
}

int mouse_command(uint8_t cmd){
  while (1){
    //sends 0xD4 to CTRL

    if (send_command(KBC_CMD_REG, WRT_BYTE_MOUSE)) return 1;

    //CMD to INB
    if (send_command(IN_BUF, cmd)) return 1;

    //receives ACK
    uint8_t received = 1;
    if (receive_command(&received)) return 1;

    if (received == NACK || received == ERROR) return 1;
    else if (received == ACK) return 0;
    //received está sempre a 0
  }
  return 0;
}

int disable_mouse(){
  uint8_t cmd = 1;
  if (send_command(KBC_CMD_REG, READ_COMMAND)) return 1; //enables reading
  if (receive_command(&cmd)) return 1; //receives KBC command byte
  cmd = cmd | MOU_DIS; //disables mouse
  if (send_command(KBC_CMD_REG, WRITE_COMMAND)) return 1; 
  if (send_command(OUT_BUF, cmd)) return 1; //writes new KBC command byte
  return 0;
}

int enable_mouse_kbc(){
  if (send_command(KBC_CMD_REG, WRITE_COMMAND)) return 1; 
  if (send_command(OUT_BUF, minix_get_dflt_kbc_cmd_byte())) return 1; //writes new KBC command byte
  return 0;
}


int mouse_poll(uint16_t period, uint8_t cnt) {
  struct packet pp;
  
  uint8_t stat = 0;
  uint8_t packet[3]; //saves bytes read from interrupt calls
  int counter = 0; //keeps track of interrupt calls
  uint8_t  packets_cnt = 0;
  bool check_read = false;

  while( packets_cnt < cnt ) {
    if (counter == 0 && !check_read) mouse_command(READ_DATA);

    if (util_sys_inb(STAT_REG, &stat))return 1; //gets status register value
    if( stat & OBF ) { //checks if output buffer is set
      if (((stat &(PAR_ERR | TO_ERR )) == 0)) { //Parity, Time Out and Mouse errors check
        if(util_sys_inb(OUT_BUF, &read_byte)) return 1; //Reads byte value
        check_read = true;
      } 
      else return 1;
    }
    else{
      tickdelay(micros_to_ticks(period));
      continue;
    } 

    if (!check_read) break; //checks if anything was received

    //checks if read_byte is the first byte of the packet
    if (counter == 0){
      if (!(read_byte & FIRST_BYTE)) break;
    }

    packet[counter] = read_byte;
    counter++;

    if(counter == 3){
      set_struct(packet, &pp);
      mouse_print_packet(&pp);
      counter = 0;
      check_read = false;
      packets_cnt++;
    }

    tickdelay(micros_to_ticks(period));
  }
  return 0;
}


// - DFA - \\//

int x_length = 0, y_length = 0;
bool drawing_first = false,first_line_done = false,drawing_second = false;


void RESET_LEN(){
  x_length = 0;
  y_length = 0;
}

bool event_state_processing(struct packet *pp,uint8_t x_len,uint8_t tolerance){

  enum EVENT event;
  static enum STATE state = Start;

  //Which event? - Defines the event that occurred
  if (pp->lb && !pp->rb && !pp->mb){ //only the left button on
    event = LeftDown;
    drawing_first = true;
  }
  else if (!pp->lb && !pp->rb && !pp->mb && drawing_first){ //Left button released 
    event = LeftUp;
    drawing_first = false;
  }
  else if (!pp->lb && pp->rb && !pp->mb){ //only the right button on
    event = RightDown;
    if(first_line_done) drawing_second = true;
  }
  else if (pp->lb && !pp->rb && !pp->mb && drawing_second ){ //Left button released 
    event = RightUp;
    drawing_second = false;
  }
  else if(!(!pp->lb && !pp->rb && !pp->mb)) event = WrongKey;  



 //(pp->delta_y <= pp->delta_x) state = Start;


  switch(state){
    case Start:
      printf("Start \n");
      RESET_LEN();
      if(event == LeftDown) state = Line1;
      break;

    case Line1:
      printf("Line1 \n");
      if(event == LeftDown){
        if((pp->delta_x < 0 && abs(pp->delta_x) > tolerance )||(pp->delta_y < 0 && abs(pp->delta_y) > tolerance)){
          state = Start;
        }
        else {
          printf("ajncka \n");
          printf("y_length: %d \n", y_length);
          printf("x_lenght: %d \n", x_length);
          if(y_length < x_length) {
            printf("wrong slope \n");
            state = Start;
          }
          else {
            printf("GOT IN INCREMENT \n");
            x_length += pp->delta_x;
            y_length += pp->delta_y;
          }
        }
      }

      else if(event == LeftUp) {
        printf("GOT IN \n");
        printf("x: %d \n", x_length);
        printf("y: %d \n", y_length);
        printf("x > x_len: %d \n", x_length >= x_len);
        printf("slope: %d \n", y_length > x_length);

        if((x_length >= x_len && (y_length > x_length)) && (abs(pp->delta_x) <= tolerance && abs(pp->delta_y) <= tolerance )){
          state = Vertex;
          first_line_done = true;
        }
        else state = Start;
      }
      else state = Start; //back to the Start state
      break;


    case Vertex:
      printf("Vertex \n");
      if(event == RightDown){
        state = Line2;
        RESET_LEN();
      }
      else if(event == LeftDown || event == WrongKey) state = Start;
      break;




    case Line2:
      printf("Line2 \n");
      if(event == RightDown){
        if( abs(pp->delta_x) <= tolerance && abs(pp->delta_y) <= tolerance){
          printf("x: %d \n", pp->delta_x);
          printf("y: %d \n", pp->delta_y);
          if (abs(pp->delta_y) <= pp->delta_x) state = Start; //Check slope for each packet 
          x_length += pp->delta_x;
        }
        else state = Start;
      }
      else if(event == RightUp){
        if((x_length >= x_len && abs(pp->delta_y) > pp->delta_x) && (abs(pp->delta_x) <= tolerance && abs(pp->delta_y) <= tolerance )){
          return true;
        }
        else state = Start;
      }
      else state = Start;

      break;
    default:
      break;
  }
  return false;
}





















