#include "controller.h"
#include "keyboard/keyboard.h"

extern uint8_t scancode;
extern struct scancode_info scan_info;

enum ClickedKey key_pressed = NOKEY;
enum FlowState current_state = MENU;
extern int counter_mouse;

extern struct cursor cursor;
extern struct Position *button_position;

bool can_draw_this = true;
bool game_alredy_started = false;

struct Board tempBoard;


void init_game(struct Game *game,int minutes, int seconds) {
  // game->White_player = {};
  init_board(&game->board);
  init_player(&game->Black_player, false, minutes, seconds);
  init_player(&game->White_player, true, minutes, seconds);
  game->state = START;
  game->piece_count = 32;
  game->isWhiteTurn = true;

  index_ = 0;
}


void game_loop(struct Game *game) {

  bool boardChanged = false;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (tempBoard.squares[i][j].type != game->board.squares[i][j].type) {
                boardChanged = true;
                break;
            }
        }
        if (boardChanged) {
            break;
        }
    }

    if (boardChanged) {
        tempBoard = game->board;

        if (index_ < 1024) {
            boardArray[index_] = game->board;
            index_++;
            max_index = index_;
        } else {
            printf("Board array is full\n");
        }
    }
  int king_count = 0;

  for(int i = 0 ; i < 32 ; i++){
    if(game->board.pieces[i].type == KING){
      if(game->board.pieces[i].isWhite)
        king_count--;
      else
        king_count++;
    }
  }

  if(king_count == 1){
    current_state = WINNER_SCREEN;
    free(game);
    erase_buffer();
    draw_white_wins();
  }else if(king_count == -1){
    current_state = WINNER_SCREEN;
    free(game);
    erase_buffer();
    draw_black_wins();
  }

  if(game->White_player.clock.minutes == 0 && game->White_player.clock.seconds == 0 && game->White_player.clock.a_tenth_of_a_second == 0){
    current_state = MENU;
    game_alredy_started = false;
    free(game);
    erase_buffer();
    draw_menu(0,0);
  }

  if(game->Black_player.clock.minutes == 0 && game->Black_player.clock.seconds == 0 && game->Black_player.clock.a_tenth_of_a_second == 0){
    current_state = MENU;
    game_alredy_started = false;
    free(game);
    erase_buffer();
    draw_menu(0,0);
  }
}

void parse_keyboard_input() {

  kbc_ih();

  if (!receive_keyboard_scan(&scan_info, &scancode)) {
    key_pressed = ESC;
    return;
  }

  if (scan_info.size_counter == 1) {
    switch (scan_info.bytes[0]) {
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
      case _FIVE:
        key_pressed = FIVE;
        break;  
      case _SIX:
        key_pressed = SIX;
        break;  
      case ESC_BREAK_CODE:
        key_pressed = ESC;
        break;
      default:
        break;
    }
  }
  else {
    switch (scan_info.bytes[1]) {
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
        key_pressed = ENTER;
        break;
      default:
        break;
    }
  }

  printf("Key pressed: %d\n", key_pressed);
  printf("Menu state: %d\n", current_state);

  router();
}

void parse_mouse_input() {
  mouse_ih();

  if (counter_mouse == 3) {
    counter_mouse = 0;
    if(current_state == GAME){
      erase_buffer();

      swap_BackgroundBuffer();

      draw_board(&game->board);

      draw_clockValue(&game->Black_player, &game->White_player);



      draw_cursor_mouse(cursor.position.x, cursor.position.y , cursor.type);

      
      swap_buffers();
    }
  }
}

int minutes = 0;
int seconds = 0;

void router() {
  struct Board tempBoard ;
  switch (current_state) {
    case MENU:
      switch (key_pressed) {
        case ONE:
          current_state = NEW_GAME;
          draw_game_mode_menu();
          key_pressed = NOKEY;
          break;
        case TWO:
          /* code */
          break;
        case THREE:
          current_state = INSTRUCTIONS;
          draw_game_instructions();
          key_pressed = NOKEY;
          break;
        case FOUR:
          current_state = EXIT;
          key_pressed = ESC;
          break;
        default:
          break;
      }
    case NEW_GAME:
    
    switch (key_pressed) {
      case ONE:
        minutes = 1;
        seconds = 0;
        key_pressed = NOKEY;
        current_state = GAME;
        router();
        break;
      case TWO:
        minutes = 5;
        seconds = 0;
        key_pressed = NOKEY;
        current_state = GAME;
        router();
        break;  
      case THREE:
        minutes = 10;
        seconds = 0;
        key_pressed = NOKEY;
        current_state = GAME;
        router();
        break;  
      case FOUR:
        minutes = 30;
        seconds = 0;
        key_pressed = NOKEY;
        current_state = GAME;
        router();
        break; 
      case FIVE:
        minutes = 59;
        seconds = 58;
        key_pressed = NOKEY;
        current_state = GAME;
        router();
        break;
      case SIX:
        key_pressed = NOKEY;
        current_state = MENU;
        draw_menu(0,0);
        break;
      default:
        break;
      }

    case LOAD_GAME:
      break;
    case INSTRUCTIONS:
      switch (key_pressed){
      case ESC:
        current_state = MENU;
        key_pressed = NOKEY;

        draw_menu(0,0);

        break;
      case ONE:
        current_state = MENU;
        key_pressed = NOKEY;

        draw_menu(0,0);

        break;  
      default:
        break;
      }
      break;
    case GAME:

      switch (key_pressed)
      {
      case NOKEY:
        if(game_alredy_started == false){
          game = create_game();

          init_game(game, minutes,seconds);
          game_alredy_started = true;
        }
          tempBoard = game->board;
          erase_buffer();

          draw_backBackGround(&game->White_player, &game->Black_player);

          copy_BackGroundBuffer();

          draw_board(&game->board);

          swap_buffers();

          can_draw_this = true;
          break;
      case ARROW_LEFT:
        printf("arrow left\n");
        index_--;
        if(index_ <= 0){
          index_ = 0;
        }

        printf("index %d\n",index_);

        tempBoard = boardArray[index_];

        erase_buffer();

        swap_BackgroundBuffer();

        draw_board(&tempBoard);

        swap_buffers();

        can_draw_this = false;
        break;
      case ARROW_RIGHT:
        
        index_++;
        printf("index after increment  %d\n",index_);
        if((index_-1) >= max_index){
          index_ = max_index-1;
        }
         tempBoard = boardArray[index_];
         erase_buffer();

        swap_BackgroundBuffer();

        draw_board(&tempBoard);

        swap_buffers();
        can_draw_this = false;
        break;
      case ARROW_DOWN:
        
        index_ = 0;
         tempBoard = boardArray[index_];

         erase_buffer();

        swap_BackgroundBuffer();

        draw_board(&tempBoard);

        swap_buffers();
        can_draw_this = false;

        break;
      case ARROW_UP:
        key_pressed = NOKEY;
        index_ = max_index;
        can_draw_this = true;
        router();
        break;
      
      default:
        break;
      }
      

      break;
    case PAUSE_MENU:
      switch (key_pressed){
      case ONE:
        current_state = GAME;
        key_pressed = NOKEY;
        break;
      case TWO:
        current_state = MENU;
        key_pressed = NOKEY;

        draw_menu(0,0);

        break;  
      default:
        break;
      }
      break;
    break;
    case EXIT:

      printf("Exiting game...\n");

      current_state = MENU;

      erase_buffer();

      draw_menu(0,0);

      swap_buffers();

      router();
      break;
    case WINNER_SCREEN:
      switch (key_pressed){
      case ONE:
        current_state = MENU;

        erase_buffer();

        draw_menu(0,0);

        swap_buffers();

        key_pressed = NOKEY;

        break;
      default:
        break;
      }
      break;  
  }
}
// min == 0 , segundos passa para a esquerda e os tenth_of_a_second passam pra direita
void decrease_player_timer() {

  if (game->isWhiteTurn) {
    game->White_player.clock.a_tenth_of_a_second--;
    /*
    MIN , SEG , SEG/10
    30    00     00;
    29    59     10;
    00    59     9;
    */
   
    if (game->White_player.clock.a_tenth_of_a_second <= 0) {
      if (game->White_player.clock.seconds == 0 && game->White_player.clock.minutes == 0) {
        game->White_player.clock.a_tenth_of_a_second = 0;
      }
      else {
        game->White_player.clock.a_tenth_of_a_second = 9;
        if (game->White_player.clock.seconds == 0) {
          if (game->White_player.clock.minutes > 0) {
            game->White_player.clock.minutes--;
            game->White_player.clock.seconds = 59;
          }
          else {
            game->White_player.clock.seconds = 0;
          }
        }
        else {
          game->White_player.clock.seconds--;
        }
      }
    }
    
  }
  else {
    game->Black_player.clock.a_tenth_of_a_second--;
   
    /*
    MIN , SEG , SEG/10
    30    00     00;
    29    59     10;
    00    59     9;
    */
   
    if (game->Black_player.clock.a_tenth_of_a_second <= 0) {
      if (game->Black_player.clock.seconds == 0 && game->Black_player.clock.minutes == 0) {
          
        game->Black_player.clock.a_tenth_of_a_second = 0;
      }
      else {
        game->Black_player.clock.a_tenth_of_a_second = 9;
        if (game->Black_player.clock.seconds == 0) {
          if (game->Black_player.clock.minutes > 0) {
            game->Black_player.clock.minutes--;
            game->Black_player.clock.seconds = 59;
          }
          else {
            game->Black_player.clock.seconds = 0;
          }
        }
        else {
          game->Black_player.clock.seconds--;
        }
      }
    }
  }

  if(can_draw_this){

  erase_buffer();

  swap_BackgroundBuffer();

  draw_board(&game->board);

  draw_clockValue(&game->Black_player, &game->White_player);

  draw_cursor_mouse(cursor.position.x, cursor.position.y , cursor.type);

  swap_buffers();
  }
}

int (change_game_state_to_menu)(){
  current_state = PAUSE_MENU;
  erase_buffer();
  draw_pause_menu();
  return 0;
}
