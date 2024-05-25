#include <lcom/lcf.h>
#include <stdio.h>

#pragma once

enum PieceType {
  PAWN = 0,
  ROOK = 1,
  KNIGHT = 2,
  BISHOP = 3,
  QUEEN = 4,
  KING = 5,
  CASTLE = 6,
  EMPTY = 7
};

enum CursorType {
  DEFAULT = 0,
  HOVERING = 1,
  SELECTED = 2
};

enum PieceColor {
  WHITE = 0,
  BLACK = 1
};

enum GameStates{
  START,
  STALEMATE,
  CHECKMATE,
  DRAW,
  END,
  ONGOING
};

struct Position {
  uint8_t x;
  uint8_t y;
};

struct mousePosition{
  uint32_t x;
  uint32_t y;
};

struct clock {
  uint8_t days;
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t a_tenth_of_a_second;
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

struct Board {
  struct Piece pieces[32];
  struct Piece squares[8][8];
  char* moves[1024];
  int movesIndex;  
};

struct Player
{
  struct Piece pieces[16];
  struct clock clock;
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
};

struct Board* create_board();
void init_board(struct Board *board);
void init_game(struct Game *game);
void move_piece(struct Game *game, enum PieceType PieceType, struct Piece * piece,
                struct Position *init_pos, struct Position *final_pos);

bool is_movement_legal(struct Board *board, enum PieceType PieceType, struct Piece * piece, 
                  struct Position *init_pos , struct Position *final_pos);

bool is_square_occupied(struct Board *board, struct Position *pos, struct Piece * piece);
bool is_inside_board(struct Position *pos);
bool is_check(struct Game *game);
bool is_checkmate(struct Game *game);
bool is_stalemate(struct Game *game);
bool is_draw(struct Game *game);
struct Piece* get_piece_from_click(int click_x, int click_y, int square_size, struct Board* board);
void change_piece_position(struct Piece *piece,struct Position *init_pos, struct Position *final_pos, struct Board *board);

void game_loop();
