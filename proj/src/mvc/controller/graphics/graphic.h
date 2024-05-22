#include <machine/int86.h>
#include "lcom/lcf.h"
#include "VBE.h"

//int (vg_exit)(); dado por LCF

int (set_graphic_mode)(uint16_t* mode);
int (set_frame_mode)(uint16_t* mode);
int (set_text_mode)();


int allocate_buffers();
void swap_buffers();
void erase_buffer();

int (get_h_res)();
int (get_v_res)();

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (adjust_color)(uint32_t color, uint16_t* new_color);

int (draw_xpm) (xpm_map_t img, uint16_t x, uint16_t y);

int (wait_for_ESC_)();


