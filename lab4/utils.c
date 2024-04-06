#include <lcom/lcf.h>
#include <stdint.h>


uint32_t cnt;

int (util_get_LSB) (uint16_t val, uint8_t *lsb){
  if (lsb == NULL)
  {
    return 1;
  }

  *lsb = val & 0x00ff;

  return 0;
}

int (util_get_MSB) (uint16_t val, uint8_t *msb){
  if (msb == NULL)
  {
    return 1;
  }
  *msb = (val >> 8);
  return 1;
}
int (util_sys_inb(int port, uint8_t *value)){
 
  if (value == NULL) {
    return 1;
  }
  uint32_t val;
  if (sys_inb(port, &val)) {
    return 1;
  }
  *value = (uint8_t) val;
  #ifdef LAB3
  cnt++;
  #endif

  return 0;
}

