#include "board.h"
/*
struct Piece
{
  enum PieceType type;
  struct Position position;
  bool isAlive;
  bool isWhite;
  bool canMove;
  bool hasMoved;
};*/
void init_board(struct Board *board){

  struct Piece WhitePawn = {PAWN, {0, 1}, true, true, true, false};
  struct Piece WhiteKnight = {KNIGHT, {1, 0}, true, true, true, false};
  struct Piece WhiteQueen = {QUEEN, {3, 0}, true, true, true, false};
  struct Piece WhiteKing = {KING, {4, 0}, true, true, true, false};
  struct Piece WhiteBishop = {BISHOP, {2, 0}, true, true, true, false};
  struct Piece WhiteRook = {ROOK, {0, 0}, true, true, true, false};

  struct Piece BlackPawn = {PAWN, {0, 6}, true, false, true, false};
  struct Piece BlackKnight = {KNIGHT, {1, 6}, true, false, true, false};
  struct Piece BlackQueen = {QUEEN, {3, 6}, true, false, true, false};
  struct Piece BlackKing = {KING, {4, 6}, true, false, true, false};
  struct Piece BlackBishop = {BISHOP, {2, 6}, true, false, true, false};
  struct Piece BlackRook = {ROOK, {0, 6}, true, false, true, false};

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

  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      board->squares[i][j].type = EMPTY;
      board->squares[i][j].position.x = i;
      board->squares[i][j].position.y = j;
      board->squares[i][j].isAlive = false;
      board->squares[i][j].isWhite = false;
      board->squares[i][j].canMove = false;
      board->squares[i][j].hasMoved = false;
    }
  }

  for(int j = 0; j < 8; j++){
    WhitePawn.position.y = j;
    board->squares[j][1] = WhitePawn;
  }

  for(int j = 0; j < 8; j++){
    BlackPawn.position.y = j;
    board->squares[j][6] = WhitePawn;
  }
  
  board->squares[0][0] = WhiteRook;
  board->squares[0][1] = WhiteKnight;
  board->squares[0][2] = WhiteBishop;
  board->squares[0][3] = WhiteQueen;
  board->squares[0][4] = WhiteKing;
  WhiteBishop.position.x = 5;  
  board->squares[0][5] = WhiteBishop;
  WhiteKnight.position.x = 6;
  board->squares[0][6] = WhiteKnight;
  WhiteRook.position.x = 7;
  board->squares[0][7] = WhiteRook;

  board->squares[7][0] = BlackRook;
  board->squares[7][1] = BlackKnight;
  board->squares[7][2] = BlackBishop;
  board->squares[7][3] = BlackQueen;
  board->squares[7][4] = BlackKing;
  BlackBishop.position.x = 5;
  board->squares[7][5] = BlackBishop;
  BlackKnight.position.x = 6;
  board->squares[7][6] = BlackKnight;
  BlackRook.position.x = 7;
  board->squares[7][7] = BlackRook;
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

bool is_piece_in_front(struct Board *board, struct Position* initalPos, struct Position *finalPos){
  if(initalPos->x == finalPos->x){
    if(initalPos->y < finalPos->y){
      for(int i = initalPos->y; i < finalPos->y; i++){
        if(board->squares[initalPos->x][i].type != EMPTY){
          return true;
        }
      }
    }else{
      for(int i = initalPos->y; i > finalPos->y; i--){
        if(board->squares[initalPos->x][i].type != EMPTY){
          return true;
        }
      }
    }
  }else if(initalPos->y == finalPos->y){
    if(initalPos->x < finalPos->x){
      for(int i = initalPos->x; i < finalPos->x; i++){
        if(board->squares[i][initalPos->y].type != EMPTY){
          return true;
        }
      }
    }else{
      for(int i = initalPos->x; i > finalPos->x; i--){
        if(board->squares[i][initalPos->y].type != EMPTY){
          return true;
        }
      }
    }
  }
  return false;
}

bool is_square_occupied(struct Board *board, struct Position *pos){
  return board->squares[pos->x][pos->y].type != EMPTY;
}

bool is_movement_legal(struct Board *board, enum PieceType PieceType, struct Piece * piece, 
                  struct Position *init_pos , struct Position *final_pos){
    switch (PieceType)
    {
    case PAWN:
    if (!piece->hasMoved) {
        if (init_pos->x == final_pos->x && (init_pos->y - final_pos->y) == 2 && !is_square_occupied(board, final_pos)) {
            return true;
        }
        if (board->squares[final_pos->x][final_pos->y].type != EMPTY && 
            board->squares[final_pos->x][final_pos->y].isWhite != board->squares[init_pos->x][init_pos->y].isWhite) {
            return true;
        }
        if (init_pos->x == final_pos->x && (init_pos->y - final_pos->y) == 1) {
            return true;
        }
    } else {
      
        if (board->squares[final_pos->x][final_pos->y].type != EMPTY && 
            board->squares[final_pos->x][final_pos->y].isWhite != board->squares[init_pos->x][init_pos->y].isWhite) {
            return true;
        }
        if (init_pos->x == final_pos->x && (init_pos->y - final_pos->y) == 1) {
            return true;
        }

        if (init_pos->x == final_pos->x && (init_pos->y - final_pos->y) == 2 && 
            !is_square_occupied(board, final_pos) && !is_piece_in_front(board, init_pos, final_pos)) {
            return false;
        }
    }
    break;

    case ROOK:
    if (init_pos->x == final_pos->x || init_pos->y == final_pos->y) {
        if (!is_square_occupied(board, final_pos) && !is_piece_in_front(board, init_pos, final_pos)) {
            if (board->squares[final_pos->x][final_pos->y].type == EMPTY || 
                board->squares[final_pos->x][final_pos->y].isWhite != board->squares[init_pos->x][init_pos->y].isWhite) {
                return true;
            } else {
                return false;
            }
        }
    }
    return false;
    break;

    case KNIGHT:
    if (is_inside_board(final_pos)) {
        int x_diff = init_pos->x - final_pos->x;
        int y_diff = init_pos->y - final_pos->y;

        if ((x_diff == 2 || x_diff == -2) && (y_diff == 1 || y_diff == -1) ||
            (x_diff == 1 || x_diff == -1) && (y_diff == 2 || y_diff == -2)) {
            if (board->squares[final_pos->x][final_pos->y].type == EMPTY || 
                board->squares[final_pos->x][final_pos->y].isWhite != board->squares[init_pos->x][init_pos->y].isWhite) {
                return true;
            } else {
                return false;
            }
        }
    }
    return false;
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
    case CASTLE:
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