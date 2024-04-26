#include "board.h"

void init_board(struct Board *board){
  board->movesIndex = 0;
  for(int i = 0; i < 32; i++){
    board->pieces[i].isAlive = true;
    board->pieces[i].canMove = true;
    board->pieces[i].position.x = 0;
    board->pieces[i].position.y = 0;
    if(i < 16){
      board->pieces[i].isWhite = true;
    }else{
      board->pieces[i].isWhite = false;
    }
  }
  for(int i = 0; i < 1024; i++){
    board->moves[i] = NULL;
  }
}
void init_game(struct Game *game){
  init_board(&game->board);
  game->state = INITIAL;
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

bool is_movement_legal(struct Board *board, enum PieceType PieceType, struct Piece * piece, 
                  struct Position *init_pos , struct Position *final_pos){
    switch (PieceType)
    {
    case PAWN:
      if(!piece->hasMoved){
        
      }
      break;
    case ROOK:
      /* code */
      break;
    case KNIGHT:
      /* code */
      break;
    case BISHOP:
      /* code */
      break;
    case QUEEN:
      /* code */
      break;
    case KING:
      /* code */
      break;
    default:
      break;
    }
}

bool is_check(struct Game *game);
bool is_checkmate(struct Game *game);
bool is_stalemate(struct Game *game);
bool is_draw(struct Game *game);

bool is_inside_board(struct Position *pos){
  return (pos->x >= 0 && pos->x < 8 && pos->y >= 0 && pos->y < 8);
}

void move_piece(struct Game *game, enum PieceType PieceType, struct Piece * piece,
                struct Position *init_pos, struct Position *final_pos){

  if(is_movement_legal(game, PieceType, piece, init_pos, final_pos) && is_inside_board(final_pos)){
    for(int i = 0; i < 32; i++){
      if(game->board.pieces[i].position.x == init_pos->x && 
         game->board.pieces[i].position.y == init_pos->y){
        game->board.pieces[i].position.x = final_pos->x;
        game->board.pieces[i].position.y = final_pos->y;
        game->board.pieces[i].canMove = false;
        break;
      }
    }
  } 
}



void game_loop(){
  struct Game game;
  init_game(&game);
  while(game.state != END){
    //draw_board(&game);
    //get_input(&game);
    //update_game(&game); 
  }
}