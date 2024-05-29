#include <lcom/lcf.h>
#include <stdio.h>

#include "../model/board.h"
#include "../model/game.h"
#include "../model/menu.h"
#include "../view/view.h"
#include "keyboard/keyboard.h"
#include "mouse/mouse.h"
#include "graphics/graphic.h"

enum ClickedKey{
  NOKEY,
  ARROW_UP,
  ARROW_DOWN,
  ARROW_LEFT,
  ARROW_RIGHT,
  ENTER,
  ESC,
  ONE,
  TWO,
  THREE,
  FOUR,
  SPACE,
};

enum FlowState{
  MENU,
  NEW_GAME,
  LOAD_GAME,
  INSTRUCTIONS,
  EXIT,
};


struct Game * game;


void start_menu();
void start_game();

void parse_keyboard_input();
void parse_mouse_input();

void update_game();
void update_menu();

void router();

void init_game(struct Game *game);
void game_loop(struct Game * game);
void controller_loop(struct Game * game, struct Menu * menu);

