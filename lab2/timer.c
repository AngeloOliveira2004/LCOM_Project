#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  int port; // Has the port from which we will get the config
            // receives it's value from timer since it ranges from 0 to 2

  uint8_t rb_command = TIMER_RB_CMD; // BIT(6) | BIT(7) Para dizer que é Read-Back Command
  rb_command = rb_command | TIMER_RB_COUNT_; // BIT(5) Perguntar se como lê em 0, tenho de trocar   o valor.
  rb_command = rb_command | TIMER_RB_SEL(timer); //Perguntar o mesmo

  if(sys_outb(TIMER_CTRL,rb_command)){
    return 1;
  }
  
  switch (timer)
  {
  case 0:
    port = TIMER_0;
    break;

  case 1:
    port = TIMER_1;
    break;

  case 2:
    port = TIMER_2;
    break;

  default:
    return 1;
  }


  if(util_sys_inb(port,st)) //Calls util_sys_inb
    return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,enum timer_status_field field) {
  
  union timer_status_field_val val;

  switch (field)
  {
  case tsf_all: // status byte

    val.byte = st; // Select which timer to use , normally number 2

    break;

  case tsf_initial: // Timer initialization mode
      if(st & TIMER_LSB){

        val.in_mode = TIMER_LSB; // Timer set to LSB

      }else if(st & TIMER_MSB){

        val.in_mode = TIMER_MSB; // Timer set to MSB

      }else if(st & TIMER_LSB_MSB){

        val.in_mode = TIMER_LSB_MSB; // Timer set to LSB and MSB, in this order

      }else{
        val.in_mode = INVAL_val; // Invalid Initialization mode
      }

    break;

  case tsf_mode: // Counting mode
    if(st & TIMER_RATE_GEN){

      val.count_mode = 2; // Count Mode 2 (Rate generator) , probably not to be used

    }else if(st & TIMER_SQR_WAVE){

      val.count_mode = 3; // Count Mode 3 (Square Wave Generator)

    }
    break;

  case tsf_base: //Counting base, true if BCD

    if(st & TIMER_BIN){

      val.bcd = false; // False, is used for binary counting mode

    }else if(st & TIMER_BCD){

      val.bcd = true; // True, is used for BCD counting mode

    }
    break;
  
  default:

    return 1;

    break;
  }

  if(timer_print_config(timer,field,val)){
    return 1;
  }

  return 0;
}
