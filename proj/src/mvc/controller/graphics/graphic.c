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
uint8_t *activeBuffer; // The active buffer
uint32_t bufferSize;

unsigned bytesPerPixel = -1;
uint16_t xRes, yRes;

vbe_mode_info_t mode_info;

/*
Parâmetro reg86_t 	Text Mode 	Graphic Mode
ah 	                  0x00 	        0x4F
al 	                  Ox03 	        0x02
ax 	                  0x0003 	      0x4F02
bx 	                  0x0000 	submode | BIT(14)
intno 	              0x10 	        0x10

*/
int (set_graphic_mode)(uint16_t* mode){
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
   reg86.bx = *mode | BIT(14);

  if (sys_int86(&reg86) != OK) {
    printf("set_mode: sys_int86() failed \n");
    return 1;
  }

  return 0;

}


int (set_text_mode)(){
  reg86_t reg86;

  memset(&reg86, 0, sizeof(reg86));

  reg86.intno = 0x10; /* set_texBIOS video services always ten*/
  reg86.ah = 0x00; /* Set Video Mode */
  reg86.al = 0x03; /* Mode 0x105 */
  reg86.ax = 0x0003; /* 0x4F02 */

  if (sys_int86(&reg86) != OK) {
    printf("set_mode: sys_int86() failed \n");
    return 1;
  }

  printf("Text mode setted\n");
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
  physic_addresses.mr_base = mode_info.PhysBasePtr;
  physic_addresses.mr_limit = physic_addresses.mr_base + frame_size; 
  
  // alocação física da memória necessária para o frame buffer
  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_addresses)))
   panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  // alocação virtual da memória necessária para o frame buffer
  frontBuffer = vm_map_phys(SELF, (void*) physic_addresses.mr_base, frame_size);

  printf("front buffer size : %d\n", frame_size);

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
  
  if(x > mode_info.XResolution || y > mode_info.YResolution) return 1;
  
  bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
  
  unsigned int index = (mode_info.XResolution * y + x) * bytesPerPixel;

  //printf("colouring on %d %d\n", x, y);
  if (memcpy(&backBuffer[index], &color, bytesPerPixel) == NULL) return 1;
  //printf("coloured on %d %d\n", x, y);
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

  if (mode_info.BitsPerPixel == 32 ) {
    *new_color = color;  
    return 0;
  }else{
    uint32_t mask = BIT(mode_info.BitsPerPixel) -1;
    *new_color = color & mask;
  }

  return 0;
}


void erase_buffer() {
  printf("Erasing Buffer\n");
    memset(backBuffer, 0, bufferSize);
}

void swap_buffers() {
    bufferSize = mode_info.XResolution * mode_info.YResolution * bytesPerPixel;
    memcpy(frontBuffer, backBuffer, bufferSize);
    printf("Buffers Swapped\n");
}

int allocate_buffers(){

  bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
  

  bufferSize = mode_info.XResolution * mode_info.YResolution * bytesPerPixel;
  
  // Allocate the back buffer
  backBuffer = (uint8_t*) malloc(bufferSize);
  printf("XResolution : %d\n", mode_info.XResolution);  
  printf("YResolution : %d\n", mode_info.YResolution);
  printf("Bytes per pixel : %d\n", bytesPerPixel);

  printf("Back buffer size : %d\n", bufferSize);
  if(backBuffer == NULL){
    printf("Error: Failed to allocate memory for the back  buffer\n");
    return 1;
  }

  return 0;
}
