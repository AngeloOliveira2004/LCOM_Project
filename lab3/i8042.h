#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_
#include <minix/sysutil.h>
#include <lcom/lcf.h>

#define IRQ_EXCLUSIVE_SET (IRQ_REENABLE|IRQ_EXCLUSIVE) 
#define ESC 0x81
#define READ_STATUS_BYTE 0X64
#define KBC_COMMANDS 0X64
#define KBC_COMMANDS_ARGUMENTS 0X60
#define KBC_COMMAND_RETURN 0X60
#define INPUT_BUFFER 0X60
#define OUTPUT_BUFFER 0X64
#define KBC_OUT_BUF 0x60
#define DELAY_US    20000
#define PARITY BIT(7)
#define TIMEOUT BIT(6)
#define AUX BIT(5)
#define INH BIT(4)
#define IBF BIT(1)
#define OBF BIT(0)
#define ERROR_KEYB_KBC (BIT(6)|BIT(7))
#define READ_COMMAND 0x20
#define WRITE_COMMAND 0x60  
#endif
