#include "game.h"

struct Game* create_game(){

  struct Game* game (struct Game*) malloc(sizeof(struct Game));
  if(game == NULL){
    return NULL;
  }
  return game;

}

void destroy_game(struct Game* game){
  free(game);
}

void init_game(struct Game *game){
  init_board(&game->board);
  game->state = START;
  game->piece_count = 32;
  game->Black_player.isWhite = false;
  game->Black_player.isWinner = false;
  game->Black_player.isDraw = false;
  game->Black_player.canLongCastle = true;
  game->Black_player.canShortCastle = true;
  game->White_player.isWhite = true;
  game->White_player.isWinner = false;
  game->White_player.isDraw = false;
  game->White_player.canLongCastle = true;
  game->White_player.canShortCastle = true;
}

void changeState(struct Game *game, enum GameStates state){
  game->state = state;
}

void changeTurn(struct Game *game){
  game->isWhiteTurn = !game->isWhiteTurn;
}

void move_piece(struct Game *game, enum PieceType PieceType, struct Piece * piece,
                struct Position *init_pos, struct Position *final_pos){
  struct Board *board = &game->board;
  if(is_movement_legal(board, PieceType, piece, init_pos, final_pos) && is_inside_board(final_pos) && piece->isWhite == game->isWhiteTurn){
    for(int i = 0; i < 32; i++){
      if(game->board.pieces[i].position.x == init_pos->x && 
         game->board.pieces[i].position.y == init_pos->y){
        game->board.pieces[i].position.x = final_pos->x;
        game->board.pieces[i].position.y = final_pos->y;
        break;
      }
    }
  } 
}