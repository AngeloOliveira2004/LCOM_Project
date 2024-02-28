#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  return 0;
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
  if (st == NULL || timer < 0 || timer > 2)
  {
    return 1;
  }
  uint8_t val = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if (sys_outb(TIMER_CTRL, val)) {
    return 1;
  }

  if(util_sys_inb(TIMER_0 + timer,st)){
    return 1;
  }

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  union timer_status_field_val uniao;

  switch (field)
  {
  case tsf_all:
    uniao.byte = st;
      break;
    case tsf_initial:
      uniao.in_mode = st >> 4;
      uniao.in_mode = uniao.in_mode & 0x3;
      break;
    case tsf_mode:
      uniao.count_mode = (st & TIMER_MASK_OP) >> 1;
      if (uniao.count_mode == 6 || uniao.count_mode == 7) {
        uniao.count_mode -= 4;
      } break;
      case tsf_base:
      uniao.bcd = st & TIMER_BCD;
      break;
    default:
      return 1;
  }
  timer_print_config(timer, field, uniao);

  return 0;
}


