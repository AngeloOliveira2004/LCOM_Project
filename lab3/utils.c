#include <lcom/lcf.h>
#include <stdint.h>


uint32_t cnt;
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

