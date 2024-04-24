// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <VBE.h>
#include <graphic.h>
#include <i8042.h>
#include <i8254.h>
#include <kbc.h>
#include <keyboard.h>
#include <lcom/timer.h>
#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

extern uint8_t scancode;
extern struct scancode_info scan_info;
extern vbe_mode_info_t vbe_mode_info;

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

int(video_test_init)(uint16_t mode, uint8_t delay) {

  if (change_graphics_mode(mode) != 0) { // Muda para graphics mode com resolução dada no mode
    return 1;
  }

  sleep(delay); // Espera o tempo delay antes de sair do modo

  if (vg_exit() != 0) { // Volta para o modo texto
    return 1;
  }

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

  // Perguntar porque quando eu uso o 0x105, o rectângulo não desenha mas passa o teste

  int ipc_status, r;
  message msg;
  bool valid = true;

  uint8_t irq_set;

  if (keyboard_subscribe_int(&irq_set) != 0) {
    return 1;
  }

  memset(&vbe_mode_info, 0, sizeof(vbe_mode_info)); // Para evitar unexpected behaviour , devemos limpar os registros que não foram usados com o memset

  if (vbe_get_mode_info(mode, &vbe_mode_info) != 0) {
    return 1;
  }

  if (map_physical_into_virtual_ram() != 0) {
    return 1;
  }

  if (change_graphics_mode(mode) != 0) { // Muda para graphics mode com resolução dada no mode
    return 1;
  }

  uint32_t normalized_color;
  if (normalize_color(color, &normalized_color) != 0) {
    return 1;
    
  }

  if (draw_rectangle(x, y, normalized_color, width, height) != 0) {
    return 1;
  }

  while (valid) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(irq_set)) {
            kbc_ih();
            tickdelay(micros_to_ticks(DELAY_US));

            if (!receive_keyboard_scan(&scan_info, &scancode)) {
              valid = false;
            }
          }
          break;
      }
    }
  }

  if (keyboard_unsubscribe_int() != 0) {
    return 1;
  }

  if (vg_exit() != 0) { // Volta para o modo texto
    return 1;
  }

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  /* To be completed */
  printf("%s(0x%03x, %u, 0x%08x, %d): under construction\n", __func__,
         mode, no_rectangles, first, step);

  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

  int ipc_status, r;
  message msg;
  bool valid = true;

  uint8_t irq_set;

  if (keyboard_subscribe_int(&irq_set) != 0) {
    return 1;
  }

  memset(&vbe_mode_info, 0, sizeof(vbe_mode_info)); // Para evitar unexpected behaviour , devemos limpar os registros que não foram usados com o memset

  if (vbe_get_mode_info(VBE_INDEXED_768p, &vbe_mode_info) != 0) {
    return 1;
  }

  if (map_physical_into_virtual_ram() != 0) {
    return 1;
  }

  if (change_graphics_mode(VBE_INDEXED_768p) != 0) { // Muda para graphics mode 0x105, o suportador pelo xpm
    return 1;
  }

  if (draw_from_xpm(xpm, x, y) != 0) {
    return 1;
  }

  while (valid) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(irq_set)) {
            kbc_ih();
            tickdelay(micros_to_ticks(DELAY_US));

            if (!receive_keyboard_scan(&scan_info, &scancode)) {
              valid = false;
            }
          }
          break;
      }
    }
  }

  if (vg_exit() != 0) { // Volta para o modo texto
    return 1;
  }

  if (keyboard_unsubscribe_int() != 0) {
    return 1;
  }

  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {

  int ipc_status, r;
  message msg;
  bool valid = true;

  uint8_t irq_set_keyboard, irq_set_timer;
  bool movement = false; // movement false = vertical , true = horizontal

  if (xi < xf && yi == yf) {
    movement = true; // O movimento é na horizontal
  }
  else if (xi == xf && yi < yf) {
    movement = false; // O movimento é na vertical
  }
  else {
    return 1; // Movimento não é vertical nem horizontal
  }

  if(fr_rate < 19){
    printf("Frame-rate overflow");
    return 1;
  }

  if (timer_subscribe_int(&irq_set_timer) != 0) {
    return 1;
  }

  if (keyboard_subscribe_int(&irq_set_keyboard) != 0) {
    return 1;
  }

  if (timer_set_frequency(0, fr_rate) != 0) {
    return 1;
  }

  memset(&vbe_mode_info, 0, sizeof(vbe_mode_info)); // Para evitar unexpected behaviour , devemos limpar os registros que não foram usados com o memset

  if (vbe_get_mode_info(VBE_INDEXED_768p, &vbe_mode_info) != 0) {
    return 1;
  }

  if (map_physical_into_virtual_ram() != 0) {
    return 1;
  }

  if (change_graphics_mode(VBE_INDEXED_768p) != 0) { // Muda para graphics mode 0x105, o suportador pelo xpm
    return 1;
  }

  if (draw_from_xpm(xpm, xi, yi) != 0) {
    return 1; // Desenha na primeira posição
  }

  while (valid) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(irq_set_keyboard)) {
            kbc_ih();
            tickdelay(micros_to_ticks(DELAY_US));

            if (!receive_keyboard_scan(&scan_info, &scancode)) {
              valid = false;
            }
          }
          if (msg.m_notify.interrupts & BIT(irq_set_timer)) {
            if (movement) {
              if (xi >= xf) {
                xi = xf;
                valid = false;
              }
              else {
                xi += speed;
              }
            }else {
              if (yi >= yf) {
                yi = yf;
                valid = false;
              }
              else {
                yi += speed;
              }
            }

            if (draw_from_xpm(xpm, xi, yi) != 0) {
              return 1; // Desenha na posição seguinte
            }
          }
          break;
      }
    }
  }

  if (vg_exit() != 0) { // Volta para o modo texto
    return 1;
  }

  if (timer_unsubscribe_int() != 0) {
    return 1;
  }

  if (keyboard_unsubscribe_int() != 0) {
    return 1;
  }

  return 0;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
