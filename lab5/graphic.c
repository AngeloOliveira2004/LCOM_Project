#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include "graphic.h"
#include <stdint.h>
#include <stdio.h>

int(set_mode)(uint16_t mode) {
  reg86_t r;
  memset(&r, 0, sizeof(r));

  r.bx = BIT(14) | mode;
  r.intno = 0x10;
  r.al = 0x02;
  r.ah = 0x4F;

  if (sys_int86(&r) != OK) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }
  return 0;
}

int(mapping_line_frame_buffer)(uint16_t mode) {

  memset(&vmi, 0, sizeof(vmi));

  if (vbe_get_mode_info(mode, &vmi)) {
    return 1;
  }

  uint8_t bytes_p_pixel = (vmi.BitsPerPixel + 7) / 8;
  unsigned int frame_size = vmi.XResolution * vmi.YResolution * bytes_p_pixel;

  struct minix_mem_range mr;
  mr.mr_base = vmi.PhysBasePtr;
  mr.mr_limit = mr.mr_base + frame_size;

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != 0)
    return 1;

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, frame_size);
  if (video_mem == NULL) {
    return 1;
  }
  memset(video_mem, 0, frame_size);
  return 0;
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {

  if (x > vmi.XResolution || y > vmi.YResolution) {
    return 1;
  }
  uint8_t *conversao = (uint8_t *) video_mem;
  uint16_t bytes_p_pixel = (vmi.BitsPerPixel + 7) / 8;
  uint32_t byte_index = (vmi.XResolution * y + x) * bytes_p_pixel;
  conversao += byte_index;
  memcpy((void *) conversao, &color, bytes_p_pixel);
  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (uint16_t i = 0; i < len; i++) {
    if (vg_draw_pixel(x + i, y, color)) {
      return 1;
    }
  }
  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (uint16_t i = 0; i < height; i++) {
    if (vg_draw_hline(x, y + i, width, color)) {
      return 1;
    }
  }
  return 0;
}

int(get_color)(uint32_t *color, uint16_t col, uint16_t row, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  uint16_t bytes_p_pixel = (vmi.BitsPerPixel + 7) / 8;

  if (bytes_p_pixel == 8) {
    *color = (first + (row * no_rectangles + col) * step) % (1 << bytes_p_pixel);
  }
  else {
    uint32_t red = ((first >> (vmi.RedFieldPosition % BIT(vmi.RedMaskSize))) + col * step) % (1 << vmi.RedMaskSize);
    uint32_t green = ((first >> (vmi.GreenFieldPosition % BIT(vmi.GreenMaskSize))) + row * step) % (1 << vmi.GreenMaskSize);
    uint32_t blue = ((first >> (vmi.BlueFieldPosition % BIT(vmi.BlueMaskSize))) + (col + row) * step) % (1 << vmi.BlueMaskSize);
    *color = (red << vmi.RedFieldPosition) | (green << vmi.GreenFieldPosition) | (blue << vmi.BlueFieldPosition);
  }

  return 0;
}


int (draw_xpm) (xpm_map_t xpm, uint16_t x, uint16_t y){
  xpm_image_t image;

  uint8_t *color = xpm_load(xpm, XPM_INDEXED, &image);
  for (unsigned int i = 0; i < image.height; i++) {
    for (unsigned int j = 0; j < image.height; j++)
    {
      if (vg_draw_pixel(x + j, y + i, *color))
      {
        return 1;
      }
      color++;
    }
  }

  return 0;
}

