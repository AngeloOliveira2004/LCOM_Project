#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "../kbc/i8042.h"
#include "mouse.h"
#include "../../model/board.h"
#include "../../view/view.h"
#include "../../proj/src/sprites/Cursor/cursors.xpm"

int hook_id_mouse = 2;

struct packet mouse;

struct cursor cursor;

bool sync_v = false;

int counter_mouse = 0;

extern int counter_packet_print;

enum InGameStates current_state;

void (mouse_ih)() {
  uint8_t data;
  read_commands_kbc(&data);
  parse_values(data,&counter_mouse,&mouse);
}

int(mouse_subscribe_int)(uint8_t *bit_no){

  *bit_no = hook_id_mouse;

  if(sys_irqsetpolicy(IRQ_MOUSE,IRQ_COMMAND_BYTE,&hook_id_mouse) != 0){
    return 1;
  }
  return 0;
}

int(mouse_unsubscribe_int)(){
  if(sys_irqrmpolicy(&hook_id_mouse) != 0){
    return 1;
  }
  return 0;
}

int(reset_mouse_struct)(struct packet *mouse){

  return 0;
}

int(cursor_draw_start)(){
  cursor.position.x = 0;
  cursor.position.y = 0;
  cursor.type = DEFAULT;
  current_state = INITIAL;
  return 0;
  
}

int(parse_values)(uint8_t data,int *cnt,struct packet *pp){
  if(*cnt == 0){
    clean_packet(pp);
  }
  *cnt += 1;
  if(*cnt == 1){
    if((!sync_v) && ((data & SYNC) == 1)){
      sync_v = true;
      *cnt = 0;
      return 0;
    }
  }

  switch (*cnt){
  case 1:{
    pp->lb = (data & LB);
    pp->rb = ((data & RB) >> 1);
    pp->mb = ((data & MB) >> 2);
    pp->delta_x = ((data & MSB_X_DELTA) << 4);
    pp->delta_y = ((data & MSB_Y_DELTA) << 3);
    pp->x_ov = ((data & X_OVFL) >> 6);
    pp->y_ov = ((data & Y_OVFL) >> 7);

    break;
  }
  case 2:{
    pp->delta_x |= data;

    if((pp->delta_x >> 8) == 1){
      pp->delta_x |= 0xFF00;
    }else{
      pp->delta_x &= 0x00FF;
    }
    break;
  }

  case 3:{
    pp->delta_y |= data;

    if((pp->delta_y >> 8) == 1){
      pp->delta_y |= 0xFF00;
    }else{
      pp->delta_y &= 0x00FF;
    }

    break;
  }
  }
  counter_packet_print++;
  pp->bytes[*cnt-1] = data;

  if(*cnt == 3){
    cursor.position.x += pp->delta_x;
    cursor.position.y -= pp->delta_y;

    if(cursor.position.x >= WIDTH-64 && cursor.position.x <= WIDTH){
      cursor.position.x = 5;
    }

    if(cursor.position.y >= HEIGHT-64 && cursor.position.y <= HEIGHT){
      cursor.position.y = 5;
    }

    if(cursor.position.x >= WIDTH-64){
      cursor.position.x = WIDTH-69;
    }

    if(cursor.position.y >= HEIGHT-64){
      cursor.position.y = HEIGHT-69;
    }
    in_game_mouse_movement();
  }

  return 0;
}

void clean_packet(struct packet *mouse){
  memset(mouse->bytes,0,sizeof(*mouse->bytes));
  mouse->delta_x = 0x0000;
  mouse->delta_y = 0x0000;
  mouse->lb = 0;
  mouse->mb = 0;
  mouse->rb = 0;
  mouse->x_ov = 0;
  mouse->y_ov = 0;
}

int(disable_mouse_report)(){

  if(send_commands_kbc(WRITE_BYTE_TO_MOUSE,KBC_IN_CMD) != 0){
    return 1;
  }

  if(send_commands_kbc(DISABLE_DATA_REPORTING,KBC_OUT_CMD) != 0){
    return 1;
  }

  uint8_t error;
  read_commands_kbc(&error);
  switch (error){
  case ACK:
    return 0;
    break;
  case NACK:
    printf("Invalid byte due to serial communication error");
    return 1;
    break;  
  case ERROR:
    printf("Second consecutive invalid byte");
    return 1;
    break;  
  }
  return 0;
}

int(enable_mouse_report)(){

  if(send_commands_kbc(WRITE_BYTE_TO_MOUSE,KBC_IN_CMD) != 0){
    return 1;
  }

  if(send_commands_kbc(ENABLE_DATA_REPORTING,KBC_OUT_CMD) != 0){
    return 1;
  }

  uint8_t error;
  read_commands_kbc(&error);
  switch (error){
  case ACK:
    return 0;
    break;
  case NACK:
    printf("Invalid byte due to serial communication error");
    return 1;
    break;  
  case ERROR:
    printf("Second consecutive invalid byte");
    return 1;
    break;  
  }
  return 0;
}

int16_t total_x_movement = 0;
int16_t total_y_movement = 0;

int (in_game_mouse_movement)(){
  switch(current_state){
    case INITIAL:
      if(mouse.lb == BUTTON_PRESSED && mouse.rb != BUTTON_PRESSED && mouse.mb != BUTTON_PRESSED){
        current_state = PIECE_SELECTED;
        printf("INITIAL\n");
      }
      break;
    case PIECE_SELECTED:
      printf("PIECE_SELECTED\n");
      if(mouse.lb != BUTTON_PRESSED){
        current_state = PIECE_CLICKED;
        cursor.type = HOVERING;
      } else {
        current_state = PIECE_DRAGGED;
        cursor.type = SELECTED;
      }
      break;
    case PIECE_CLICKED:
      printf("PIECE_CLICKED\n");
      if(mouse.lb == BUTTON_PRESSED){
        current_state = COMPLETE;
      }else if(mouse.rb == BUTTON_PRESSED){
        current_state = INITIAL;
      }
      break;
    case PIECE_DRAGGED:
      printf("PIECE_DRAGGED\n");
      if(mouse.lb != BUTTON_PRESSED){
        current_state = COMPLETE;
      }
      break;  
    case COMPLETE:
      printf("COMPLETE\n");
      cursor.type = DEFAULT;
      current_state = INITIAL;
      break;
  }
  return 0;
}



