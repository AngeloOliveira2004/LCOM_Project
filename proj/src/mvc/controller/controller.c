#include "controller.h"
#include "keyboard/keyboard.h"

extern uint8_t scancode;
extern struct scancode_info scan_info;


void controller_loop(struct Game * game, struct Menu * menu){
  
}

void game_loop(struct Game * game){
  if(is_check(game)){
    if(is_checkmate(game)){
      game->state = CHECKMATE;
    }
  }
  else if(is_stalemate(game)){
    game->state = STALEMATE;
  }
  else if(is_draw(game)){
    game->state = DRAW;
  }
}


void parse_keyboard_input(){
  switch (scancode)
  {
  case ESC_BREAK:
    break;
  case TWO_BYTE_SCANCODE:
    scan_info.two_byte = true;
    break;
  case BREAK_CODE:
    scan_info.break_code = true;
    break;
  case MAKE_CODE:
    scan_info.make_code = true;
    break;
  case KEY_UP:
    scan_info.key_up = true;
    break;
  case KEY_DOWN:
    scan_info.key_down = true;
    break;
  case KEY_LEFT:
    scan_info.key_left = true;
    break;
  case KEY_RIGHT:
    scan_info.key_right = true;
    break;
  case KEY_SPACE:
    scan_info.key_space = true;
    break;
  case KEY_ENTER:
    scan_info.key_enter = true;
    break;
  case KEY_ESC:
    scan_info.key_esc = true;
    break;
  case KEY_A:
    scan_info.key_a = true;
    break;
  case KEY_B:
    scan_info.key_b = true;
    break;
  case KEY_C:
    scan_info.key_c = true;
    break;
  case KEY_D:
    scan_info.key_d = true;
    break;
  case KEY_E:
    scan_info.key_e = true;
    break;
  case KEY_F:
    scan_info.key_f = true;
    break;
  case KEY_G:
    scan_info.key_g = true;
    break;
  case KEY_H:
    scan_info.key_h = true;
    break;
  case KEY_I:
    scan_info.key_i = true;
    break;
  case KEY_J:
    scan_info.key_j = true;
    break;
  case KEY_K:
    scan_info.key_k = true;
    break;
  case KEY_L:
    scan_info.key_l = true;
    break;
  case KEY_M:
    scan_info.key_m = true;
    break;
  case KEY_N:
    scan_info.key_n = true;
    break;
  case KEY_O:
    scan_info.key_o = true;
    break;
  case KEY_P:
    scan_info.key_p = true;
    break;
  case KEY_Q:
    scan_info.key_q = true;
    break;
  case KEY_R:
    scan_info.key_r = true;
    break;
  case KEY_S:
    scan_info.key_s = true;
    break;
  case KEY_T:
    scan_info.key_t = true;
    break;
  }
}