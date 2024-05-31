

#ifndef RTC_H
#define RTC_H

#include <stdint.h>
#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "I_RTC.h"

/**
 * @brief Struct to represent date and time.
 */
typedef struct{
    uint8_t seconds;    
    uint8_t minutes;    
    uint8_t hours;      
    uint8_t day;        
    uint8_t month;      
    uint8_t year;           
    uint8_t day_week;    
} current_date;


int (rtc_subscribe_int) (uint8_t *bit_no);


int (rtc_unsubscribe_int) ();

int (read_register) (uint32_t reg, uint8_t* st);

void (convert_bcd_to_decimal) (uint8_t* value);

void (convert_decimal_to_bcd) (uint8_t* value);

int (rtc_get_current_date) (current_date* dt);

void (print_date) (current_date* dt);

void (print_time) (current_date* dt);

void (adjust_date_values) (current_date* dt);


#endif /* RTC_H */


