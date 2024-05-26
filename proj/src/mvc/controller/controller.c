#include "controller.h"
#include "keyboard/keyboard.h"

extern uint8_t scancode;
extern struct scancode_info scan_info;

enum ClickedKey key_pressed = NOKEY;
extern int counter_mouse;

extern struct cursor cursor;
extern struct Board *board;


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
  
  kbc_ih(); 

  if(!receive_keyboard_scan(&scan_info,&scancode)){
    key_pressed = ESC;
    return;
  }

  if(scan_info.size_counter == 1){
    switch (scan_info.bytes[0])
    {
    case W:
      key_pressed = ARROW_UP;
      break;
    case S:
      key_pressed = ARROW_DOWN;
      break;
    case A:
      key_pressed = ARROW_LEFT;
      break;
    case D:
      key_pressed = ARROW_RIGHT;
      break;
    case ESC_BREAK_CODE:
      key_pressed = ESC;
      break;
    }
  }
  else{
    switch (scan_info.bytes[1])
    {
    case UP_ARROW:
      key_pressed = ARROW_UP;
      break;
    case DOWN_ARROW:
      key_pressed = ARROW_DOWN;
      break;
    case LEFT_ARROW:
      key_pressed = ARROW_LEFT;
      break;
    case RIGHT_ARROW:
      key_pressed = ARROW_RIGHT;
      break;
    case _ENTER_:
      key_pressed = _ENTER_;
      break;
    }
  }
}

void parse_mouse_input(){
  mouse_ih();

  if (counter_mouse == 3) {
    counter_mouse = 0;
    draw_cursor(&cursor, board);
  }

}
