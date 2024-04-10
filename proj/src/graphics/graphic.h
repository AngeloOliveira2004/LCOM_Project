#include <machine/int86.h>
#include "lcom/lcf.h"
#include "VBE.H"



/**
 * @brief This is the base addres (pointer) to the beggining of th array where display pixels are stored
 */
//int (vg_exit)(); dado por LCF

int (set_graphic_mode)(uint16_t mode);
int (set_text_mode)();

int (get_h_res)();
int (get_v_res)();

int (set_frame_mode)(uint16_t* mode);
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y,uint16_t width, uint16_t height, uint32_t color);

int (normalize_color)(uint32_t color, uint32_t* new_color);

int (normalize_color_32_to_16)(uint32_t color, uint16_t* new_color);


int (wait_for_ESC_)();

int (fill)(int x , int y , int width , int height , uint32_t color);
int (draw)();
int (draw_board)();
