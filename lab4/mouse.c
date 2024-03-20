#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "mouse.h"
#include "kbc.h"

int hook_id_mouse = 1;

int mouse_subscribe_int(uint8_t *bit_no){
  *bit_no = MOUSE_IRQ;
  if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse) != OK){
    printf("Error in sys_irqsetpolicy\n");
    return 1;
  }
  return 0;
}

int mouse_unsubscribe_int();