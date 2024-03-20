#include <lcom/lcf.h>
#include <lcom/lab4.h>
#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "mouse.h"
#include "kbc.h"

int hook_id_mouse = 2;

int mouse_subscribe_int(uint8_t *bit_no){

  *bit_no = hook_id_mouse;

  if(sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE_EXCLUSIVE , &hook_id_mouse) != OK){
    printf("Error in sys_irqsetpolicy\n");
    return 1;
  }
  return 0;
}

int mouse_unsubscribe_int(){
  if(sys_irqrmpolicy(&hook_id_mouse) != OK){
    printf("Error in sys_irqrmpolicy\n");
    return 1;
  }
  return 0;
}

int (read_status_byte)(uint8_t *st){
  if(util_sys_inb(STATUS_BYTE, st) != OK){
    printf("Error in sys_inb\n");
    return 1;
  }
  return 0;
}