#include <lcom/lcf.h>
#include <stdio.h>

#include "../model/board.h"
#include "../model/game.h"
#include "../model/menu.h"
#include "../view/view.h"
#include "keyboard/keyboard.h"
#include "mouse/mouse.h"
#include "graphics/graphic.h"

#pragma once

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


void start_menu();
void start_game();

void parse_keyboard_input();
void parse_mouse_input();

void update_game();
void update_menu();

void game_loop(struct Game * game);
void controller_loop(struct Game * game, struct Menu * menu);

