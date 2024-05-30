

#ifndef RTC_H
#define RTC_H

#include <stdint.h>
#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "I_RTC.h"


int (rtc_subscribe_int) (uint8_t *bit_no);

int (rtc_unsubscribe_int) ();

int (read_output_buffer) (uint32_t reg, uint8_t* val);

int (write_input_buffer) (uint32_t reg, uint32_t val);

int (rtc_ih) ();

int (rtc_read_conf) ();

void (convert_bcd_to_decimal) (uint8_t* val);

void (convert_decimal_to_bcd) (uint8_t* val);

int (rtc_get_current_date) (current_date* date);

void (print_current_date) (current_date* date);

void (print_current_time) (current_date* date);

void (adjust_date_from_bcd_to_decimal) (current_date* date);



#endif /* RTC_H */