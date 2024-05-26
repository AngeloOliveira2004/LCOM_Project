#include <lcom/lcf.h>
#include <stdio.h>

#pragma once

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
  bool isWhiteTurn;
};

struct Game* create_game();
void destroy_game(struct Game* game);

void init_game(struct Game *game);

void changeState(struct Game *game, enum GameStates state);
void changeTurn(struct Game *game);

void move_piece(struct Game *game, enum PieceType PieceType, struct Piece * piece,
                struct Position *init_pos, struct Position *final_pos);
