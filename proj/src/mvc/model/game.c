#include "game.h"
#include "../view/view.h"

struct Game *create_game() {
  struct Game *game = (struct Game *) malloc(sizeof(struct Game));
  if (game == NULL) {
    return NULL;
  }
  return game;
}

void destroy_game(struct Game *game) {
  free(game);
}

void changeState(struct Game *game, enum GameStates state) {
  game->state = state;
}

struct Player *create_player() {

  struct Player *player = (struct Player *) malloc(sizeof(struct Player));
  if (player == NULL) {
    return NULL;
  }
  return player;
}

void destroy_player(struct Player *player) {
  free(player);
}

void initClock(struct Clock *clock, uint8_t minutes, uint8_t seconds) {
  clock->days = 0;
  clock->hours = 0;
  clock->a_tenth_of_a_second = 0;
  clock->minutes = minutes;
  clock->seconds = seconds;
}

void init_player(struct Player *player, bool isWhite, uint8_t minutes, uint8_t seconds) {
  player->isWhite = isWhite;
  player->isWinner = false;
  player->isDraw = false;
  player->canLongCastle = true;
  player->canShortCastle = true;
  initClock(&player->clock, minutes, seconds);
}

void changeTurn(struct Game *game) {
  game->isWhiteTurn = !game->isWhiteTurn;
}

void move_piece(struct Game *game, enum PieceType PieceType, struct Piece *piece,
                struct Position *init_pos, struct Position *final_pos) {
  struct Board *board = &game->board;
  if (is_movement_legal(board, PieceType, piece, init_pos, final_pos) && is_inside_board(final_pos) && piece->isWhite == game->isWhiteTurn) {
    for (int i = 0; i < 32; i++) {
      if (game->board.pieces[i].position.x == init_pos->x &&
          game->board.pieces[i].position.y == init_pos->y) {
        game->board.pieces[i].position.x = final_pos->x;
        game->board.pieces[i].position.y = final_pos->y;
        break;
      }
    }
  }
}

struct Piece *is_check(struct Game *game) {

  struct Board *board = &game->board;
  struct Position king_pos;
  bool isWhite = false;

  for (int i = 0; i < 32; i++) {
    if (board->pieces[i].type == KING) {
      if (board->pieces[i].isWhite == game->isWhiteTurn) {
        king_pos.x = board->pieces[i].position.x;
        king_pos.y = board->pieces[i].position.y;
        isWhite = board->pieces[i].isWhite;
        break;
      }
    }
  }

  for (int i = 0; i < 32; i++) {
    if (board->pieces[i].isWhite != isWhite) {
      struct Move *possible_moves = get_possible_moves(game, &board->pieces[i]);
      for (int j = 0; j < 32; j++) {
        struct Move move = possible_moves[j];
        struct Position *init_pos = move.init_pos;
        struct Position *final_pos = move.final_pos;
        if (is_movement_legal(board, board->pieces[i].type, &board->pieces[i], init_pos, final_pos)) {
          if (final_pos->x == king_pos.x && final_pos->y == king_pos.y) {
            return &board->pieces[i];
          }
        }
      }
    }
  }

  return NULL;
}

bool is_checkmate(struct Game *game) {
  if (is_check(game)) {
    struct Board *board = &game->board;
    struct Position king_pos;
    bool isWhite = false;

    for (int i = 0; i < 32; i++) {
      if (board->pieces[i].type == KING) {
        if (board->pieces[i].isWhite == game->isWhiteTurn) {
          king_pos.x = board->pieces[i].position.x;
          king_pos.y = board->pieces[i].position.y;
          isWhite = board->pieces[i].isWhite;
          break;
        }
      }
    }

    for (int i = 0; i < 32; i++) {
      if (board->pieces[i].isWhite == isWhite) {
        struct Move *possible_moves = get_possible_moves(game, &board->pieces[i]);
        for (int j = 0; j < 32; j++) {
          struct Move move = possible_moves[j];
          struct Position *init_pos = move.init_pos;
          struct Position *final_pos = move.final_pos;
          if (is_movement_legal(board, board->pieces[i].type, &board->pieces[i], init_pos, final_pos)) {
            return false;
          }
        }
      }
    }
    return true;
  }

  return false;
}

bool is_stalemate(struct Game *game) {

  if (game->isWhiteTurn) {
    for (int i = 0; i < 16; i++) {
      struct Piece piece = game->White_player.pieces[i];

      if (piece.canMove) {
        return false;
      }
    }
  }
  else {
    for (int i = 0; i < 16; i++) {
      struct Piece piece = game->Black_player.pieces[i];

      if (piece.canMove) {
        return false;
      }
    }
  }

  return true;
}

bool is_draw(struct Game *game) {

  int whiteKnightCount = 0;
  int blackKnightCount = 0;
  int whiteQueenCount = 0;
  int blackQueenCount = 0;
  int whiteBishopCount = 0;
  int blackBishopCount = 0;
  int whiteRookCount = 0;
  int blackRookCount = 0;
  int whitePawnCount = 0;
  int blackPawnCount = 0;

  for (int i = 0; i < 32; i++) {

    switch (game->board.pieces[i].type) {
      case PAWN:
        if (game->board.pieces[i].isWhite) {
          whitePawnCount++;
        }
        else {
          blackPawnCount++;
        }
        break;
      case ROOK:
        if (game->board.pieces[i].isWhite) {
          whiteRookCount++;
        }
        else {
          blackRookCount++;
        }
        break;
      case BISHOP:
        if (game->board.pieces[i].isWhite) {
          whiteBishopCount++;
        }
        else {
          blackBishopCount++;
        }
        break;
      case QUEEN:
        if (game->board.pieces[i].isWhite) {
          whiteQueenCount++;
        }
        else {
          blackQueenCount++;
        }
        break;
      case KNIGHT:
        if (game->board.pieces[i].isWhite) {
          whiteKnightCount++;
        }
        else {
          blackKnightCount++;
        }
        break;
      default:
        break;
    }
  }

  if (whitePawnCount > 0 || blackPawnCount > 0 || blackQueenCount > 0 || whiteQueenCount > 0 || blackRookCount > 0 || whiteRookCount > 0) {
    return false;
  }

  if ((whiteBishopCount == 2 || blackBishopCount == 2)) {
    return false;
  }

  if ((whiteBishopCount == 1 && whiteKnightCount == 1) ||
      (blackBishopCount == 1 && blackKnightCount == 1)) {
    return false;
  }

  return true;
}

struct Move *get_possible_moves(struct Game *game, struct Piece *piece) {
  struct Move *possible_moves = (struct Move *) malloc(32 * sizeof(struct Move));
  if (possible_moves == NULL) {
    return NULL;
  }
  int index = 0;
  struct Position pos;
  pos.x = piece->position.x;
  pos.y = piece->position.y;

  uint8_t x = pos.x;
  uint8_t y = pos.y;

  switch (piece->type) {
    case EMPTY:
      break;
    case CASTLE:
      break;
    case PAWN:
      if (piece->isWhite) {
        if (!piece->hasMoved) {
          pos.y += 2;
          if (!is_square_occupied(&game->board, &pos)) {
            index += sizeof(struct Move);
            if (is_inside_board(&pos)) {
              struct Move move = {piece, &piece->position, &pos};
              possible_moves[index] = move;
            }
          }
        }
        else {
          pos.y += 1;
          if (!is_square_occupied(&game->board, &pos)) {
            index += sizeof(struct Move);
            if (is_inside_board(&pos)) {
              struct Move move = {piece, &piece->position, &pos};
              possible_moves[index] = move;
            }
          }
        }
      }
      break;
    case ROOK:

      for (int i = 0; i < 8; i++) {
        if (i != piece->position.y) {
          pos.y = i;
          if (is_inside_board(&pos)) {
            index += sizeof(struct Move);
            struct Move move = {piece, &piece->position, &pos};
            possible_moves[index] = move;
          }
        }
      }
      pos.x = x;
      pos.y = y;
      for (int i = 0; i < 8; i++) {
        if (i != piece->position.x) {
          pos.x = i;
          if (is_inside_board(&pos)) {
            index += sizeof(struct Move);
            struct Move move = {piece, &piece->position, &pos};
            possible_moves[index] = move;
          }
        }
      }
      break;
    case KNIGHT:
      pos.x = piece->position.x + 2;
      pos.y = piece->position.y + 1;
      if (is_inside_board(&pos) && !is_square_occupied(&game->board, &pos)) {
        index += sizeof(struct Move);
        struct Move move = {piece, &piece->position, &pos};
        possible_moves[index] = move;
      }

      pos.x = x;
      pos.y = y;

      pos.x = piece->position.x + 2;
      pos.y = piece->position.y - 1;
      if (is_inside_board(&pos) && !is_square_occupied(&game->board, &pos)) {
        index += sizeof(struct Move);
        struct Move move = {piece, &piece->position, &pos};
        possible_moves[index] = move;
      }

      pos.x = x;
      pos.y = y;

      pos.x = piece->position.x - 2;
      pos.y = piece->position.y + 1;
      if (is_inside_board(&pos)) {
        index += sizeof(struct Move);
        struct Move move = {piece, &piece->position, &pos};
        possible_moves[index] = move;
      }

      pos.x = x;
      pos.y = y;

      pos.x = piece->position.x - 2;
      pos.y = piece->position.y - 1;
      if (is_inside_board(&pos) && !is_square_occupied(&game->board, &pos)) {
        index += sizeof(struct Move);
        struct Move move = {piece, &piece->position, &pos};
        possible_moves[index] = move;
      }

      pos.x = x;
      pos.y = y;

      pos.x = piece->position.x + 1;
      pos.y = piece->position.y + 2;
      if (is_inside_board(&pos) && !is_square_occupied(&game->board, &pos)) {
        index += sizeof(struct Move);
        struct Move move = {piece, &piece->position, &pos};
        possible_moves[index] = move;
      }

      pos.x = x;
      pos.y = y;

      pos.x = piece->position.x + 1;
      pos.y = piece->position.y - 2;
      if (is_inside_board(&pos) && !is_square_occupied(&game->board, &pos)) {
        index += sizeof(struct Move);
        struct Move move = {piece, &piece->position, &pos};
        possible_moves[index] = move;
      }

      pos.x = x;
      pos.y = y;

      pos.x = piece->position.x - 1;
      pos.y = piece->position.y + 2;
      if (is_inside_board(&pos) && !is_square_occupied(&game->board, &pos)) {
        index += sizeof(struct Move);
        struct Move move = {piece, &piece->position, &pos};
        possible_moves[index] = move;
      }

      pos.x = x;
      pos.y = y;

      pos.x = piece->position.x - 1;
      pos.y = piece->position.y - 2;
      if (is_inside_board(&pos) && !is_square_occupied(&game->board, &pos)) {
        index += sizeof(struct Move);
        struct Move move = {piece, &piece->position, &pos};
        possible_moves[index] = move;
      }
      break;
    case BISHOP:
      for (int i = 0; i < 8; i++) {
        if (i != piece->position.x && i != piece->position.y) {
          pos.x = i;
          pos.y = i;
          if (is_inside_board(&pos) && !is_square_occupied(&game->board, &pos)) {
            index += sizeof(struct Move);
            struct Move move = {piece, &piece->position, &pos};
            possible_moves[index] = move;
          }
        }
      }
      break;
    case QUEEN:
      for (int i = 0; i < 8; i++) {
        if (i != piece->position.y) {
          pos.y = i;
          if (is_inside_board(&pos)) {
            index += sizeof(struct Move);
            struct Move move = {piece, &piece->position, &pos};
            possible_moves[index] = move;
          }
        }
      }

      pos.x = x;
      pos.y = y;

      for (int i = 0; i < 8; i++) {
        if (i != piece->position.x) {
          pos.x = i;
          if (is_inside_board(&pos)) {
            index += sizeof(struct Move);
            struct Move move = {piece, &piece->position, &pos};
            possible_moves[index] = move;
          }
        }
      }

      pos.x = x;
      pos.y = y;

      for (int i = 0; i < 8; i++) {
        if (i != piece->position.x && i != piece->position.y) {
          pos.x = i;
          pos.y = i;
          if (is_inside_board(&pos) && !is_square_occupied(&game->board, &pos)) {
            index += sizeof(struct Move);
            struct Move move = {piece, &piece->position, &pos};
            possible_moves[index] = move;
          }
        }
      }

      pos.x = x;
      pos.y = y;

      for (int i = 0; i < 8; i++) {
        if (i != piece->position.x && i != piece->position.y) {
          pos.x = i;
          pos.y = 7 - i;
          if (is_inside_board(&pos) && !is_square_occupied(&game->board, &pos)) {
            index += sizeof(struct Move);
            struct Move move = {piece, &piece->position, &pos};
            possible_moves[index] = move;
          }
        }
      }

      break;

    case KING:
      pos.x = piece->position.x + 1;

      if (is_inside_board(&pos) && !is_square_occupied(&game->board, &pos)) {
        index += sizeof(struct Move);
        struct Move move = {piece, &piece->position, &pos};
        possible_moves[index] = move;
      }

      pos.x = x;

      pos.x = piece->position.x - 1;

      if (is_inside_board(&pos) && !is_square_occupied(&game->board, &pos)) {
        index += sizeof(struct Move);
        struct Move move = {piece, &piece->position, &pos};
        possible_moves[index] = move;
      }

      pos.x = x;

      pos.y = piece->position.y + 1;

      if (is_inside_board(&pos) && !is_square_occupied(&game->board, &pos)) {
        index += sizeof(struct Move);
        struct Move move = {piece, &piece->position, &pos};
        possible_moves[index] = move;
      }

      pos.y = y;

      pos.y = piece->position.y - 1;

      if (is_inside_board(&pos) && !is_square_occupied(&game->board, &pos)) {
        index += sizeof(struct Move);
        struct Move move = {piece, &piece->position, &pos};
        possible_moves[index] = move;
      }

      pos.y = y;

      pos.x = piece->position.x + 1;

      pos.y = piece->position.y + 1;

      if (is_inside_board(&pos) && !is_square_occupied(&game->board, &pos)) {
        index += sizeof(struct Move);
        struct Move move = {piece, &piece->position, &pos};
        possible_moves[index] = move;
      }

      pos.x = x;
      pos.y = y;

      pos.x = piece->position.x - 1;
      pos.y = piece->position.y - 1;

      if (is_inside_board(&pos) && !is_square_occupied(&game->board, &pos)) {
        index += sizeof(struct Move);
        struct Move move = {piece, &piece->position, &pos};
        possible_moves[index] = move;
      }

      pos.x = x;
      pos.y = y;

      pos.x = piece->position.x + 1;
      pos.y = piece->position.y - 1;

      if (is_inside_board(&pos) && !is_square_occupied(&game->board, &pos)) {
        index += sizeof(struct Move);
        struct Move move = {piece, &piece->position, &pos};
        possible_moves[index] = move;
      }

      pos.x = x;
      pos.y = y;

      pos.x = piece->position.x - 1;
      pos.y = piece->position.y + 1;

      if (is_inside_board(&pos) && !is_square_occupied(&game->board, &pos)) {
        index += sizeof(struct Move);
        struct Move move = {piece, &piece->position, &pos};
        possible_moves[index] = move;
      }

      break;
  }

  return possible_moves;
}

struct Board *create_board() {
  struct Board *board = (struct Board *) malloc(sizeof(struct Board));
  if (board == NULL) {
    return NULL;
  }
  return board;
}

void destroy_board(struct Board *board) {
  free(board);
}

void init_board(struct Board *board) {

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
    struct Piece whitePawn = {PAWN, {j, 1}, true, true, true, false, false, j + 1};
    board->squares[j][1] = whitePawn;
    board->pieces[j] = whitePawn;
  }

  // Initialize black pawns
  for (int j = 0; j < 8; j++) {
    struct Piece blackPawn = {PAWN, {j, 6}, true, false, true, false, false, 9 + j};
    board->squares[j][6] = blackPawn;
    board->pieces[16 + j] = blackPawn;
  }

  // Initialize white pieces
  struct Piece whitePieces[] = {
    {ROOK, {0, 0}, true, true, true, false, false, 17},
    {KNIGHT, {1, 0}, true, true, true, false, false, 18},
    {BISHOP, {2, 0}, true, true, true, false, false, 19},
    {QUEEN, {3, 0}, true, true, true, false, false, 20},
    {KING, {4, 0}, true, true, true, false, false, 21},
    {BISHOP, {5, 0}, true, true, true, false, false, 22},
    {KNIGHT, {6, 0}, true, true, true, false, false, 23},
    {ROOK, {7, 0}, true, true, true, false, false, 24}};

  for (int i = 0; i < 8; i++) {
    board->squares[i][0] = whitePieces[i];
    board->pieces[8 + i] = whitePieces[i];
  }

  // Initialize black pieces
  struct Piece blackPieces[] = {
    {ROOK, {0, 7}, true, false, true, false, false, 25},
    {KNIGHT, {1, 7}, true, false, true, false, false, 26},
    {BISHOP, {2, 7}, true, false, true, false, false, 27},
    {QUEEN, {3, 7}, true, false, true, false, false, 28},
    {KING, {4, 7}, true, false, true, false, false, 29},
    {BISHOP, {5, 7}, true, false, true, false, false, 30},
    {KNIGHT, {6, 7}, true, false, true, false, false, 31},
    {ROOK, {7, 7}, true, false, true, false, false, 32}};

  for (int i = 0; i < 8; i++) {
    board->squares[i][7] = blackPieces[i];
    board->pieces[24 + i] = blackPieces[i];
  }
}

bool is_piece_in_front(struct Board *board, struct Position *initialPos, struct Position *finalPos) {
  if (initialPos->x == finalPos->x) {
    if (initialPos->y < finalPos->y) {
      for (int i = initialPos->y + 1; i < finalPos->y; i++) {
        if (board->squares[initialPos->x][i].type != EMPTY) {
          return true;
        }
      }
    }
    else {
      for (int i = initialPos->y - 1; i > finalPos->y; i--) {
        if (board->squares[initialPos->x][i].type != EMPTY) {
          return true;
        }
      }
    }
  }
  else if (initialPos->y == finalPos->y) {
    if (initialPos->x < finalPos->x) {
      for (int i = initialPos->x + 1; i < finalPos->x; i++) {
        if (board->squares[i][initialPos->y].type != EMPTY) {
          return true;
        }
      }
    }
    else {
      for (int i = initialPos->x - 1; i > finalPos->x; i--) {
        if (board->squares[i][initialPos->y].type != EMPTY) {
          return true;
        }
      }
    }
  }
  return false;
}

bool is_piece_in_diagonal(struct Board *board, struct Position *initialPos, struct Position *finalPos) {
  if (initialPos->x < finalPos->x && initialPos->y < finalPos->y) {
    for (int i = initialPos->x + 1, j = initialPos->y + 1; i < finalPos->x && j < finalPos->y; i++, j++) {
      if (board->squares[i][j].type != EMPTY) {
        return true;
      }
    }
  }
  else if (initialPos->x < finalPos->x && initialPos->y > finalPos->y) {
    for (int i = initialPos->x + 1, j = initialPos->y - 1; i < finalPos->x && j > finalPos->y; i++, j--) {
      if (board->squares[i][j].type != EMPTY) {
        return true;
      }
    }
  }
  else if (initialPos->x > finalPos->x && initialPos->y < finalPos->y) {
    for (int i = initialPos->x - 1, j = initialPos->y + 1; i > finalPos->x && j < finalPos->y; i--, j++) {
      if (board->squares[i][j].type != EMPTY) {
        return true;
      }
    }
  }
  else if (initialPos->x > finalPos->x && initialPos->y > finalPos->y) {
    for (int i = initialPos->x - 1, j = initialPos->y - 1; i > finalPos->x && j > finalPos->y; i--, j--) {
      if (board->squares[i][j].type != EMPTY) {
        return true;
      }
    }
  }

  return false;
}

bool is_square_occupied(struct Board *board, struct Position *pos) {
  return board->squares[pos->x][pos->y].type != EMPTY;
}

bool can_take(struct Board *board, struct Position *pos, struct Piece *piece) {
  return board->squares[pos->x][pos->y].type != EMPTY &&
         board->squares[pos->x][pos->y].isWhite != piece->isWhite;
}

bool is_movement_legal(struct Board *board, enum PieceType PieceType, struct Piece *piece,
                       struct Position *init_pos, struct Position *final_pos) {
  switch (PieceType) {
    case PAWN:
      if (!piece->hasMoved) {
        bool isPieceInFront = is_piece_in_front(board, init_pos, final_pos);
        bool isSquareOccupied = is_square_occupied(board, final_pos);

        if ((init_pos->y - final_pos->y) <= 2 && (init_pos->y - final_pos->y) > 0) {

          if (!isSquareOccupied && !isPieceInFront && init_pos->x == final_pos->x) {
            piece->hasMoved = true;
            return true;
          }

          if (can_take(board, final_pos, piece) && ((init_pos->x - final_pos->x) == 1 && (init_pos->y - final_pos->y) == 1)) {
            piece->hasMoved = true;
            return true;
          }

          return false;
        }
      }
      else {
        bool isPieceInFront = is_piece_in_front(board, init_pos, final_pos);
        bool isSquareOccupied = is_square_occupied(board, final_pos);

        if ((init_pos->y - final_pos->y) == 1) {

          if (!isSquareOccupied && !isPieceInFront && init_pos->x == final_pos->x) {
            return true;
          }

          if (can_take(board, final_pos, piece) && ((init_pos->x - final_pos->x) == 1 && (init_pos->y - final_pos->y) == 1)) {
            return true;
          }

          return false;
        }
      }
      break;

    case ROOK:
      if (init_pos->x == final_pos->x || init_pos->y == final_pos->y) {
        bool isPieceInFront = is_piece_in_front(board, init_pos, final_pos);
        bool isSquareOccupied = is_square_occupied(board, final_pos);

        if (!isSquareOccupied && !isPieceInFront) {
          piece->hasMoved = true;
          return true;
        }

        if (can_take(board, final_pos, piece) && (init_pos->x == final_pos->x || init_pos->y == final_pos->y)) {
          piece->hasMoved = true;
          return true;
        }
      }
      return false;
      break;

    case KNIGHT:
      if (is_inside_board(final_pos)) {
        int x_diff = init_pos->x - final_pos->x;
        int y_diff = init_pos->y - final_pos->y;
        bool isSquareOccupied = is_square_occupied(board, final_pos);
        if ((abs(x_diff) == 2 && abs(y_diff) == 1) || (abs(x_diff) == 1 && abs(y_diff) == 2)) {
          if (!isSquareOccupied) {
            return true;
          }

          if (can_take(board, final_pos, piece)) {
            return true;
          }
        }
      }
      break;

      return false;
      break;
    case BISHOP:
      if (is_inside_board(final_pos)) {
        if (abs(init_pos->x - final_pos->x) == abs(init_pos->y - final_pos->y)) {
          if (!is_square_occupied(board, final_pos) && !is_piece_in_diagonal(board, init_pos, final_pos)) {
            bool isPieceInDiagonal = is_piece_in_diagonal(board, init_pos, final_pos);
            bool isSquareOccupied = is_square_occupied(board, final_pos);

            if (isSquareOccupied && can_take(board, final_pos, piece)) {
              return true;
            }

            if (!isSquareOccupied && !isPieceInDiagonal) {
              return true;
            }
          }
        }
        return false;
      }
      break;
    case QUEEN:
      if (is_inside_board(final_pos)) {
        if (init_pos->x == final_pos->x || init_pos->y == final_pos->y) {
          bool isPieceInFront = is_piece_in_front(board, init_pos, final_pos);
          bool isSquareOccupied = is_square_occupied(board, final_pos);

          if (!isSquareOccupied && !isPieceInFront) {
            piece->hasMoved = true;
            return true;
          }

          if (can_take(board, final_pos, piece) && (init_pos->x == final_pos->x || init_pos->y == final_pos->y)) {
            piece->hasMoved = true;
            return true;
          }
        }else if (abs(init_pos->x - final_pos->x) == abs(init_pos->y - final_pos->y)) {
          if (!is_square_occupied(board, final_pos) && !is_piece_in_diagonal(board, init_pos, final_pos)) {
            bool isPieceInDiagonal = is_piece_in_diagonal(board, init_pos, final_pos);
            bool isSquareOccupied = is_square_occupied(board, final_pos);

            if (isSquareOccupied && can_take(board, final_pos, piece)) {
              return true;
            }

            if (!isSquareOccupied && !isPieceInDiagonal) {
              return true;
            }
          }
        }
        return false;
      }
      break;
    case KING:
      if (is_inside_board(final_pos)) {
        if (abs(init_pos->x - final_pos->x) <= 1 && abs(init_pos->y - final_pos->y) <= 1) {
          bool isSquareOccupied = is_square_occupied(board, final_pos);

          if(!isSquareOccupied){
            return true;
          }

          if(can_take(board, final_pos, piece)){
            return true;
          }
        }
      }
      return false;
      break;
    case CASTLE:
      /* code */
      break;
    default:
      break;
  }

  return false;
}

bool is_inside_board(struct Position *pos) {
  return (pos->x >= 0 && pos->x < 8 && pos->y >= 0 && pos->y < 8);
}

struct Piece *get_piece_from_click(int click_x, int click_y, int square_size, struct Board *board) {

  struct Position pos;

  pos.x = (click_x - 200) / square_size;
  pos.y = (click_y - 100) / square_size;

  if (board == NULL) {
    printf("null board\n");
  }

  if (is_inside_board(&pos)) {
    if (board->squares[pos.x][pos.y].type != EMPTY) {
      printf("Piece type: %d\n", board->squares[pos.x][pos.y].type);
      return &board->squares[pos.x][pos.y];
    }
  }

  return NULL;
}

bool change_piece_position(struct Piece *piece,
                           struct Position *init_pos, struct Position *final_pos, struct Board *board) {

  printf("InitPosX %d\n", piece->position.x);
  printf("InitPosY %d\n", piece->position.y);

  printf("FinalPosX %d\n", final_pos->x);
  printf("FinalPosY %d\n", final_pos->y);

  if (is_movement_legal(board, piece->type, piece, init_pos, final_pos) && is_inside_board(final_pos)) {
    for (int i = 0; i < 32; i++) {
      if (board->pieces[i].position.x == init_pos->x && board->pieces[i].position.y == init_pos->y) {
        advance_piece(piece, final_pos, board);
        board->pieces[i].position.x = final_pos->x;
        board->pieces[i].position.y = final_pos->y;
        piece->position.x = final_pos->x;
        piece->position.y = final_pos->y;
        board->squares[final_pos->x][final_pos->y] = *piece;
        board->squares[init_pos->x][init_pos->y].type = EMPTY;
        return true;
        break;
      }
    }
    printf("Piece moved to new position: (%d, %d)\n", piece->position.x, piece->position.y);
    printf("Square at final position now has piece type: %d\n", board->squares[final_pos->x][final_pos->y].type);
    printf("Square at initial position is now empty: %d\n", board->squares[init_pos->x][init_pos->y].type);
  }
  return false;
}
