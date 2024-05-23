#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "i8042.h"
#include "mouse.h"
#include "kbc.h"

int hook_id_mouse = 2;

struct packet mouse;

bool sync_v = false;

int counter = 0;

extern int counter_packet_print;

enum States current_state = INITIAL;

void (mouse_ih)() {
  uint8_t data;
  read_commands_kbc(&data);
  parse_values(data,&counter,&mouse);
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


int (read_status_register)(uint8_t *st){
  if(util_sys_inb(STATUS_BYTE,st)){
    return 1;
  }
  return 0;
}

int(reset_mouse_struct)(struct packet *mouse){

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
    *cnt = 0;
    mouse_print_packet(pp);
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

// int (mouse_gesture)(uint8_t x_len,uint8_t tolerance){

//   int16_t delta_x = mouse.delta_x; //Deslocamento do rato no eixo dos x
//   int16_t delta_y = mouse.delta_y; //Deslocamento do rato no eixo dos y

//   switch(current_state){
//     case INITIAL:
//       //Verificar que apenas o botão esquerdo está ativado
//       if(mouse.lb == BUTTON_PRESSED && mouse.rb != BUTTON_PRESSED && mouse.mb != BUTTON_PRESSED){
//         current_state = DRAWING_FIRST_LINE;
//       }

//       break;

//     case DRAWING_FIRST_LINE:
//       if(mouse.lb != BUTTON_PRESSED){ //Condição que verifica que o botão esquerdo foi largado
//         if( (abs(delta_x) <= tolerance && abs(delta_y) <=(tolerance)) &&                                                                            (total_y_movement/total_x_movement > 1) &&                                       total_x_movement >= x_len){
//           current_state = DRAWING_VERTEX_ZONE; //Se o deslocamento de x for não nulo, o slope for maior do que 1 e o movimento de x for maior que o valor dado pelo x_len trocar de estado
//         }else{
//           current_state = INITIAL;
//         }
//         total_x_movement = 0;
//         total_y_movement = 0;
//       }else if(mouse.mb == BUTTON_PRESSED || mouse.rb == BUTTON_PRESSED){
//         current_state = INITIAL; // Outro botão foi clicado durante a criação da linha
//         total_x_movement = 0;
//         total_y_movement = 0;
//       }else if((delta_x < 0 || delta_y < 0) && (abs(delta_x) > tolerance || abs(delta_y) > tolerance)){
//         current_state = INITIAL; //Caso o deslocamento seja feito para a esquerda e/ou o valor ultrapasse o valor da tolerância, retorna ao início
//         total_x_movement = 0;
//         total_y_movement = 0;
//       }else{ //Caso passe as condições, o movimento está dentro das margens de tolerância e poderá prosseguir
//         total_x_movement += delta_x;
//         total_y_movement += delta_y;
//       }

//       break;

//     case DRAWING_VERTEX_ZONE:
//       if((abs(delta_x) > tolerance || abs(delta_y) > tolerance)){
//         current_state = INITIAL;
//       }else if(mouse.lb != BUTTON_PRESSED && mouse.rb != BUTTON_PRESSED && mouse.mb != BUTTON_PRESSED){
//         total_x_movement += delta_x;
//         total_y_movement += delta_y;
//       }else if(mouse.lb != BUTTON_PRESSED && mouse.rb == BUTTON_PRESSED && mouse.mb != BUTTON_PRESSED){
//         if(delta_x <= tolerance){
//           current_state = DRAWING_SECOND_LINE;
//         }else{
//           current_state = INITIAL;
//         }
//         total_x_movement = 0;
//         total_y_movement = 0;
//       }
//       break;

//     case DRAWING_SECOND_LINE:

//       if(mouse.rb != BUTTON_PRESSED){ //Condição que verifica que o botão esquerdo foi largado
//         if((abs(delta_x) <= tolerance && abs(delta_y) <=(tolerance)) &&                                                                           (abs(total_y_movement/total_x_movement) > 1) &&                                       total_x_movement >= x_len){
//           current_state = COMPLETE; //Se o deslocamento de x for não nulo, o absoluto da slope for maior do que 1 e o movimento de x for maior que o valor dado pelo x_len trocar de estado
//           return 0;
//         }else{
//           current_state = INITIAL;
//         }
//         total_x_movement = 0;
//         total_y_movement = 0;
//       }else if(mouse.lb == BUTTON_PRESSED || mouse.mb == BUTTON_PRESSED){
//         current_state = INITIAL;
//         total_x_movement = 0;
//         total_y_movement = 0;
//       }else if((delta_x < 0 || delta_y > 0) && (abs(delta_x) > tolerance || abs(delta_y) > tolerance)){
//         current_state = INITIAL;
//         total_x_movement = 0;
//         total_y_movement = 0;
//       }else{ //Caso passe as condições, o movimento está dentro das margens de tolerância e poderá prosseguir
//         total_x_movement += delta_x;
//         total_y_movement += delta_y;
//       }
//       break;
//     case COMPLETE:
//       current_state = COMPLETE;
//       break;    
//   }

//   return 0;
// }


int (in_game_mouse_movement)(){
  switch(current_state){
    case INITIAL:
      if(mouse.lb == BUTTON_PRESSED && mouse.rb != BUTTON_PRESSED && mouse.mb != BUTTON_PRESSED){
        current_state = PIECE_SELECTED;
      }
      break;
    case PIECE_SELECTED:
      if(mouse.lb != BUTTON_PRESSED){
        current_state = PIECE_CLICKED;
      } else {
        current_state = PIECE_DRAGGED;
      }
      break;
    case PIECE_CLICKED:
      if(mouse.lb == BUTTON_PRESSED){
        current_state = COMPLETE;
      }
      break;
    case PIECE_DRAGGED:
      if(mouse.lb != BUTTON_PRESSED){
        current_state = COMPLETE;
      }
      break;  
    case COMPLETE:
      
      break;
  }
  return 0;
}

