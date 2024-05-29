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
  CHECK,
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

struct Clock {
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

struct Move {
  struct Piece *piece;
  struct Position *init_pos;
  struct Position *final_pos;
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
void init_player(struct Player *player, bool isWhite , uint8_t minutes, uint8_t seconds);

void initClock(struct Clock *clock , uint8_t minutes, uint8_t seconds);

void changeState(struct Game *game, enum GameStates state);
void changeTurn(struct Game *game);

void move_piece(struct Game *game, enum PieceType PieceType, struct Piece * piece,
                struct Position *init_pos, struct Position *final_pos);

