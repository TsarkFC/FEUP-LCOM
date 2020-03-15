#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

void (mouse_ih)();
int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)();

void set_struct(uint8_t array[], struct packet *pp);

int mouse_command(uint8_t com);
int receive_command(uint8_t *command);
int send_command(uint8_t port, uint8_t command);

int disable_mouse();
int enable_mouse_kbc();
int mouse_poll(uint16_t period, uint8_t cnt);



enum STATE{Start,Line1,Vertex,Line2};
enum EVENT{WrongKey,LeftDown,LeftUp,RightDown,RightUp};

bool event_state_processing(struct packet *pp,uint8_t x_len,uint8_t tolerance);

