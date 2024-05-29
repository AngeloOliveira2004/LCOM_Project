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
