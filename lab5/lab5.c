// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "graphic.h"
#include "i8042.h"
#include "Keyboard.h"
#include "i8254.h"
#include "timer.c"

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

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height,
uint32_t color) {
  if (mapping_line_frame_buffer(mode)!=0)
  {
    return 1;
  }
  if (set_mode(mode) != 0) {
    return 1;
  }
  if (vg_draw_rectangle(x, y, width, height, color) != 0) {
    return 1;
  }
  uint8_t irq_set;
  if (kbc_si(&irq_set)) {
    return 1;
  }
  int ipc_status, r;

  message msg;

  while(scancode != ESC) { /* You may want to use a different condition */
     /* Get a request message. */
     if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
       printf("driver_receive failed with: %d", r);
       continue;
     }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */				
            if (msg.m_notify.interrupts & BIT(irq_set)) { /* subscribed interrupt */
              kbc_ih();                                    /* process it */
              
            }
            break;
          default:
            break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
 }
 kbc_ui();
 if (vg_exit() != 0) {
    return 1;
  }
 
  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {

  if (mapping_line_frame_buffer(mode)!=0)
  {
    return 1;
  }
 
  if (set_mode(mode)) {
    return 1;
  }
 

  unsigned int hori = vmi.XResolution / no_rectangles;
  unsigned int verti = vmi.YResolution / no_rectangles;

  uint32_t color = first;

  for (unsigned int i = 0; i < no_rectangles; i++)
  {
   
    for (unsigned int j = 0; j < no_rectangles; j++)
    {
 
      get_color(&color, j, i, no_rectangles, first, step);
      vg_draw_rectangle(j * hori, i * verti, hori, verti, color);
    }
  }
  uint8_t irq_set;
  if (kbc_si(&irq_set)) {
    return 1;
  }
  int ipc_status, r;

  message msg;

  while(scancode != ESC) { /* You may want to use a different condition */
     /* Get a request message. */
     if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
       printf("driver_receive failed with: %d", r);
       continue;
     }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */				
            if (msg.m_notify.interrupts & BIT(irq_set)) { /* subscribed interrupt */
              kbc_ih();                                    /* process it */
              
            }
            break;
          default:
            break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
 }
 kbc_ui();
 
 if(vg_exit() != 0){
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

   uint8_t irq_set;
  if (kbc_si(&irq_set)) {
    return 1;
  }
  int ipc_status, r;

  message msg;

  while(scancode != ESC) { /* You may want to use a different condition */
     /* Get a request message. */
     if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
       printf("driver_receive failed with: %d", r);
       continue;
     }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */				
            if (msg.m_notify.interrupts & BIT(irq_set)) { /* subscribed interrupt */
              kbc_ih();                                    /* process it */
              
            }
            break;
          default:
            break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
 }
 kbc_ui();
 
 if(vg_exit() != 0){
   return 1;
 }

  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,int16_t speed, uint8_t fr_rate) {
  uint8_t irq_set_keyborad;
  uint8_t irq_set_timer;

  bool movimento = false;

  if (xi < xf && yi == yf)
  {
    movimento = true;
  }
  else if (xi == xf && yi < yf)
  {
    movimento = false;
  }
  else{
    return 1;
  }

  if (timer_subscribe_int(&irq_set_timer)){
    return 1;
  }   
  if (kbc_si(&irq_set_keyborad)) {
    return 1;
  }
  if (timer_set_frequency(0,fr_rate)!=0)
  {
    return 1;
  }

  

  if (mapping_line_frame_buffer(0x105)!=0)
  {
    return 1;
  }

  if (set_mode(0x105) != 0)
  {
    return 1;
  }

  if (draw_xpm(xpm, xi, yi) != 0)
  {
    return 1;
  }

  int ipc_status, r;
  message msg;
 
  while(scancode != ESC && (xi < xf || yi < yf)) { /* You may want to use a different condition */
     /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */
          	if (msg.m_notify.interrupts & BIT(irq_set_keyborad)) { /* subscribed interrupt */
              kbc_ih();                                    /* process it */
            }
            if (msg.m_notify.interrupts & BIT(irq_set_timer)) { /* subscribed interrupt */
              vg_draw_rectangle(xi, yi, 100, 100, 0x0000);
              if (movimento) {
                xi += speed;
                if (xi > xf) {
                  xi = xf;

                }
                else {
                  yi += speed;
                  if (yi > yf) {
                    yi = yf;
                  }
                }
                
                if (draw_xpm(xpm, xi, yi) != 0) {
                  return 1;
                }
                printf("x = %d\n", xi);
              } /* process it */
            }
            break;
          default:
            break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
 }
  if(timer_unsubscribe_int()){
    return 1;
  }
  kbc_ui();

  if(vg_exit() != 0){
   return 1;
 }
  
  return 0;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}

