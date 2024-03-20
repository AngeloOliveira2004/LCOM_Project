#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "utils.c"
#include "i8042.h"

int hook_id = 2;


void (mouse_ih)() {

}

int(mouse_subscribe_int)(uint8_t *bit_no){

  *bit_no = hook_id;

  if(sys_irqsetpolicy(IRQ_MOUSE,IRQ_COMMAND_BYTE,&hook_id) != 0){
    return 1;
  }
  return 0;
}

int(mouse_unsubscribe_int)(){
  if(sys_irqrmpolicy(&hook_id) != 0){
    return 1;
  }
  return 0;
}


int (read_status_register)(uint8_t *st){
  if(util_sys_inb(STATUS_BYTE,st)){
    return 1;
  }
  return 0;
}

void (mouse_ih)() {}

