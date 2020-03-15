// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>
#include "headers/rtc.h"
#include "headers/graphics.h"

int bit_id_rtc;
uint32_t relogio[3];

int rtc_subscribe_int(uint8_t * bit_no){
  bit_id_rtc = *bit_no;
  sys_irqsetpolicy(RTC_IRQ, IRQ_EXCLUSIVE | IRQ_REENABLE, &bit_id_rtc);

  uint32_t reg_b;

  //ENABLE INTENDED INTERRUPT SOURCE
  sys_outb(RTC_ADDR_REG, RTC_REG_B);
  sys_inb(RTC_DATA_REG, &reg_b);

  //SET VALUES TO BINARY
  reg_b = reg_b | UIE | DM;

  sys_outb(RTC_ADDR_REG, RTC_REG_B);
  sys_outb(RTC_DATA_REG, reg_b);

  return 0;
}

int rtc_unsubscribe_int(){
  //RESET REGISTER B STATE
  uint32_t reg_b;

  sys_outb(RTC_ADDR_REG, RTC_REG_B);
  sys_inb(RTC_DATA_REG, &reg_b);

  reg_b = reg_b & ~UIE;

  sys_outb(RTC_ADDR_REG, RTC_REG_B);
  sys_outb(RTC_DATA_REG, reg_b);

  //UNSUBSCRIBE
  sys_irqrmpolicy(&bit_id_rtc);

  return 0;
}

void inhibit_updates_true(){
  uint32_t reg_b = 0;

  sys_outb(RTC_ADDR_REG, RTC_REG_B);
  sys_inb(RTC_DATA_REG, &reg_b);

  reg_b = reg_b | SET;

  sys_outb(RTC_ADDR_REG, RTC_REG_B);
  sys_outb(RTC_DATA_REG, reg_b);
}

void inhibit_updates_false(){
  uint32_t reg_b = 0;

  sys_outb(RTC_ADDR_REG, RTC_REG_B);
  sys_inb(RTC_DATA_REG, &reg_b);

  reg_b = reg_b & ~SET;

  sys_outb(RTC_ADDR_REG, RTC_REG_B);
  sys_outb(RTC_DATA_REG, reg_b);
}

void wait_valid_rtc() {
  uint32_t regA = 0;
  do {
    sys_irqdisable(&bit_id_rtc);

    sys_outb(RTC_ADDR_REG, RTC_REG_A);
    sys_inb(RTC_DATA_REG, &regA);

    sys_irqenable(&bit_id_rtc);

  } while ( regA & RTC_UIP);
}

void rtc_ih() {
  uint32_t regC;

  sys_outb(RTC_ADDR_REG, RTC_REG_C);
  sys_inb(RTC_DATA_REG, &regC);

  if( regC & RTC_UF ) handle_update_int();
}

int handle_update_int(){
  relogio[0] = get_hour();
  relogio[1] = get_minutes();
  relogio[2] = get_seconds();
  return 0;
}

void update_record(uint32_t time, int port){
  uint32_t record = 0;

  wait_valid_rtc();

  sys_outb(RTC_ADDR_REG, port);
  sys_inb(RTC_DATA_REG, &record);

  if ((uint32_t)time < record || record == 0){
    inhibit_updates_true();

    sys_outb(RTC_ADDR_REG, port);
    sys_outb(RTC_DATA_REG, time);

    inhibit_updates_false();
  }
}

uint32_t get_record1(){
  wait_valid_rtc();

  uint32_t record = 0;

  sys_outb(RTC_ADDR_REG, RECORD);
  sys_inb(RTC_DATA_REG, &record);

  return record;
}

uint32_t get_record2(){
  wait_valid_rtc();

  uint32_t record = 0;

  sys_outb(RTC_ADDR_REG, RECORD2);
  sys_inb(RTC_DATA_REG, &record);

  return record;
}
uint32_t get_record3(){
  wait_valid_rtc();

  uint32_t record = 0;

  sys_outb(RTC_ADDR_REG, RECORD3);
  sys_inb(RTC_DATA_REG, &record);

  return record;
}

uint32_t get_hour(){
  wait_valid_rtc();

  uint32_t hour;

  sys_outb(RTC_ADDR_REG, HOURS);
  sys_inb(RTC_DATA_REG, &hour);

  return hour;
}

void set_hour(uint32_t hour){

  inhibit_updates_true();

  sys_outb(RTC_ADDR_REG, HOURS);
  sys_outb(RTC_DATA_REG, hour);

  inhibit_updates_false();
}

uint32_t get_minutes(){
  wait_valid_rtc();

  uint32_t minutes;

  sys_outb(RTC_ADDR_REG, MINUTES);
  sys_inb(RTC_DATA_REG, &minutes);

  return minutes;
}

void set_minutes(uint32_t minutes){

  inhibit_updates_true();

  sys_outb(RTC_ADDR_REG, MINUTES);
  sys_outb(RTC_DATA_REG, minutes);

  inhibit_updates_false();
}

uint32_t get_seconds(){
  wait_valid_rtc();

  uint32_t seconds;

  sys_outb(RTC_ADDR_REG, SECONDS);
  sys_inb(RTC_DATA_REG, &seconds);

  return seconds;
}

void draw_background_color(){
  if (relogio[0] > 19 || relogio[0] < 5) vg_draw_rectangle(0, 0, 1152, 864, 0);
  else if (relogio[0] == 5 || relogio[0] == 19) vg_draw_rectangle(0,0,1152,864,0x0e3e52);
  else if (relogio[0] == 6 || relogio[0] == 18) vg_draw_rectangle(0,0,1152,864,0x166484);
  else if (relogio[0] == 7 || relogio[0] == 17) vg_draw_rectangle(0,0,1152,864,0x1c7ea6);
  else if (relogio[0] == 8 || relogio[0] == 16) vg_draw_rectangle(0,0,1152,864,0x24a4d8);
  else if (relogio[0] == 9 || relogio[0] == 15) vg_draw_rectangle(0,0,1152,864,0x33acdd);
  else if (relogio[0] == 10 || relogio[0] == 14) vg_draw_rectangle(0,0,1152,864,0x44b3e0);
  else if (relogio[0] == 11 || relogio[0] == 12 || relogio[0] == 13)
    vg_draw_rectangle(0,0,1152,864,0x55b9e3);
}
