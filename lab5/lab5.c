// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include <graphic.h>
#include <keyboard.h>
#include <kbc.h>
#include <i8042.h>

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


  if(change_graphics_mode(mode) != 0){ // Muda para graphics mode com resolução dada no mode
    return 1;
  }


  sleep(delay); //Espera o tempo delay antes de sair do modo



  if(vg_exit() != 0){ //Volta para o modo texto
    return 1;
  }
  

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
                            
  //Perguntar porque quando eu uso o 0x105, o rectângulo não desenha mas passa o teste
  
  int ipc_status,r;
  message msg;
  bool valid = true;

  uint8_t irq_set;

  if(keyboard_subscribe_int(&irq_set) != 0){
    return 1;
  }

  memset(&vbe_mode_info,0,sizeof(vbe_mode_info)); // Para evitar unexpected behaviour , devemos limpar os registros que não foram usados com o memset

  if(vbe_get_mode_info(mode,&vbe_mode_info) != 0){
    return 1;
  }

  if(map_physical_into_virtual_ram() != 0){
    return 1;
  }

  if(change_graphics_mode(mode) != 0){ // Muda para graphics mode com resolução dada no mode
    return 1;
  }

  if(draw_rectangle(x,y,color,width,height) != 0){
    return 1;
  }

  while(valid) { 
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE:			
                if (msg.m_notify.interrupts & BIT(irq_set)) { 
                  kbc_ih();
                  tickdelay(micros_to_ticks(DELAY_US));

                  if(!receive_keyboard_scan(&scan_info,&scancode)){
                    valid = false;
                  }
                }
                break;
            }
    }
 }

  if(keyboard_unsubscribe_int() != 0){
    return 1;
  }   

  if(vg_exit() != 0){ //Volta para o modo texto
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
  /* To be completed */
  printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);

  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
