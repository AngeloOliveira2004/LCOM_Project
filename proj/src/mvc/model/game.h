#include <lcom/lcf.h>
#include <stdio.h>

#include "board.h"
#include "enum.h"

#pragma once

struct Position {
  uint8_t x;
  uint8_t y;
};

struct mousePosition{
  uint32_t x;
  uint32_t y;
};

struct Clock {
  int days;
  int seconds;
  int minutes;
  int hours;
  int a_tenth_of_a_second;
};

struct cursor {
  struct mousePosition position;
  enum CursorType type;
};

struct Piece
{
  enum PieceType type;
  struct Position position;
  bool isAlive;
  bool isWhite;
  bool canMove;
  bool hasMoved;
  bool isSelected;
  int id;
};

struct Move {
  struct Piece *piece;
  struct Position *init_pos;
  struct Position *final_pos;
};

struct Movelist {
  struct Move *moves[1024];
  int index;
};


struct Board {
  struct Piece pieces[32];
  struct Piece squares[8][8];
  char* moves[1024];
  int movesIndex;  
};


struct Player
{
  struct Piece pieces[16];
  struct Clock clock;
  bool isWhite;
  bool isWinner;
  bool isDraw;
  bool canLongCastle;
  bool canShortCastle;
};

struct Game{
  struct Board board;
  struct Player Black_player;
  struct Player White_player;
  enum GameStates state;  
  uint8_t piece_count;
  bool isWhiteTurn;
};

struct Game* create_game();
void destroy_game(struct Game* game);

struct Player* create_player();
void destroy_player(struct Player* player);
void init_player(struct Player *player, bool isWhite , int minutes, int seconds);

void initClock(struct Clock *clock , int minutes, int seconds);

void changeState(struct Game *game, enum GameStates state);
void changeTurn(struct Game *game);

void move_piece(struct Game *game, enum PieceType PieceType, struct Piece * piece,
                struct Position *init_pos, struct Position *final_pos);

bool is_stalemate(struct Game *game);
bool is_draw(struct Game *game);
struct Movelist get_possible_moves(struct Game *game, struct Piece *piece);


struct Board* create_board();

void init_board(struct Board *board);

bool is_movement_legal(struct Board *board, enum PieceType PieceType, struct Piece * piece, 
                  struct Position *init_pos , struct Position *final_pos);

bool is_square_occupied(struct Board *board, struct Position *pos);
bool can_take(struct Board *board, struct Position *pos, struct Piece * piece);
bool is_inside_board(struct Position *pos);

struct Piece* get_piece_from_click(int click_x, int click_y, int square_size, struct Board* board);
bool change_piece_position(struct Piece *piece,struct Position *init_pos, struct Position *final_pos, struct Board *board);

void remove_piece_from_board(struct Board *board, struct Position *pos);

bool is_movement_legal_without_removing(struct Board *board, enum PieceType PieceType, struct Piece * piece, struct Position *init_pos , struct Position *final_pos);

bool is_check(struct Game *game);

int promote_pawn_to_queen(struct Board *board, struct Piece *pawn);
