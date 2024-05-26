#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "../kbc/i8042.h"

enum ClickedKey{
  NOKEY,
  UP,
  DOWN_ARROW,
  LEFT_ARROW,
  RIGHT_ARROW,
  ENTER_ARROW,
  ESC,
  ONE,
  TWO,
  THREE,
  FOUR,
};

struct scancode_info {
  uint8_t size_counter;
  bool make_code;
  uint8_t bytes[2];
};

int keyboard_subscribe_int(uint8_t *bit_no);
int keyboard_unsubscribe_int();
int read_status_register(uint8_t *st);
int read_out_buffer(uint8_t *out);
bool(receive_keyboard_scan)(struct scancode_info *scan_info, uint8_t *scancode);
void(clean_scan_info)(struct scancode_info *scan);
bool(test_status_polling)(uint8_t st);
int(kbc_enable_interrupts)();
int try_read_out_buffer(uint8_t *out);


int (check_ESC)();
int (break_from_cycles)();

#endif /* KEYBOARD_H */
