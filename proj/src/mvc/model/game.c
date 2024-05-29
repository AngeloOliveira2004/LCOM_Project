#include "game.h"

struct Game* create_game(){

  struct Game* game = (struct Game*) malloc(sizeof(struct Game));
  if(game == NULL){
    return NULL;
  }
  return game;

}

void destroy_game(struct Game* game){
  free(game);
}

void changeState(struct Game *game, enum GameStates state){
  game->state = state;
}

struct Player* create_player(){

  struct Player* player = (struct Player*) malloc(sizeof(struct Player));
  if(player == NULL){
    return NULL;
  }
  return player;

}

void destroy_player(struct Player* player){
  free(player);
}

void initClock(struct Clock *clock , uint8_t minutes, uint8_t seconds){
  clock->days = 0;
  clock->hours = 0;
  clock->a_tenth_of_a_second = 0;
  clock->minutes = minutes;
  clock->seconds = seconds;
}

void init_player(struct Player *player, bool isWhite ,uint8_t minutes, uint8_t seconds){
  player->isWhite = isWhite;
  player->isWinner = false;
  player->isDraw = false;
  player->canLongCastle = true;
  player->canShortCastle = true;
  initClock(&player->clock , minutes , seconds)
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