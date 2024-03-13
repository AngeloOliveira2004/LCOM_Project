#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "keyboard.h"
#include "kbc.h"

int hook_id = 1;
uint8_t scancode;
struct scancode_info scan_info;

int (keyboard_subscribe_int)(uint8_t *bit_no) {

  *bit_no = hook_id;

  if(sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE_EXCLUSIVE ,&hook_id) != 0){
    return 1;
  }

  return 0;
}

int (keyboard_unsubscribe_int)() {
  
  if(sys_irqrmpolicy(&hook_id) != 0){
    return 1;
  }

  return 0;
}

int (read_status_register)(uint8_t *st){
  if(util_sys_inb(STATUS_BYTE,st)){
    return 1;
  }
  return 0;
}

int (read_out_buffer)(uint8_t *out){
  if(util_sys_inb(OUT_SCANCODES,out)){
    return 1;
  }
  return 0;
}

void (kbc_ih)() {
  uint8_t st; //Caso o bit 7 ou 6 esteja ativo descartar a informação do output buffer
  read_status_register(&st);
  if(st & OUT_BUFF_FULL){
      read_out_buffer(&scancode);
  }

  if((st & KEYBOARD_STATUS_ERRORS) != 0){
    scancode = -1;
  }

  //tickdelay(micros_to_ticks(DELAY_US));
}

bool(test_status_polling)(uint8_t st){
  if(((st & AUX_STATUS_REG) == 0) && ((st & OUT_BUFF_FULL) == 1)){
    return true;
  }
  return false;
}

void(clean_scan_info)(struct scancode_info *scan){
  memset(scan->bytes, 0, sizeof(scan->bytes));
  scan->size_counter = 0;
  scan->make_code = false;
}

bool(receive_keyboard_scan)(struct scancode_info *scan_info, uint8_t *scancode){
  bool valid = true;
  if(*scancode == ESC_BREAK_CODE){ //Caso específico para o ESC_BREAK_CODE, se for só o make code , ele não apresenta problemas, apenas fecha o programa quando recebe o break code
    valid = false; // Encerra o loop
    scan_info->make_code = false; //Coloca make_code a falso, para salientar que é break code
    scan_info->size_counter = 1;
    scan_info->bytes[0] = ESC_BREAK_CODE;
  }else{

    if(*scancode == 0xE0){ 
      scan_info->bytes[0] = *scancode;
      scan_info->size_counter++;
      read_out_buffer(scancode);
    }

    if((*scancode >> 7) & 1){ //Vê se o bit msb do scancode é break code (1)
     scan_info->make_code = false;
    }else{
      scan_info->make_code = true;
    }

      scan_info->bytes[scan_info->size_counter] = *scancode; //Adiciona ao index de byte 0 ou   1       dependendo do   número de bytes do scancode
      scan_info->size_counter++; //Incrementa o counter para ser passado para a print_scancode
  }

  kbd_print_scancode(scan_info->make_code,scan_info->size_counter,scan_info->bytes); // Dá print ao valor do scancode
  if(valid){
    return true;
  }else{
    return false;
  }
}

int(kbc_enable_interrupts)(){
  uint8_t command_byte;
  if(kbc_write_command(READ_COMMAND_BYTE) != 0){
    return 1;
  }

  if(read_out_buffer(&command_byte) != 0){
    return 1;
  }

  command_byte |= ENABLE_INTERRUPT_OBF_KEYBOARD;

  if(kbc_write_command(WRITE_COMMAND_BYTE) != 0){
    return 1;
  }

  if(kbc_write_command_args(&command_byte) != 0){
    return 1;
  }

  return 0;
}







