#ifndef __MOUSE_H
#define __MOUSE_H

#include <lcom/lcf.h>

#include <stdint.h>
#include "i8042.h"
#include "Keyboard.h"



int(mouse_subscribe_int)(uint8_t *bit_no);

int(mouse_unsubscribe_int)();

int(read_status_reg)(uint8_t *st);

int(read_out_buf)(uint8_t *out);

int check_error(uint8_t st);

void(mouse_ih)();

int(mouse_report)(uint8_t command);

void(packet__)(struct packet *pp);

int(write_command_mouse)(uint8_t command, int port);
#endif
