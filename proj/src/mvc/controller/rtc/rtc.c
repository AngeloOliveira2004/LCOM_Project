#include "rtc.h"

int __hook_id_rtc = 8;

int (rtc_subscribe_int)(uint8_t *bit_no){
	*bit_no = __hook_id_rtc;
	if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &__hook_id_rtc) != 0){
		printf("Error subscribing rtc.\n");
		return -1;
	}
	return 0;
}

int (rtc_unsubscribe_int)() {
	if (sys_irqrmpolicy(&__hook_id_rtc) != 0){
		printf("Error unsubscribing rtc.\n");
		return -1;
	}
	return 0;
}

int (read_register) (uint32_t reg, uint8_t* value){
	
	if (sys_outb(INP_REG, reg) != 0) 
		return -1;
	
	if (util_sys_inb(OUT_REG, value) != 0) 
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

void (convert_bcd_decimal) (uint8_t* value){
	uint8_t decimal = (*value & 0xF0) >> 4;
	uint8_t unity = *value & 0x0F;
	*value = decimal * 10 + unity;
}

void (convert_decimal_bcd) (uint8_t* value){
	uint8_t first_hex = (*value / 10) << 4;
	uint8_t last_hex = *value % 10;
	*value = first_hex | last_hex;
}

void (adjust_dt_values) (date_time* dt){
	convert_bcd_decimal(&dt->seconds);
	convert_bcd_decimal(&dt->minutes);
	convert_bcd_decimal(&dt->hours);
	convert_bcd_decimal(&dt->day);
	convert_bcd_decimal(&dt->day_week);
	convert_bcd_decimal(&dt->month);
	convert_bcd_decimal(&dt->year);
}

int (rtc_get_datetime) (date_time* dt){
	
	while (true){
		
		int ret = update_in_progress();

		if(ret == -1){
			return -1;
		}
		else if(ret == 1){
			break;
		}
	}

	// Seconds
	if (read_register(SEC, &dt->seconds) != 0) return -1;

	// Minutes
	if (read_register(MINUTES, &dt->minutes) != 0) return -1;

	// Hours
	if (read_register(HOURS, &dt->hours) != 0) return -1;

	// Day of the month
	if (read_register(DAY_OF_MONTH, &dt->day) != 0) return -1;

	// Day of the Week
	if (read_register(DAY_OF_WEEK, &dt->day_week) != 0) return -1;

	// Month
	if (read_register(MONTH, &dt->month) != 0) return -1;

	// Year
	if (read_register(YEAR, &dt->year) != 0) return -1;
	adjust_dt_values(dt);
	return 0;
}

void (show_date) (date_time* dt) {
    printf("%04d-%02d-%02d", 2000 + dt->year, dt->month, dt->day);
}

void (show_time) (date_time* dt) {
    printf("%02d:%02d:%02d", dt->hours, dt->minutes, dt->seconds);
}
