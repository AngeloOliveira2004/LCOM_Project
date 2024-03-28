#ifndef KBC_H
#define KBC_H

#include <stdbool.h>
#include <stdint.h>
#include <lcom/lcf.h>
#include <lcom/lab4.h>
#include "i8042.h"

int kbc_write_command(uint8_t command);
int kbc_write_command_args(uint8_t* arguments);


int kbc_read_status(uint8_t *status);
int kbc_read_output(uint8_t *out , uint8_t mouse , uint8_t port);
int kbc_read_command(uint8_t *command);
  
#endif
