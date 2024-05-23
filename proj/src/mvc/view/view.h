#include "../controller/graphics/graphic.h"
#include <machine/int86.h>
#include "lcom/lcf.h"
#include "../model/board.h"
#include "../model/menu.h"


int initialize_graphics(uint16_t* mode);

int (fill)(int x , int y , int width , int height , uint32_t color);
int (draw_board)(struct Board* board);
int draw_board_except_one_piece(int id , struct Board* board);
int (draw_backBackGround)();
int (draw_BackGround_Without_Erase)();
int (draw_Clocks)();
int draw_piece();
int draw_initial_pos();
int draw_cursor(struct cursor* cursor);

int (return_to_initial_pos)(struct Piece* piece , struct Position* initialPos , struct Board * board);
int (advance_piece)(struct Piece* piece , struct Position* initialPos , struct Board* board);

int (draw_pawn)(struct Piece* piece);
int (draw_rook)();
int (draw_knight)();
int (draw_bishop)();
int (draw_queen)();
int (draw_king)();
