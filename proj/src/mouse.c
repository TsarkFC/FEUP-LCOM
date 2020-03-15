#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "headers/macro_mouse.h"
#include "headers/keyboard.h"
#include "headers/macro.h"
#include "headers/mouse.h"

int bit_id_mouse;
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
      if (((stat & (PAR_ERR | TO_ERR)) == 0)) { //Parity and Time Out errors check*/
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
