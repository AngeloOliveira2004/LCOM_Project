#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_
#include <minix/sysutil.h>
#include <lcom/lcf.h>

#define MOUSE_IRQ 12

#define IRQ_EXCLUSIVE_SET (IRQ_REENABLE|IRQ_EXCLUSIVE) 
#define ESC 0x81
#define READ_STATUS_BYTE 0X64
#define KBC_COMMANDS 0X64
#define KBC_COMMANDS_ARGUMENTS 0X60
#define KBC_COMMAND_RETURN 0X60
#define INPUT_BUFFER 0X60
#define OUTPUT_BUFFER 0X60
#define  READ_OUT_BUF 0x60
#define DELAY_US   20000
#define PARITY BIT(7)
#define TIMEOUT BIT(6)
#define AUX BIT(5)
#define INH BIT(4)
#define IBF BIT(1)
#define OBF BIT(0)
#define ERROR_KEYB_KBC (BIT(6)|BIT(7))
#define READ_COMMAND 0x20
#define WRITE_COMMAND 0x60  

#define WRITE_BYTE_MOUSE 0xD4

#define LEFT_BUTTON BIT(0)
#define RIGHT_BUTTON BIT(1)
#define MIDDLE_BUTTON BIT(2)
#define MSB_X_DELTA BIT(4)
#define MSB_Y_DELTA BIT(5)
#define X_OVL BIT(6)
#define Y_OVL BIT(7)

#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC

#define ERROR_MOUSE_KBC (BIT(6)|BIT(7))


#define DISABLE_MRP 0xF5
#define ENABLE_MRP 0xF4

#endif
