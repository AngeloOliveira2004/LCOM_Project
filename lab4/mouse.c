#include <lcom/lcf.h>

#include <stdint.h>
#include "i8042.h"
#include "Keyboard.h"
#include "mouse.h"
#include "kbc.c"

int hook_id_mouse = 3;
uint8_t packet;
uint8_t data_receiver;
extern uint8_t code;

int (mouse_subscribe_int) (uint8_t *bit_no) {
  *bit_no = hook_id_mouse;
  if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_EXCLUSIVE_SET, &hook_id_mouse)){
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
  if (util_sys_inb(READ_STATUS_BYTE, st))
  {
    return 1;
  }
  return 0;
}

int (read_out_buf) (uint8_t *out){
  if (out == NULL)
  {
    return 1;
  }

  if (util_sys_inb(READ_OUT_BUF, out))
  {
    return 1;
  }
  return 0;
}

int check_error(uint8_t st){
  if ((st & TIMEOUT) != 0 || (st & PARITY) != 0) {
    return 1;
  }
  return 0;
}

void (mouse_ih) (){
  uint8_t st;
  read_status_reg(&st);
  if (st & OBF)
  {
    if (((st & PARITY) != 0) || ((st & TIMEOUT) != 0))
    {
      code = 1;
    }
    else{
      code = 0;
    }
    read_out_buf(&data_receiver);
  }
  else{
    code = 1;
  }
}

void (packet__) (struct packet *pp)
{
  int y_msb, x_msb;
  pp->lb = pp->bytes[0] & LEFT_BUTTON;
  pp->rb = (pp->bytes[0] & RIGHT_BUTTON) >> 1;
  pp->mb = (pp->bytes[0] & MIDDLE_BUTTON) >> 2;
  y_msb = (pp->bytes[0] & MSB_Y_DELTA) >> 5;
  x_msb = (pp->bytes[0] & MSB_X_DELTA) >> 4;
  pp->x_ov = (pp->bytes[0] & X_OVL) >> 6;
  pp->y_ov = (pp->bytes[0] & Y_OVL) >> 7;

  
  
  pp->delta_x = (uint16_t)pp->bytes[1];
  
 
  
  pp->delta_y = (uint16_t)pp->bytes[2];
  if (x_msb == 1)
  {
    pp->delta_x = pp->delta_x | 0xff00;
  }
  if (y_msb == 1)
  {
    pp->delta_y = pp->delta_y | 0xff00;
  }
}


int (mouse_report) (uint8_t command){
  uint8_t st, type = 0;

  while (1)
  {
    if (read_status_reg(&st) != 0)
    {
      return 1;
    }
    if (((st & AUX) == 0) && ((st & IBF) == 0))
    {
      if (sys_outb(READ_STATUS_BYTE, WRITE_BYTE_MOUSE) != 0)
      {
        return 1;
      }
    }

    if (read_status_reg(&st) != 0)
    {
      return 1;
    }
    if (((st & AUX) == 0) && ((st & IBF) == 0))
    {
      if (sys_outb(WRITE_COMMAND, command) != 0)
      {
        return 1;
      }
    }
    if (util_sys_inb(WRITE_COMMAND, &type) != 0)return 1;
    
      return 1;
      if (type == ACK)
      {
        return 0;
      }
      if (type == NACK)
      {
        return 1;
      }
      if (type == ERROR)
      {
        return 1;
      }
    
  }
  return 0;
}
