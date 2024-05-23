#ifndef KBC_H
#define KBC_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"

int(kbc_write_command_args)(uint8_t *command_byte);
int(read_commands_kbc)(uint8_t *data);
int(send_commands_kbc)(uint8_t command,int port);
int(kbc_enable_interrupts)();



#endif
