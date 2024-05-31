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

#define RTC_ADDR_REG 0x70

#define RTC_DATA_REG 0x71

#define RTC_IRQ 8

#define RTC_SECONDS            0
#define RTC_SECONDS_ALARM      1
#define RTC_MINUTES            2
#define RTC_MINUTES_ALARM      3
#define RTC_HOURS              4
#define RTC_HOURS_ALARM        5
#define RTC_DAY_OF_WEEK        6
#define RTC_DAY_OF_MONTH       7
#define RTC_MONTH              8
#define RTC_YEAR               9
#define RTC_REGISTER_A        10
#define RTC_REGISTER_B        11
#define RTC_REGISTER_C        12
#define RTC_REGISTER_D        13

#endif
