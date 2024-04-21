#include <lcom/lcf.h>
#include "graphic.h"
#include "VBE.h"

int(change_graphics_mode)(uint16_t res_mode){
  reg86_t r;

  memset(&r,0,sizeof(r)); // Para evitar unexpected behaviour , sempre que fazemos uma call no intno 0x10 , devemos limpar os registros que não foram usados com o memset

  r.ax = SET_VBE_MODE; // Colocar o valor de ax como 0x4F02 , pode ser feito com ah = 0x4F e al = 0x02

  r.intno = 0x10; //Intno é sempre igual a 10

  r.bx = res_mode | BX_LINEAR_FB;

  if(sys_int86(&r) != OK ) {
    printf("set_vbe_mode: sys_int86() failed \n");
   return 1;
  } 
  return 0;
}
