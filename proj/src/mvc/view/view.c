#include "../controller/graphics/graphic.h"
#include "view.h"

int initialize_graphics(uint16_t* mode){

  if(allocate_buffers() != 0)
    return 1;
  
  if(set_graphic_mode(mode) != 0 )
    return 1;

  if(set_frame_mode(mode) != 0 )
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

int (draw_board)(){

  uint32_t bege = 0xf5f5dc;
  uint32_t brown = 0x964B00;

  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if((i+j) % 2 == 0){
        if(vg_draw_rectangle(i*CELL_SIZE_WIDTH, j*CELL_SIZE_HEIGHT, CELL_SIZE_WIDTH, CELL_SIZE_HEIGHT, bege) != 0){
          return 1;
        }
      }else{
        if(vg_draw_rectangle(i*CELL_SIZE_WIDTH, j*CELL_SIZE_HEIGHT, CELL_SIZE_WIDTH, CELL_SIZE_HEIGHT, brown) != 0){
          return 1;
        }
      }
    }
  }

  return 0;
}

int draw_piece(){
  return 0;
}

int draw_initial_pos(){
  return 0;
}

int draw(){
  erase_buffer();

  if(draw_board() != 0)
    return 1;
  
  swap_buffers();

  return 0;
}
