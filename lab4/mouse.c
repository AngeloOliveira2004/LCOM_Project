#include "mouse.h"


int hook_id_mouse = 2;
struct packet mouse_packet;
uint8_t mouse_byte; //byte to store the mouse packet
uint8_t byte_index = 0; //index of the byte in the packet
uint8_t current_byte; //current byte of the packet
uint8_t mouse_bytes[3]; //array to store the 3 bytes of the packet

int mouse_subscribe_int(uint8_t *bit_no){

  *bit_no = hook_id_mouse;

  if(sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE_EXCLUSIVE , &hook_id_mouse) != OK){
    printf("Error in sys_irqsetpolicy\n");
    return 1;
  }
  return 0;
}

int mouse_unsubscribe_int(){
  if(sys_irqrmpolicy(&hook_id_mouse) != OK){
    printf("Error in sys_irqrmpolicy\n");
    return 1;
  }
  return 0;
}

int (read_status_byte)(uint8_t *st){
  if(util_sys_inb(STATUS_BYTE, st) != OK){
    printf("Error in sys_inb\n");
    return 1;
  }
  return 0;
}

void mouse_ih(){
  if(kbc_read_output(&mouse_byte, 1, OUT_BUF) != OK){
    printf("Error in kbc_read_output\n");    
  }
}

void mouse_sync_bytes(){
  if(byte_index == 0 && (mouse_byte & BIT(3)) == 0){ //check if the first byte is correct. It is only correct if BIT(3) is 0S
    return;
  }
  mouse_bytes[byte_index] = mouse_byte;
  byte_index++;
}

void mouse_parse_packet(){
  mouse_packet.bytes[0] = mouse_bytes[0];
  mouse_packet.bytes[1] = mouse_bytes[1];
  mouse_packet.bytes[2] = mouse_bytes[2];

  mouse_packet.lb = mouse_bytes[0] & BIT(0);
  mouse_packet.mb = mouse_bytes[0] & BIT(2);
  mouse_packet.rb = mouse_bytes[0] & BIT(1);

  mouse_packet.delta_x = mouse_bytes[1];
  mouse_packet.delta_y = mouse_bytes[2];

  mouse_packet.x_ov = mouse_bytes[0] & BIT(6);
  mouse_packet.y_ov = mouse_bytes[0] & BIT(7);

  return;
}