#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "Keyboard.h"
 




int(read_command)(uint8_t *command){
  uint8_t st;
  int cnt = 0;
  while (1) {
    read_status_reg(&st);
    if(cnt == 5){
      return 1;
    }
    if (st & OBF)
    {
      util_sys_inb(READ_STATUS_BYTE, command);
      if (((st & ERROR_MOUSE_KBC) != 0) && ((st & AUX) != 0))
      {
        return 1;
      }
      break;
    }else{
      cnt++;
      tickdelay(micros_to_ticks(DELAY_US));
    }
  }
  return 0;
}

int(send_command)(uint8_t *command){
  uint8_t st;
  int cnt = 0;
  while (1) {
    read_status_reg(&st);
    if(cnt == 5){
      return 1;
    }
    if (st & OBF)
    {
      util_sys_inb(READ_STATUS_BYTE, command);
      if ((st & BIT(0))!= 0)
      {
        return 1;
      }
      break;
    }else{
      cnt++;
      tickdelay(micros_to_ticks(DELAY_US));
    }
  }
  return 0;
}
  
