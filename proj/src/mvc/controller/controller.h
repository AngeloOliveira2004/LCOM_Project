#include <lcom/lcf.h>
#include <stdio.h>

#include "../model/board.h"
#include "../model/game.h"
#include "../model/menu.h"

#pragma once

void start_menu();
void start_game();
void check_mouse_input();
void check_keyboard_input();
void parse_keyboard_input();
void parse_mouse_input();
void update_game();
void update_menu();

void game_loop(struct Game * game);
void controller_loop(struct Game * game, struct Menu * menu);

