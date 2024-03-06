#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"

int hook_id = 1;
uint8_t scancode;

int (keyboard_subscribe_int)(uint8_t *bit_no) {

  *bit_no = hook_id;

  if(sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE_EXCLUSIVE ,&hook_id) != 0){
    return 1;
  }

  return 0;
}

int (keyboard_unsubscribe_int)() {
  
  if(sys_irqrmpolicy(&hook_id) != 0){
    return 1;
  }

  return 0;
}

void (keyboard_int_handler)() {
  uint8_t st; //Caso o bit 7 ou 6 esteja ativo descartar a informação do output buffer
  read_status_register(&st);
  if(st & OUT_BUFF_FULL){
    if(sys_inb(OUT_SCANCODES,scancode)){
      return 1;
    }
    read_out_buffer(scancode);

    if((scancode & KEYBOARD_STATUS_ERRORS) != 0){
      scancode = 0;
    }
  }
  tickdelay(DELAY_US);
}

int (read_status_register)(uint8_t *st){
  if(util_sys_inb(STATUS_BYTE,st)){
    return 1;
  }
  return 0;
}

int (read_out_buffer)(uint8_t *out){
  if(util_sys_inb(OUT_SCANCODES,out)){
    return 1;
  }
  return 0;
}


//to do:
// fazer make or break code func





