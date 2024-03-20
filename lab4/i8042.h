#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define IRQ_COMMAND_BYTE (IRQ_REENABLE | IRQ_EXCLUSIVE)

#define IRQ_MOUSE 0x0C // IRQ 12

#define DELAY_US 20000 //Define o tempo de delay

#define STATUS_BYTE 0x64




#endif
