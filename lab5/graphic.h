#ifndef GRAPHIC_H
#define GRAPHIC_H


int(change_graphics_mode)(uint16_t res_mode);
int(change_text_mode)();
uint16_t get_hres();
uint16_t get_vres();
int(map_physical_into_virtual_ram)();
int draw_pixel(uint16_t x, uint16_t y, uint32_t color);
int draw_line(uint16_t x, uint16_t y, uint32_t color,uint16_t width);
int draw_rectangle(uint16_t x, uint16_t y, uint32_t color,uint16_t width,uint16_t height);
int draw_from_xpm(xpm_map_t xpm, uint16_t x, uint16_t y);
int normalize_color(uint32_t old_color, uint32_t *normalized);



#endif
