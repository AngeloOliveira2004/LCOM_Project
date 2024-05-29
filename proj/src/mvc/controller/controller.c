#include "controller.h"
#include "keyboard/keyboard.h"

extern uint8_t scancode;
extern struct scancode_info scan_info;

enum ClickedKey key_pressed = NOKEY;
enum FlowState current_state = MENU;
extern int counter_mouse;

extern struct cursor cursor;

void init_game(struct Game *game){
  //game->White_player = {};
  init_board(&game->board);
  game->state = START;
  game->piece_count = 32;
  game->Black_player.isWhite = false;
  game->Black_player.isWinner = false;
  game->Black_player.isDraw = false;
  game->Black_player.canLongCastle = true;
  game->Black_player.canShortCastle = true;
  game->White_player.isWhite = true;
  game->White_player.isWinner = false;
  game->White_player.isDraw = false;
  game->White_player.canLongCastle = true;
  game->White_player.canShortCastle = true;
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
    case _ONE:
      key_pressed = ONE;
      break;
    case _TWO:
      key_pressed = TWO;
      break;
    case _THREE:
      key_pressed = THREE;
      break;
    case _FOUR:
      key_pressed = FOUR;
      break;
    case ESC_BREAK_CODE:
      key_pressed = ESC;
      break;
    default:
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
    default:
      break;
    }
  }

  printf("Key pressed: %d\n",key_pressed);
  printf("Menu state: %d\n",current_state);

  router();
}

void parse_mouse_input(){
  mouse_ih();

  if (counter_mouse == 3) {
    counter_mouse = 0;
    draw_cursor(&cursor, &game->board);
  }

}

void router(){
  switch (current_state)
  {
  case MENU:
    switch (key_pressed)
    {
    case ONE:
      
      game = create_game();

      init_game(game);

      init_player(&game->Black_player, false, 1, 0);
      init_player(&game->White_player, true, 1, 0);
      

      erase_buffer();
      draw_backBackGround(&game->White_player,&game->Black_player);
      
      copy_BackGroundBuffer();

      draw_board(&game->board);
      
      swap_buffers();
      break;
    case TWO:
      /* code */
      break;
    case THREE:
      /* code */
      break;
    case FOUR:
      /* code */
      break;
    default:
      break;
    }
    break;
  case NEW_GAME:
    //update_game();
    break;
  case LOAD_GAME:
    break;
  case INSTRUCTIONS:
    break;
  case EXIT:
    break;
  }
}
//min == 0 , segundos passa para a esquerda e os tenth_of_a_second passam pra direita
void decrease_player_timer() {
  if (game->isWhiteTurn) {
      game->White_player.clock.a_tenth_of_a_second--;
      /*
      MIN , SEG , SEG/10
      30    00     00;
      29    59     10;
      00    59     9;
      */  
      if (game->White_player.clock.a_tenth_of_a_second < 0) {

          if(game->White_player.clock.seconds == 0 && game->White_player.clock.minutes == 0){
            game->White_player.clock.a_tenth_of_a_second = 0;
          }else{
            game->White_player.clock.a_tenth_of_a_second = 9;
            game->White_player.clock.seconds--;
          }

          if (game->White_player.clock.seconds < 0) {
              game->White_player.clock.seconds = 59;
              game->White_player.clock.minutes--;

              if (game->White_player.clock.minutes < 0) {
                  game->White_player.clock.minutes = 0;
                  game->White_player.clock.seconds = 0;
                  game->White_player.clock.a_tenth_of_a_second = 0;
              }
          }
      }
  } else {
      game->Black_player.clock.a_tenth_of_a_second--;

      if (game->Black_player.clock.a_tenth_of_a_second < 0) {
          game->Black_player.clock.a_tenth_of_a_second = 9;
          game->Black_player.clock.seconds--;

          if (game->Black_player.clock.seconds < 0) {
              game->Black_player.clock.seconds = 59;
              game->Black_player.clock.minutes--;

              if (game->Black_player.clock.minutes < 0) {
                  game->Black_player.clock.minutes = 0;
                  game->Black_player.clock.seconds = 0;
                  game->Black_player.clock.a_tenth_of_a_second = 0;
              }
          }
      }
  }


  erase_buffer();

  swap_BackgroundBuffer();

  draw_board(&game->board);

  draw_clockValue(&game->White_player , &game->Black_player);

  swap_buffers();
}
