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


struct Board* create_board(){
  struct Board* board = (struct Board*) malloc(sizeof(struct Board));
  if(board == NULL){
    return NULL;
  }
  return board;
}

void init_board(struct Board *board) {
    // Initialize moves and movesIndex
    board->movesIndex = 0;
    for (int i = 0; i < 1024; i++) {
        board->moves[i] = NULL;
    }

    // Initialize squares
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board->squares[i][j].type = EMPTY;
            board->squares[i][j].position.x = i;
            board->squares[i][j].position.y = j;
            board->squares[i][j].isAlive = false;
            board->squares[i][j].isWhite = false;
            board->squares[i][j].canMove = false;
            board->squares[i][j].hasMoved = false;
        }
    }

    // Initialize white pawns
    for (int j = 0; j < 8; j++) {
        struct Piece whitePawn = {PAWN, {j, 1}, true, true, true, false, false , j+1};
        board->squares[j][1] = whitePawn;
        board->pieces[j] = whitePawn;
    }

    // Initialize black pawns
    for (int j = 0; j < 8; j++) {
        struct Piece blackPawn = {PAWN, {j, 6}, true, false, true, false, false , 9 + j};
        board->squares[j][6] = blackPawn;
        board->pieces[16 + j] = blackPawn;
    }

    // Initialize white pieces
    struct Piece whitePieces[] = {
        {ROOK, {0, 0}, true, true, true, false, false , 17},
        {KNIGHT, {1, 0}, true, true, true, false, false , 18},
        {BISHOP, {2, 0}, true, true, true, false, false , 19},
        {QUEEN, {3, 0}, true, true, true, false, false , 20},
        {KING, {4, 0}, true, true, true, false, false , 21},
        {BISHOP, {5, 0}, true, true, true, false, false , 22},
        {KNIGHT, {6, 0}, true, true, true, false, false , 23},
        {ROOK, {7, 0}, true, true, true, false, false , 24}
    };

    for (int i = 0; i < 8; i++) {
        board->squares[i][0] = whitePieces[i];
        board->pieces[8 + i] = whitePieces[i];
    }

    // Initialize black pieces
    struct Piece blackPieces[] = {
        {ROOK, {0, 7}, true, false, true, false, false , 25},
        {KNIGHT, {1, 7}, true, false, true, false, false , 26},
        {BISHOP, {2, 7}, true, false, true, false, false , 27},
        {QUEEN, {3, 7}, true, false, true, false, false , 28},
        {KING, {4, 7}, true, false, true, false, false , 29},
        {BISHOP, {5, 7}, true, false, true, false, false , 30},
        {KNIGHT, {6, 7}, true, false, true, false, false , 31},
        {ROOK, {7, 7}, true, false, true, false, false , 32}
    };

    for (int i = 0; i < 8; i++) {
        board->squares[i][7] = blackPieces[i];
        board->pieces[24 + i] = blackPieces[i];
    }
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

        if (((x_diff == 2 || x_diff == -2) && (y_diff == 1 || y_diff == -1)) ||
            ((x_diff == 1 || x_diff == -1) && (y_diff == 2 || y_diff == -2))) {
            if (board->squares[final_pos->x][final_pos->y].type == EMPTY || 
                board->squares[final_pos->x][final_pos->y].isWhite != board->squares[init_pos->x][init_pos->y].isWhite) {
                return true;
            } else {
                return false;
            }
        }
    }
    break; // Make sure to include a break statement to avoid falling through

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

    return false;
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
  struct Board *board = &game->board;
  if(is_movement_legal(board, PieceType, piece, init_pos, final_pos) && is_inside_board(final_pos)){
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
