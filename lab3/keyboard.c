#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "Keyboard.h"
 

int hook_id = 2;
uint8_t scancode;

int (kbc_si) (uint8_t *bit_no){
  *bit_no = hook_id;
  if (sys_irqsetpolicy(KEYBOARD_IRQ , IRQ_EXCLUSIVE_SET, &hook_id))
  {
    return 1;
  }
  return 0;
}

int (kbc_ui) (){
  if(sys_irqrmpolicy(&hook_id)){
    return 1;
  }
  return 0;
}

int (read_status_reg) (uint8_t *st){
  if (util_sys_inb(READ_STATUS_BYTE, st))
  {
    return 1; 
  }
  return 0;
}
int (read_out_buf) (uint8_t *out){
  if (util_sys_inb(KBC_OUT_BUF, out))
  {
    return 1;
  }
  return 0;
}

void(kbc_ih) (){
  uint8_t st;
  read_status_reg(&st);
  if (st & OBF)
  {
    read_out_buf(&scancode);    
    if ((st &(ERROR_KEYB_KBC))!=0)
    {
      scancode = -1;
    }
  }
  //tickdelay(micros_to_ticks(DELAY_US));
}

bool(Check_oub_full) (uint8_t st){
  if ((st & AUX) == 0 && (st & OBF) != 0)
  {
    return true;
  }
  return false;
}
