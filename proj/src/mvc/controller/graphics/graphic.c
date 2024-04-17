#include "graphic.h"
#include <machine/int86.h>
#include <lcom/lcf.h>
#include "../keyboard/kbc.h"
#include "../keyboard/i8042.h"
#include <stdint.h>
#include <stdio.h>
#include "../keyboard/keyboard.h"

uint8_t *frontBuffer; // The front buffer
uint8_t *backBuffer;  // The back buffer
uint16_t bufferSize;

unsigned bytesPerPixel = -1;
uint16_t xRes, yRes;

vbe_mode_info_t mode_info;
uint8_t* vAddr_base;

/*
Parâmetro reg86_t 	Text Mode 	Graphic Mode
ah 	                  0x00 	        0x4F
al 	                  Ox03 	        0x02
ax 	                  0x0003 	      0x4F02
bx 	                  0x0000 	submode | BIT(14)
intno 	              0x10 	        0x10

*/
int (set_graphic_mode)(uint16_t mode){
  reg86_t reg86;

  memset(&reg86, 0, sizeof(reg86));

  reg86.intno = 0x10; /* BIOS video services always ten*/
  reg86.ah = 0x4F; /* Set Video Mode */
  reg86.al = 0x02; /* Mode 0x105 */
  //reg86.ax = 0x4F02; /* 0x4F02 */

  /**
   * Sets the mode of the reg86.bx register with the given mode value.
   * The mode value is combined with the BIT(14) flag using the bitwise OR operator.
   *
   * @param mode The mode value to set.
   */
   reg86.bx = mode | BIT(14);

  if (sys_int86(&reg86) != OK) {
    printf("set_mode: sys_int86() failed \n");
    return 1;
  }

  return 0;

}


int (set_text_mode)(){
  reg86_t reg86;

  memset(&reg86, 0, sizeof(reg86));

  reg86.intno = 0x10; /* BIOS video services always ten*/
  reg86.ah = 0x00; /* Set Video Mode */
  reg86.al = 0x03; /* Mode 0x105 */
  reg86.ax = 0x0003; /* 0x4F02 */

  if (sys_int86(&reg86) != OK) {
    printf("set_mode: sys_int86() failed \n");
    return 1;
  }

  return 0;
}

int (set_frame_mode)(uint16_t* mode){
  
  memset(&mode_info, 0, sizeof(mode_info));
  if(vbe_get_mode_info(*mode, &mode_info)){
    return 1;
  }

  unsigned int bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
  unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * bytesPerPixel;

  int r;

  // preenchimento dos endereços físicos
  struct minix_mem_range physic_addresses;
  physic_addresses.mr_base = mode_info.PhysBasePtr; // início físico do buffer
  physic_addresses.mr_limit = physic_addresses.mr_base + frame_size; // fim físico do buffer
  
  // alocação física da memória necessária para o frame buffer
  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_addresses)))
   panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  // alocação virtual da memória necessária para o frame buffer
  frontBuffer = vm_map_phys(SELF, (void*) physic_addresses.mr_base, frame_size);

  if(frontBuffer == MAP_FAILED)
   panic("couldn't map video memory");

  return 0;

}

int (get_h_res)(){
  return mode_info.XResolution;
}
int (get_v_res)(){
  return mode_info.YResolution;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
  
  // As coordenadas têm de ser válidas
  if(x > mode_info.XResolution || y > mode_info.YResolution) return 1;
  
  // Cálculo dos Bytes per pixel da cor escolhida. Arredondamento por excesso.
  unsigned bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;

  // Índice (em bytes) da zona do píxel a colorir
  unsigned int index = (mode_info.XResolution * y + x) * bytesPerPixel;

  // A partir da zona frame_buffer[index], copia @BytesPerPixel bytes da @color
  if (memcpy(&frontBuffer[index], &color, bytesPerPixel) == NULL) return 1;

  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){

  for(int i = 0 ; i < len; i++){
    if(vg_draw_pixel(x+i, y, color) != 0){
      return 1;
    }
  }

  return 0;
}

int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){

  for(unsigned   i = 0 ; i < len; i++){
    if(vg_draw_pixel(x, y+i, color) != 0){
      return 1;
    }
  }

  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y,uint16_t width, uint16_t height, uint32_t color){
  
  for(unsigned i = 0; i < height ; i++)
    if (vg_draw_hline(x, y+i, width, color) != 0) {
      return 1;
    }

  return 0;
}

int (adjust_color)(uint32_t color, uint16_t* new_color){

  if (mode_info.BitsPerPixel == 32 || mode_info.BitsPerPixel == 3 || mode_info.BitsPerPixel == 1) {
    *new_color = color;  
    return 0;
  } 

  uint8_t redMask = (color >> 16) & 0xFF;
  redMask <<= (8 - mode_info.RedMaskSize);
  redMask >>= mode_info.RedMaskSize;

  uint8_t greenMask = (color >> 8) & 0xFF;
  greenMask <<= (8 - mode_info.RedMaskSize);
  greenMask >>= mode_info.RedMaskSize;

  uint8_t blueMask = color & 0xFF;
  blueMask <<= (8 - mode_info.RedMaskSize);
  blueMask >>= mode_info.RedMaskSize;

  *new_color = (mode_info.RedFieldPosition << redMask) | (mode_info.RedFieldPosition << greenMask) | (mode_info.RedFieldPosition << blueMask);

  return 0;

}

int (wait_for_ESC_)(){

  if(wait_for_ESC() != OK){
    return 1;
  }

  return 0;
}

int (draw)(){

  uint16_t mode = VBE_800x600_DC;

  if(set_graphic_mode(mode) != 0 )
    return 1;

  if(set_frame_mode(&mode) != 0 )
      return 1;

  if(draw_board() != 0)
    return 1;

  if(wait_for_ESC_() != 0)
    return 1;
  
  if(vg_exit() != 0){
    printf("video_test_init: vg_exit() failed\n");
    return 1;
  }

  return 0;
}

int (draw_board)(){

  uint32_t bege = 0xf5f5dc;
  uint32_t brown = 0x964B00;

  uint16_t bege_16;
  uint16_t brown_16;

  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if((i+j) % 2 == 0){
        if(vg_draw_rectangle(i*CELL_SIZE_WIDTH, j*CELL_SIZE_HEIGHT, CELL_SIZE_WIDTH, CELL_SIZE_HEIGHT, bege_16) != 0){
          return 1;
        }
      }else{
        if(vg_draw_rectangle(i*CELL_SIZE_WIDTH, j*CELL_SIZE_HEIGHT, CELL_SIZE_WIDTH, CELL_SIZE_HEIGHT, brown_16) != 0){
          return 1;
        }
      }
    }
  }

  return 0;
}

int activate_double_buffer() {

    if (allocate_buffers() != 0)
        return 1;

    return 0;
}

int erase_buffer() {
    memset(backBuffer, 0, bufferSize);

    return 0;
}

int swap_buffer() {

    uint8_t *temp = frontBuffer;
    frontBuffer = backBuffer;
    backBuffer = temp;

    memcpy(vAddr_base, frontBuffer, bufferSize);

    return 0;
}

int allocate_buffers(){

  if(bytesPerPixel == -1){
    bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
  }

  bufferSize = mode_info.XResolution * mode_info.YResolution * bytesPerPixel;
  
  // Allocate the front buffer
  frontBuffer = (uint8_t*) malloc(bufferSize);
  if(frontBuffer == NULL){
    printf("Error: Failed to allocate memory for the front buffer\n");
    return 1;
  }

  // Allocate the back buffer
  backBuffer = (uint8_t*) malloc(bufferSize);
  if(backBuffer == NULL){
    printf("Error: Failed to allocate memory for the back  buffer\n");
    return 1;
  }

  return 0;
}