#include "../controller/graphics/graphic.h"
#include "sprites/pieces.xpm"
#include "view.h"

int initialize_graphics(uint16_t* mode){

  if(set_frame_mode(mode) != 0 )
    return 1;

  if(set_graphic_mode(mode) != 0 )
    return 1;

  if(allocate_buffers() != 0)
    return 1;
  
  printf("Initialized");
  return 0;
}

int (fill)(int x , int y , int width , int height , uint32_t color){

  for(int i = 0 ; i < height; i++){
    if(vg_draw_hline(x, y+i, width, color) != 0){
      return 1;
    }
  }

    return 0;
}

int (draw_board)(struct Board* board){

  for (int i = 0; i < 32; i++) {
    draw_piece(&board->pieces[i]);
  }

  swap_buffers();

  return 0;
}


int draw_Clocks(){

  for(int i = 650 ; i < 800; i++){
    for(int j = 0; j < 100; j++){
      if(vg_draw_pixel(i*CELL_SIZE_WIDTH, j*CELL_SIZE_HEIGHT, 0xFFFFF) != 0){
        continue;
      }
    }

    for(int j = 600 ; j > 500; j--){
      if(vg_draw_pixel(i*CELL_SIZE_WIDTH, j*CELL_SIZE_HEIGHT, 0xFFFFFF) != 0){
        continue;
      }
    }
  }

  return 0;
}

int draw_backBackGround(){
 
  erase_buffer();
  
  if(draw_board_without_Pieces() != 0)
    return 1;

  swap_buffers();

  return 0;
}

int (draw_pawn)(struct Piece* piece){
  int initialX = piece->position.x * CELL_SIZE_WIDTH + 10;
  int initialY = piece->position.y * CELL_SIZE_HEIGHT + 10;


  if(piece->isWhite){
    draw_white_piece(Chess_plt45, initialX, initialY);
  }else{
    draw_black_piece(Chess_black_plt45, initialX, initialY);
  }

  return 0;
}

int (draw_rook)(struct Piece* piece){
  int initialX = piece->position.x * CELL_SIZE_WIDTH + 10;
  int initialY = piece->position.y * CELL_SIZE_HEIGHT + 10;

  if(piece->isWhite){
    draw_white_piece(Chess_rlt45, initialX, initialY);
  }else{
    printf("Drawing Black Rook\n");
    draw_black_piece(Chess_black_rlt45, initialX, initialY);
  }

  return 0;
}

int (draw_knight)(struct Piece* piece){
  int initialX = piece->position.x * CELL_SIZE_WIDTH + 10;
  int initialY = piece->position.y * CELL_SIZE_HEIGHT + 10;

  if(piece->isWhite){
    draw_white_piece(Chess_nlt45, initialX, initialY);
  }else{
    draw_black_piece(Chess_black_nlt45, initialX, initialY);
  }

  return 0;
}

int (draw_bishop)(struct Piece* piece){
  int initialX = piece->position.x * CELL_SIZE_WIDTH + 10;
  int initialY = piece->position.y * CELL_SIZE_HEIGHT + 10;

  if(piece->isWhite){
    draw_white_piece(Chess_blt45, initialX, initialY);
  }else{
    draw_black_piece(Chess_black_blt45, initialX, initialY);
  }

  return 0;
}

int (draw_queen)(struct Piece* piece){
  
  int initialX = piece->position.x * CELL_SIZE_WIDTH + 10;
  int initialY = piece->position.y * CELL_SIZE_HEIGHT + 10;

  if(piece->isWhite){
    draw_white_piece(Chess_qlt45, initialX, initialY);
  }else{
    draw_black_piece(Chess_black_qlt45, initialX, initialY);
  }

  return 0;
}

int (draw_king)(struct Piece* piece){
  int initialX = piece->position.x * CELL_SIZE_WIDTH + 10;
  int initialY = piece->position.y * CELL_SIZE_HEIGHT + 10;

  if(piece->isWhite){
    draw_white_piece(Chess_klt45, initialX, initialY);
  }else{
    draw_black_piece(Chess_black_klt45, initialX, initialY);
  }

  return 0;
}

int draw_piece(struct Piece* piece){
  printf("Drawing Piece\n");
  printf("Piece Type: %d\n", piece->type);
  printf("Piece Position: %d %d\n", piece->position.x, piece->position.y);
  printf("Piece isAlive: %d\n", piece->isAlive);
  printf("Piece isWhite: %d\n", piece->isWhite);
  printf("Piece canMove: %d\n", piece->canMove);
  printf("Piece hasMoved: %d\n", piece->hasMoved);
  printf("Piece isSelected: %d\n", piece->isSelected);


  switch (piece->type)
  {
  case PAWN:
    draw_pawn(piece);
    break;
  case ROOK:
    draw_rook(piece);
    break;
  case KNIGHT:
    draw_knight(piece);
    break;
  case BISHOP:
    draw_bishop(piece);
    break;
  case QUEEN:
    draw_queen(piece);
    break;
  case KING:
    draw_king(piece);
    break;
  default:
    return 0;
  }

  return 0;
}

int return_to_initial_pos(struct Piece* piece, struct Position* initialPos) {
    int initialX = initialPos->x * CELL_SIZE_WIDTH + 10;
    int initialY = initialPos->y * CELL_SIZE_HEIGHT + 10;

    int currentX = piece->position.x * CELL_SIZE_WIDTH + 10;
    int currentY = piece->position.y * CELL_SIZE_HEIGHT + 10;

    if (currentX == initialX && currentY == initialY) {
        return 0; // Already at the initial position
    }

    double m = 0;
    if (currentX != initialX) {
        m = (double)(currentY - initialY) / (currentX - initialX);
    }
    double b = currentY - m * currentX;

    while (currentX != initialX || currentY != initialY) {
        printf("Current Position Before Move: %d %d\n", currentX, currentY);

        if (currentX != initialX) {
            if (currentX > initialX) {
                currentX--;
            } else {
                currentX++;
            }
        }

        if (currentX != initialX) {
            currentY = (int)(m * currentX + b);
        } else if (currentY != initialY) {
            if (currentY > initialY) {
                currentY--;
            } else {
                currentY++;
            }
        }

        printf("Current Position After Move: %d %d\n", currentX, currentY);

        if (piece->isWhite) {
            draw_white_piece(Chess_plt45, currentX, currentY);
        } else {
            draw_black_piece(Chess_black_plt45, currentX, currentY);
        }
        swap_buffers();
    }

    piece->position.x = initialPos->x;
    piece->position.y = initialPos->y;

    return 0;
}

/*int (advance_piece)(struct Piece* piece , struct Position* initialPos){
  int initialX = initialPos->x * CELL_SIZE_WIDTH + 10;
    int initialY = initialPos->y * CELL_SIZE_HEIGHT + 10;

    int currentX = piece->position.x * CELL_SIZE_WIDTH + 10;
    int currentY = piece->position.y * CELL_SIZE_HEIGHT + 10;

    if (currentX == initialX && currentY == initialY) {
        return 0; // Already at the initial position
    }

    double m = 0;
    if (currentX != initialX) {
        m = (double)(currentY - initialY) / (currentX - initialX);
    }
    double b = currentY - m * currentX;

    while (currentX != initialX || currentY != initialY) {
        printf("Current Position Before Move: %d %d\n", currentX, currentY);

        if (currentX != initialX) {
            if (currentX > initialX) {
                currentX--;
            } else {
                currentX++;
            }
        }

        if (currentX != initialX) {
            currentY = (int)(m * currentX + b);
        } else if (currentY != initialY) {
            if (currentY > initialY) {
                currentY--;
            } else {
                currentY++;
            }
        }

        printf("Current Position After Move: %d %d\n", currentX, currentY);

        if (piece->isWhite) {
            draw_white_piece(Chess_plt45, currentX, currentY);
        } else {
            draw_black_piece(Chess_black_plt45, currentX, currentY);
        }
        swap_buffers();
    }

    piece->position.x = initialPos->x;
    piece->position.y = initialPos->y;

    return 0;
}*/
