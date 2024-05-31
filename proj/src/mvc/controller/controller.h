#pragma once

#include <lcom/lcf.h>
#include <stdio.h>
#include "../model/game.h"
#include "../view/view.h"
#include "keyboard/keyboard.h"
#include "rtc/rtc.h"
#include "rtc/I_RTC.h"

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
  PAUSE_MENU,
  INSTRUCTIONS,
  WINNER_SCREEN,
  EXIT,
};

struct Game * game;
struct Board boardArray[1024]; 
int index_;
int max_index;

void parse_keyboard_input();
void parse_mouse_input();

void update_game();
void update_menu();

void router();

void init_game(struct Game *game,int minutes, int seconds);
void game_loop(struct Game * game);
void decrease_player_timer();
int(change_game_state_to_menu)();
