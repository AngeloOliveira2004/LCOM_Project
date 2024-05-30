#ifndef _I_RTC_H_
#define _I_RTC_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdint.h>

typedef struct{
    uint8_t seconds;    
    uint8_t minutes;    
    uint8_t hours;         
    uint8_t day;   
    uint8_t day_week;      
    uint8_t month;         
    uint8_t year;            
} current_date;

#define RTC_IRQ 8
#define RTC_MASK 5

#define RTC_INP_REG 0x70
#define RTC_OUT_REG 0x71

#define BIN BIT(2)
#define HOUR_MODE BIT(1)
#define UPDATE BIT(7)

#define SEC 0x00
#define MIN 0x02
#define HOUR 0x04
#define DAY 0x07
#define MONTH 0x08
#define YEAR 0x09

#endif