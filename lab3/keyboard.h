#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

int keyboard_subscribe_int(uint8_t *bit_no);
int keyboard_unsubscribe_int();
int read_status_register(uint8_t *st);
int read_out_buffer(uint8_t *out);

#endif /* KEYBOARD_H */
