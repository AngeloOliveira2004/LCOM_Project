#include <machine/int86.h>
#include "lcom/lcf.h"
#include "VBE.h"
#include "../../model/board.h"

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
int (draw_board_without_Pieces)();

int (wait_for_ESC_)();


