#include <machine/int86.h>
#include "lcom/lcf.h"
#include "VBE.h"
#include "../../model/game.h"
#include "../mouse/mouse.h"
#include "sprites/GameElements/clock_150.xpm"
#include "sprites/Characters/Characters20x32/0_20.xpm"
#include "sprites/Characters/Characters20x32/1_20.xpm"
#include "sprites/Characters/Characters20x32/2_20.xpm"
#include "sprites/Characters/Characters20x32/3_20.xpm"
#include "sprites/Characters/Characters20x32/4_20.xpm"
#include "sprites/Characters/Characters20x32/5_20.xpm"
#include "sprites/Characters/Characters20x32/6_20.xpm"
#include "sprites/Characters/Characters20x32/7_20.xpm"
#include "sprites/Characters/Characters20x32/8_20.xpm"
#include "sprites/Characters/Characters20x32/9_20.xpm"
#include "sprites/Characters/Characters20x32/two_point.xpm"

#pragma once

#define MAX_PIXELS 75

struct Pixel {
    int x;
    int y;
    uint32_t color;
};

//int (vg_exit)(); dado por LCF

int (set_graphic_mode)(uint16_t* mode);
int (set_frame_mode)(uint16_t* mode);
int (set_text_mode)();
int (load_xpm)(xpm_map_t img ,enum PieceType pieceType , bool isWhite);
int (load_xpm_cursor)();
int load_xpm_menu();
int (load_numbers)();
int (load_xpm_clocks_board)();

int allocate_buffers();
void swap_buffers();
void swap_BackgroundBuffer();
void copy_BackGroundBuffer();
void erase_buffer();

int (get_h_res)();
int (get_v_res)();

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (adjust_color)(uint32_t color, uint16_t* new_color);

int (draw_xpm) (xpm_map_t img, uint16_t x, uint16_t y);
int (draw_black_piece)(uint16_t x, uint16_t y , enum PieceType pieceType);
int (draw_white_piece)(uint16_t x, uint16_t y , enum PieceType pieceType);
int draw_both_clocks();
int (draw_board_without_Pieces)();
int (draw_cursor_mouse)(uint16_t x, uint16_t y , enum CursorType cursorType);
int (draw_menu)(uint16_t x, uint16_t y);
int (draw_number)(uint16_t x , uint16_t y , int number);
int (draw_two_points)(uint16_t x , uint16_t);
int(draw_game_background)();
int load_xpm_game_background();

int (wait_for_ESC_)();


