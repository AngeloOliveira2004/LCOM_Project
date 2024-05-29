#include <lcom/lcf.h>
#include <stdio.h>

#include "game.h"

struct Board* create_board();
void destroy_game(struct Game* game);

void init_board(struct Board *board);
void init_game(struct Game *game);


struct Move* get_possible_moves(struct Game *game, struct Piece *piece);

bool is_movement_legal(struct Board *board, enum PieceType PieceType, struct Piece * piece, 
                  struct Position *init_pos , struct Position *final_pos);

bool is_square_occupied(struct Board *board, struct Position *pos);
bool can_take(struct Board *board, struct Position *pos, struct Piece * piece);
bool is_inside_board(struct Position *pos);
struct Piece* is_check(struct Game *game);
bool is_checkmate(struct Game *game);
bool is_stalemate(struct Game *game);
bool is_draw(struct Game *game);
struct Piece* get_piece_from_click(int click_x, int click_y, int square_size, struct Board* board);
void change_piece_position(struct Piece *piece,struct Position *init_pos, struct Position *final_pos, struct Board *board);

