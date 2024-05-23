#include "lcom/lcf.h"
#include <stdbool.h>
#include <stdint.h>

#include "../kbc/i8042.h"
#include "keyboard.h"
#include "../kbc/kbc.h"

int hook_id = 1;
uint8_t scancode;
struct scancode_info scan_info;

int (keyboard_subscribe_int)(uint8_t *bit_no) {

  *bit_no = BIT(hook_id);

  if(sys_irqsetpolicy(IRQ_KEYBOARD , IRQ_REENABLE | IRQ_EXCLUSIVE , &hook_id) != OK)
    return 1;

  return 0;
}

int (keyboard_unsubscribe_int)() {

  if(sys_irqrmpolicy(&hook_id) != OK)
    return 1;

  return 0;
}

int (read_status_register)(uint8_t *st){
  
  if(util_sys_inb(STATUS_BYTE , st) != OK)
    return 1;

  return 0;
}

int (read_out_buffer)(uint8_t *out){


  if(util_sys_inb(OUT_SCANCODES , out) != OK)
    return 1;

  return 0;
}

void (kbc_ih)() {

  uint8_t st;

  read_status_register(&st);

  if((st & OUT_BUFF_FULL) == 1){
    read_out_buffer(&scancode);
  }

  if((st & KEYBOARD_STATUS_ERRORS) == 1){
    scancode = -1;
  }
}

bool(test_status_polling)(uint8_t st){
  if((st & AUX_STATUS_REG) == 0 && (st & OUT_BUFF_FULL)){
    return true;
  }

  return false;
}
/*
struct scancode_info {
  uint8_t size_counter;
  bool make_code;
  uint8_t bytes[2];
;*/
void(clean_scan_info)(struct scancode_info *scan){
  for(int i = 0 ; i < scan->size_counter ; i++){
    scan->bytes[i] = 0;
  }
  scan->size_counter = 0;
  scan->make_code = false;
}

bool(receive_keyboard_scan)(struct scancode_info *scan_info, uint8_t *scancode){
  bool is_E0 = false;
  bool valid = true;

  if(*scancode == ESC_BREAK_CODE){
    scan_info->make_code = false;
    scan_info->size_counter = 1;
    scan_info->bytes[0] = ESC_BREAK_CODE;
    valid = false;
  }else{
    if(*scancode == 0xE0){
      scan_info->bytes[0] = *scancode;
      scan_info->size_counter++;
      is_E0 = true;
    }else{
      if((*scancode >> 7) & 1){
        scan_info->make_code = false;
      }else{
        scan_info->make_code = true;
      }

      scan_info->bytes[scan_info->size_counter] = *scancode;
      scan_info->size_counter++;
    }
  }

  if(!is_E0){
    clean_scan_info(scan_info);
  }

  if(valid){
    return true;
  }else{
    return false;
  }

  printf("Scancode: 0x%02X\n",*scancode);
}

int(kbc_enable_interrupts)(){
  
  uint8_t command_byte;

  if(send_commands_kbc(READ_COMMAND_BYTE,KBC_CMD_REG) != OK){
    return 1;
  }

  if(try_read_out_buffer(&command_byte)){
    return 1;
  }

  command_byte |= ENABLE_INTERRUPT_OBF_KEYBOARD;

  if(send_commands_kbc(WRITE_COMMAND_BYTE,KBC_CMD_REG) != OK){
    return 1;
  }

  if(send_commands_kbc(command_byte,IN_KBC_COMMAND_ARGS) != OK){
    return 1;
  }

  return 0;
}

int try_read_out_buffer(uint8_t *out){
  uint8_t st , temp_out;

  while (1)
  {
    read_status_register(&st);

    if((st & OUT_BUFF_FULL)){
      read_out_buffer(&temp_out);
      if((st & KEYBOARD_STATUS_ERRORS)){
        *out = -1;
        return 1;
      }else{
        *out = temp_out;
        return 0;
      }
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
}


int (check_ESC)(){

  kbc_ih(); 

  if(!receive_keyboard_scan(&scan_info,&scancode)){
    printf("Pressed ESC\n");
    
    return 1;
  }


  return 0;
}



int (break_from_cycles)(){
  int ipc_status,r;
  message msg;
  bool valid = true;

  uint8_t irq_set;

  if(keyboard_subscribe_int(&irq_set) != 0){
    return 1;
  }

   
  if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
  }

  if (is_ipc_notify(ipc_status)) {
          switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:			
              if (msg.m_notify.interrupts & BIT(irq_set)) { 
                kbc_ih(); 

                if(!receive_keyboard_scan(&scan_info,&scancode)){
                  valid = false;
                }
              }
              break;
          }
  }

  if(keyboard_unsubscribe_int() != 0){
    return 1;
  }

  if(valid){
    return 0;
  }else{
    return 1;
  }
}


