#include <machine/int86.h>
#include "lcom/lcf.h"
#include "VBE.h"

//int (vg_exit)(); dado por LCF

int (set_graphic_mode)(uint16_t mode);
int (set_frame_mode)(uint16_t* mode);
int (set_text_mode)();

int activate_double_buffer();
int deactivate_double_buffer();
int allocate_buffers();
int swap_buffer();
int erase_buffer();

int (get_h_res)();
int (get_v_res)();

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int (adjust_color)(uint32_t color, uint16_t* new_color);


int (wait_for_ESC_)();

int (fill)(int x , int y , int width , int height , uint32_t color);
int (draw)();
int (draw_board)();
