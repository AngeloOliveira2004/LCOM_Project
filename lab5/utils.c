#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include <lcom/utils.h>

uint32_t cnt;

int(util_get_MSB)(uint16_t val, uint8_t *msb){
  val >>= 8;
  *msb = (uint8_t) val;
  return 0;
}

int(util_get_LSB)(uint16_t val, uint8_t *lsb){
  *lsb = (uint8_t) val;
  return 0;
}

int(util_sys_inb)(int port,uint8_t *val){
  uint32_t value;

  if(sys_inb(port,&value) != 0){
    return 1;
  }

  *val = (uint8_t) value;

  #ifdef LAB3
  cnt++;
  #endif

  return 0;
}
