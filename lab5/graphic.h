#ifndef __GRAPHIC_H
#define __GRAPHIC_H

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

vbe_mode_info_t vmi;
uint8_t *video_mem;

int(set_mode)(uint16_t mode);

int(mapping_line_frame_buffer)(uint16_t mode);

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int(vg_draw_line)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int vg_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
#endif
