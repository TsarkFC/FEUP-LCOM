// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "headers/serial_port.h"
#include "headers/macro.h"

int bit_id_serial1; //hook id for com1
int bit_id_serial2; //hook id for com2
uint32_t last_received1 = 0;
uint32_t last_received2 = 0;

//PLAYER 1 RECEIVES PLAYER 2 INFORMATION FROM COM1
void serial_subscribe1_read_int(uint8_t *bit_no){
  bit_id_serial1 = *bit_no;
  sys_irqsetpolicy(COM1_IRQ, IRQ_EXCLUSIVE | IRQ_REENABLE, &bit_id_serial1);

  uint32_t ier = 0;

  sys_inb(COM1 + UART_IER, &ier);
  ier = ier | EN_DATA_RECEIVED;
  sys_outb(COM1 + UART_IER, ier);
}

void serial_unsubscribe1_read_int(){
  uint32_t ier = 0;

  sys_inb(COM1 + UART_IER, &ier);
  ier = ier & ~EN_DATA_RECEIVED;
  sys_outb(COM1 + UART_IER, ier);

  sys_irqrmpolicy(&bit_id_serial1);
}

//PLAYER 1 SENDS INFO TO PLAYER 2 FROM COM2
void serial_subscribe1_send_int(uint8_t *bit_no){
  bit_id_serial2 = *bit_no;
  sys_irqsetpolicy(COM2_IRQ, IRQ_EXCLUSIVE | IRQ_REENABLE, &bit_id_serial2);

  uint32_t ier = 0;

  sys_inb(COM2 + UART_IER, &ier);
  ier = ier | EN_TRANS_HOL_REG_EMPTY;
  sys_outb(COM2 + UART_IER, ier);
}

void serial_unsubscribe1_send_int(){
  uint32_t ier = 0;

  sys_inb(COM2 + UART_IER, &ier);
  ier = ier & ~EN_TRANS_HOL_REG_EMPTY;
  sys_outb(COM2 + UART_IER, ier);

  sys_irqrmpolicy(&bit_id_serial2);
}

//PLAYER 2 RECEIVES INFO FROM PLAYER 1 FROM COM2
void serial_subscribe2_read_int(uint8_t *bit_no){
  bit_id_serial2 = *bit_no;
  sys_irqsetpolicy(COM2_IRQ, IRQ_EXCLUSIVE | IRQ_REENABLE, &bit_id_serial2);

  uint32_t ier = 0;

  sys_inb(COM2 + UART_IER, &ier);
  ier = ier | EN_DATA_RECEIVED;
  sys_outb(COM2 + UART_IER, ier);
}

void serial_unsubscribe2_read_int(){
  uint32_t ier = 0;

  sys_inb(COM2 + UART_IER, &ier);
  ier = ier & ~EN_DATA_RECEIVED;
  sys_outb(COM2 + UART_IER, ier);

  sys_irqrmpolicy(&bit_id_serial2);
}

//PLAYER 2 SENDS INFO TO PLAYER 1 FROM COM1

void serial_subscribe2_send_int(uint8_t *bit_no){
  bit_id_serial1 = *bit_no;
  sys_irqsetpolicy(COM1_IRQ, IRQ_EXCLUSIVE | IRQ_REENABLE, &bit_id_serial1);

  uint32_t ier = 0;

  sys_inb(COM1 + UART_IER, &ier);
  ier = ier | EN_TRANS_HOL_REG_EMPTY;
  sys_outb(COM1 + UART_IER, ier);
}

void serial_unsubscribe2_send_int(){
  uint32_t ier = 0;

  sys_inb(COM1 + UART_IER, &ier);
  ier = ier & ~EN_TRANS_HOL_REG_EMPTY;
  sys_outb(COM1 + UART_IER, ier);

  sys_irqrmpolicy(&bit_id_serial1);
}

void serial_ih(int device){
  uint32_t iir = 0;
  uint32_t lsr = 0;

  if (device == 1){
    sys_inb(COM1 + UART_IIR, &iir);

    //GOT INTERRUPT
    if ((iir & GOT_INTERRUPT) == 0){
      //RECEIVED DATA
      if (iir & RECEIVED_DATA_AVAILABLE){
        last_received1 = receive_info(device);
      }
    }

    //TRANSMITTING DATA
    sys_inb(COM2 + UART_IIR, &iir);
    if (iir & TRANS_HOL_REG_EMPTY_IIR){
      sys_inb(COM2 + UART_LSR, &lsr);
      lsr = lsr | TRANS_HOL_REG_EMPTY;
      sys_outb(COM2 + UART_LSR, lsr);
    }
  }

  else if (device == 2){
    //GOT INTERRUPT
    sys_inb(COM2 + UART_IIR, &iir);
    if ((iir & GOT_INTERRUPT) == 0){
      //RECEIVED DATA
      if (iir & RECEIVED_DATA_AVAILABLE){
        last_received2 = receive_info(device);
      }
    }

    //TRANSMITTING DATA
    sys_inb(COM1 + UART_IIR, &iir);
    if ((iir & GOT_INTERRUPT) == 0){
      if (iir & TRANS_HOL_REG_EMPTY_IIR){
        sys_inb(COM1 + UART_LSR, &lsr);
        lsr = lsr | TRANS_HOL_REG_EMPTY;
        sys_outb(COM1 + UART_LSR, lsr);
      }
    }
  }
}

uint32_t receive_info(int device){
  uint32_t info = 0;
  if (device == 1) sys_inb(COM1 + UART_RBR, &info);
  else if (device == 2) sys_inb(COM2 + UART_RBR, &info);
  return info;
}

void send_info(uint32_t info, int device){
  uint32_t lsr = 0;

  if (device == 1){
    sys_inb(COM2 + UART_LSR, &lsr);
    if (lsr & TRANS_HOL_REG_EMPTY) sys_outb(COM2 + UART_THR, info);
  }
  else if (device == 2){
    sys_inb(COM1 + UART_LSR, &lsr);
    if (lsr & TRANS_HOL_REG_EMPTY) sys_outb(COM2 + UART_THR, info);
  }
}

void serial_subscribe_int(int device, uint8_t *hook_id1, uint8_t *hook_id2){
  if (device == 1){
    serial_subscribe1_read_int(hook_id1);
    serial_subscribe1_send_int(hook_id2);
  }
  else if (device == 2){
    serial_subscribe2_read_int(hook_id2);
    serial_subscribe2_send_int(hook_id1);
  }
}

void serial_unsubscribe_int(int device){
  if (device == 1){
    serial_unsubscribe1_read_int();
    serial_unsubscribe1_send_int();
  }
  else if (device == 2){
    serial_unsubscribe2_read_int();
    serial_unsubscribe2_send_int();
  }
}

bool connection_linker(int device){
  if (device == 1){
    send_info(32, 1);

    if (last_received2 == 32) return true;
  }
  else if (device == 2){
    send_info(32, 2);

    if (last_received1 == 32) return true;
  }

  return false;
}
