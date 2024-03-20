#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

int kbc_si(uint8_t *bit_no);
int kbc_ui();
int read_status_reg(uint8_t *st);
int read_out_buf(uint8_t *out);

#endif
