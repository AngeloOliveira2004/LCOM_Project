#include "rtc.h"

int __hook_id_rtc = 8;

int (rtc_subscribe_int)(uint8_t *bit_no){
	*bit_no = __hook_id_rtc;
	if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &__hook_id_rtc) != 0){
		printf("Error subscribing rtc\n");
		return -1;
	}
	return 0;
}

int (rtc_unsubscribe_int)() {
	if (sys_irqrmpolicy(&__hook_id_rtc) != 0){
		printf("Error unsubscribing rtc\n");
		return -1;
	}
	return 0;
}

int (read_register) (uint32_t reg, uint8_t* val){
	
	if (sys_outb(INP_REG, reg) != 0) 
		return -1;
	
	if (util_sys_inb(OUT_REG, val) != 0) 
		return -1;

	return 0;
}

int (update_in_progress) (){
	uint8_t st;

	if (read_register(UPDATE_REG, &st) != 0) 
		return -1;

	if(st & UPDATE_IN_PROGRESS)
		return 1;

	return 0;
}

void (convert_bcd_to_decimal) (uint8_t* value){
	uint8_t decimal = (*value & 0xF0) >> 4;
	uint8_t unity = *value & 0x0F;
	*value = decimal * 10 + unity;
}

void (convert_decimal_to_bcd) (uint8_t* value){
	uint8_t first_hex = (*value / 10) << 4;
	uint8_t last_hex = *value % 10;
	*value = first_hex | last_hex;
}

void (adjust_date_values) (current_date* date){
	convert_bcd_to_decimal(&date->seconds);
	convert_bcd_to_decimal(&date->minutes);
	convert_bcd_to_decimal(&date->hours);
	convert_bcd_to_decimal(&date->day);
	convert_bcd_to_decimal(&date->day_week);
	convert_bcd_to_decimal(&date->month);
	convert_bcd_to_decimal(&date->year);
}

int (rtc_get_current_date) (current_date* date){
	
	while (true){
		
		int ret = update_in_progress();

		if(ret == -1){
			return -1;
		}
		else if(ret == 1){
			break;
		}
	}

	if (read_register(SEC, &date->seconds) != 0) return -1;

	if (read_register(MINUTES, &date->minutes) != 0) return -1;

	if (read_register(HOURS, &date->hours) != 0) return -1;

	if (read_register(DAY_OF_MONTH, &date->day) != 0) return -1;

	if (read_register(DAY_OF_WEEK, &date->day_week) != 0) return -1;

	if (read_register(MONTH, &date->month) != 0) return -1;

	if (read_register(YEAR, &date->year) != 0) return -1;

	adjust_date_values(date);
	return 0;
}

void (print_date) (current_date* date) {
    printf("%04d-%02d-%02d", 2000 + date->year, date->month, date->day);
}

void (print_time) (current_date* date) {
    printf("%02d:%02d:%02d", date->hours, date->minutes, date->seconds);
}
