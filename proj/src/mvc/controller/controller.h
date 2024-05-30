#pragma once

#include <lcom/lcf.h>
#include <stdio.h>
#include "../model/game.h"
#include "../model/board.h"
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
  FIVE,
  SIX,
  SPACE,
};

enum FlowState{
  MENU,
  GAME,
  NEW_GAME,
  LOAD_GAME,
  INSTRUCTIONS,
  WINNER_SCREEN,
  EXIT,
};

struct Game * game;
struct Board * boardArray[1024]; 
int index_;
int max_index;

void start_menu();
void start_game();

void parse_keyboard_input();
void parse_mouse_input();

void update_game();
void update_menu();

void router();

void init_game(struct Game *game,int minutes, int seconds);
void game_loop(struct Game * game);
void controller_loop(struct Game * game, struct Menu * menu);
void decrease_player_timer();



