#include <lcom/lcf.h>
#include "graphic.h"
#include "VBE.h"

static uint16_t hres;
static uint16_t vres;
vbe_mode_info_t vbe_mode_info;
uint8_t* frame_buffer;

int(change_graphics_mode)(uint16_t res_mode){
  reg86_t r;

  memset(&r,0,sizeof(r)); // Para evitar unexpected behaviour , sempre que fazemos uma call no intno 0x10 , devemos limpar os registros que não foram usados com o memset

  r.ax = SET_VBE_MODE; // Colocar o valor de ax como 0x4F02 , pode ser feito com ah = 0x4F e al = 0x02

  r.intno = 0x10; //Intno é sempre igual a 10

  r.bx = res_mode | BX_LINEAR_FB; //Coloca o modo de resolução e indexação de bits no parámetro bx

  if(sys_int86(&r) != OK ) {
    printf("set_vbe_mode: sys_int86() failed \n");
   return 1;
  } 
  return 0;
}


int(change_text_mode)(){
  reg86_t r;

  memset(&r,0,sizeof(r)); // Para evitar unexpected behaviour , sempre que fazemos uma call no intno 0x10 , devemos limpar os registros que não foram usados com o memset

  r.ax = SET_TEXT_MODE; // Colocar o valor de ax como 0003 , pode ser feito com ah = 0x00 e al = 0x03

  r.intno = 0x10; //Intno é sempre igual a 10

  r.bx = 0x0000;

  if(sys_int86(&r) != OK ) {
    printf("set_text_mode: sys_int86() failed \n");
   return 1;
  } 
  return 0;
}

uint16_t get_hres() {
    return hres;
}

uint16_t get_vres() {
    return vres;
}

int(map_physical_into_virtual_ram)(){
  //PhysBasePtr: Endereço físico da entrade do frame buffer
  //XResolution: Resolução horizontal , medidada em pixels
  //YResolution: Resolução vertical , medidada em pixels
  //BitsPerPixel: Nº de bits por pixel
  //MaskSize: Número de bits da máscara
  //Field Position: Posição da máscara
  //Memory Model

  int r;
  struct minix_mem_range mr; //Range da memória física

  unsigned int vram_size = (vbe_mode_info.XResolution * vbe_mode_info.YResolution) * ((vbe_mode_info.BitsPerPixel + 7) / 8); //Garantir que o arrendondamento é feito por excesso com a soma do 7 , a primeira parcela multiplica o x pelo o y e de seguida , multiplica por o número de bits por cada pixel

  mr.mr_base = vbe_mode_info.PhysBasePtr;
  mr.mr_limit = mr.mr_base + vram_size;

  if(OK != (r = sys_privctl(SELF,SYS_PRIV_ADD_MEM,&mr))){
    panic("sys_privctl (Physical memory alocation error) failed: %d\n",r);
    return 1;
  }

  frame_buffer = vm_map_phys(SELF,(void *) mr.mr_base,vram_size);

  if(frame_buffer == NULL){
    panic("Couldn't map video memory");
    return 1;
  }  

  return 0;
}

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color){

  if(x > vbe_mode_info.XResolution || y > vbe_mode_info.YResolution){
    printf("The values %d for x and %d for y are out of bounds",x,y);
    return 1;
  }

  uint8_t *conversion = (uint8_t *) frame_buffer;

  uint8_t bits_per_pixel = ((vbe_mode_info.BitsPerPixel + 7) / 8);

  unsigned int index_position = ((vbe_mode_info.XResolution * y) + x) * bits_per_pixel;

  conversion += index_position;

  if(memcpy((void *) conversion,&color,bits_per_pixel) == NULL){
    printf("Failed to copy");
    return 1;
  }
  return 0;
}

int (draw_line)(uint16_t x, uint16_t y, uint32_t color,uint16_t width){
  for(uint16_t i = 0; i < width;i++){

    if(draw_pixel(x+i,y,color) != 0){
      printf("Failed to draw pixel");
      return 1;
    }

  }
  return 0;
}

int (draw_rectangle)(uint16_t x, uint16_t y, uint32_t color,uint16_t width,uint16_t height){
  for(uint16_t i = 0; i < height;i++){
    if(draw_line(x,y+i,color,width) != 0){
      printf("Failed to draw line");
      return 1;
    }
  }
  return 0;
}


int (draw_from_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y){
  xpm_image_t xpm_image;

  uint8_t *pixel_colors = xpm_load(xpm,XPM_INDEXED,&xpm_image);
  if(pixel_colors == NULL){
    printf("Failed to load xpm");
    return 1;
  }

  for(uint16_t height = 0; height < xpm_image.height;height++){
    for(uint16_t width = 0; width < xpm_image.width;width++){
      if(draw_pixel(x+width,y+height,*pixel_colors) != 0 ){
        return 1;
      }else{
        pixel_colors++; //Vai para a cor seguinte no xpm
      }
    }
  }

  return 0;
}

int normalize_color(uint32_t old_color, uint32_t *normalized) {
  if (vbe_mode_info.BitsPerPixel == 32) {
    *normalized = old_color;
  } else {
    int bpp = vbe_mode_info.BitsPerPixel;
    uint32_t mask = BIT(bpp) - 1;
    *normalized = old_color & mask;
  }
  return 0;
}


