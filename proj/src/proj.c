// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "draw_xpm.h"
#include "pieces.xpm"



extern vbe_mode_info_t vmi;
uint8_t scancode;
uint32_t count;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g1/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g1/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(proj_main_loop)(int UNUSED(argc), char *UNUSED(argv[]))
{
  mapping_line_frame_buffer(0x118);
  set_mode(0x118);
  draw_xpm(Chess_blt45, 20, 20);
  draw_xpm(Chess_klt45, 70, 70);
  draw_xpm(Chess_nlt45, 120, 120);
  draw_xpm(Chess_plt45, 170, 170);
  draw_xpm(Chess_qlt45, 220, 220);
  draw_xpm(Chess_rlt45, 270, 270);
  draw_xpm(Chess_qltb45, 320, 320);
  sleep(3);

  vg_exit();

  return 0;

}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if (set_mode(mode) != 0)
  {
    return 1;
  }
  sleep(delay);
  if (vg_exit() != 0) {
    return 1;
  }

  return 0;
}





int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

  if (mapping_line_frame_buffer(0x105)!=0)
  {
    return 1;
  }
 
  if (set_mode(0x105)) {
    return 1;
  }
 

  if (draw_xpm(xpm,x,y))
  {
    return 1;
  }
 

 if(vg_exit() != 0){
   return 1;
 }

  return 0;
}
