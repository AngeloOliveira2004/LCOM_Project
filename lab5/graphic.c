#include "graphic.h"
#include <machine/int86.h>
#include <lcom/lcf.h>
#include "kbc.h"
#include "i8042.h"
#include "VBE.H"
#include <stdint.h>
#include <stdio.h>
#include "keyboard.h"

uint8_t *drawBuffer;
unsigned bytesPerPixel;
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
  reg86.ax = 0x4F02; /* 0x4F02 */

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

  unsigned int bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
  unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;
  
  // preenchimento dos endereços físicos
  struct minix_mem_range physic_addresses;
  physic_addresses.mr_base = mode_info.PhysBasePtr; // início físico do buffer
  physic_addresses.mr_limit = physic_addresses.mr_base + frame_size; // fim físico do buffer
  
  // alocação física da memória necessária para o frame buffer
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_addresses)) {
    printf("Physical memory allocation failed\n");
    return 1;
  }

  // alocação virtual da memória necessária para o frame buffer
  drawBuffer = vm_map_phys(SELF, (void*) physic_addresses.mr_base, frame_size);
  if (drawBuffer == NULL) {
    printf("Virtual memory allocation failed\n");
    return 1;
  }

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
  unsigned BytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;

  // Índice (em bytes) da zona do píxel a colorir
  unsigned int index = (mode_info.XResolution * y + x) * BytesPerPixel;

  // A partir da zona frame_buffer[index], copia @BytesPerPixel bytes da @color
  if (memcpy(&drawBuffer[index], &color, BytesPerPixel) == NULL) return 1;

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
      vg_exit();
      return 1;
    }

  return 0;
}

int (normalize_color)(uint32_t color, uint32_t* new_color){

  if (mode_info.BitsPerPixel == 32) {
    *new_color = color;
  } else {
    *new_color = color & (BIT(mode_info.BitsPerPixel) - 1);
  }
  return 0;

}

int (wait_for_ESC_)(){

  if(wait_for_ESC() != OK){
    return 1;
  }

  return 0;
}
