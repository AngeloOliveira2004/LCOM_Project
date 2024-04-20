// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "i8042.h"
#include "mouse.h"
#include <lcom/timer.h>

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/g1/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g1/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

extern struct packet mouse;
extern int counter;
int counter_packet_print = 0;
extern int counter_timer;


int (mouse_test_packet)(uint32_t cnt) {

  int ipc_status,r;
  message msg;

  uint8_t irq_set;

  if(enable_mouse_report() != 0){
    return 1;
  }

  if(mouse_subscribe_int(&irq_set) != 0){ //Não conseguiu subscrever ao interrupt
    return 1;
  }

  while(cnt) { 
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:			
          if (msg.m_notify.interrupts & BIT(irq_set)) { 
            mouse_ih();
            if(counter_packet_print % 3 == 0){
              cnt--;
            }
          }
        break;
      }
    }
  }

  if(mouse_unsubscribe_int() != 0){
    return 1;
  }

  if(disable_mouse_report() != 0){
    return 1;
  }

  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    
  int ipc_status,r;
  message msg;

  uint8_t irq_set_mouse,irq_set_timer;

  uint8_t idle_time_track = idle_time;

  uint32_t frequency = sys_hz(); // Kernel call que permite ao sistema ir buscar a frequência do timer

  if(enable_mouse_report() != 0){
    return 1;
  }

  if(mouse_subscribe_int(&irq_set_mouse) != 0){
    return 1;
  }

  if(timer_subscribe_int(&irq_set_timer) != 0){
    return 1;
  }

  while(idle_time_track) { 
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:			
          if (msg.m_notify.interrupts & BIT(irq_set_mouse)) { 
            mouse_ih();
            idle_time_track = idle_time; // Colocar o valor do tracker igual ao idle_time fornecido , para reiniciar a contagem do while loop
            counter_timer = 0; // Reset o counter para não guardar informação anterior
          }
          if (msg.m_notify.interrupts & BIT(irq_set_timer)) { 
            timer_int_handler(); // Incrementa o counter_timer
            if(counter_timer % frequency == 0){
              idle_time_track--;
            }
          }
        break;
      }
    }
  }

  if(timer_unsubscribe_int() != 0){
    return 1;
  }

  if(mouse_unsubscribe_int() != 0){
    return 1;
  }

  if(disable_mouse_report() != 0){
    return 1;
  }

    return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
