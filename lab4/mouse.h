#ifndef MOUSE_H
#define MOUSE_H

#include <stdbool.h>
#include <stdint.h>
#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include "i8042.h"



int mouse_subscribe_int(uint8_t *bit_no);
int mouse_unsubscribe_int();

#endif