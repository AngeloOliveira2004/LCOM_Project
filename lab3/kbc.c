#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "Keyboard.h"
 

int (write_command) (uint8_t command){
  uint8_t st;
  int cnt = 0;
  while (1) {
    read_status_reg(&st);
    if(cnt == 5){
      return 1;
    }
    if ((st & IBF)) {
      cnt++;
      tickdelay(micros_to_ticks(DELAY_US));
    }
    else {
      sys_outb(KBC_COMMANDS, command);
      break;
    }
  }
  return 0;
}


int(write_command_argumments)(uint8_t *command){
  uint8_t st;
  int cnt = 0;
  while (1) {
    read_status_reg(&st);
    if(cnt == 5){
      return 1;
    }
    if ((st & IBF))
    {
      cnt++;
      tickdelay(micros_to_ticks(DELAY_US));
    }else{
      sys_outb(KBC_COMMANDS_ARGUMENTS, *command);
      break;
    }
  }
  return 0;
}
  

int(write_command_return)(uint8_t *command){
  uint8_t st;
  int cnt = 0;
  while (1) {
    read_status_reg(&st);
    if(cnt == 5){
      return 1;
    }
    if (st & OBF)
    {
      util_sys_inb(OUTPUT_BUFFER, command);
      if (st & ERROR_KEYB_KBC)
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
