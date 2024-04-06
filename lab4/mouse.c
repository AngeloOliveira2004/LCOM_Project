#include <lcom/lcf.h>

#include <stdint.h>
#include "i8042.h"
#include "mouse.h"

int hook_id_mouse = 3;

int (mouse_subscribe_int) (uint8_t *bit_no) {
  *bit_no = hook_id_mouse;
  if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_EXCLUSIVE_SET, hook_id_mouse)){
    return 1;
  }
  return 0;
}

int (mouse_unsubscribe_int) (){
  if (sys_irqrmpolicy(&hook_id_mouse))
  {
    return 1;
  }

  return 0;
}

int (read_status_reg) (uint8_t *st){
  if (st == NULL)
  {
    return 1;
  }

  if (util_sys_inb(READ_STATUS_BYTE, &st))
  {
    return 1;
  }
}
int (read_out_buf) (uint8_t *out){
  if (out == NULL)
  {
    return 1;
  }

  if (util_sys_inb(READ_OUT_BUF, out))
  {
    return 1;
    0-
  }
}

void (mouse_ih) (){

} 