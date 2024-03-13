#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"
#include "i8042.h"

extern uint8_t scancode;
extern uint32_t cnt;
extern struct scancode_info scan_info;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g1/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g1/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  // Test to be able to read the scancodes from the KBC using an interrupt handler (IH)
  // Displays the scancodes received from the keyboard using interrupts.
  // Exits upon release of the ESC key (0x81)

  int ipc_status,r;
  message msg;

  uint8_t irq_set;
  bool valid = true;
  if(keyboard_subscribe_int(&irq_set) != 0){
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
                  clean_scan_info(&scan_info);

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

  if(kbd_print_no_sysinb(cnt) != 0){
    return 1;
  }

  return 0;
}

int(kbd_test_poll)() {
  uint8_t st;
  bool valid = true;
  while(valid){
    read_status_register(&st); //Fica sempre a ler o st register
    if(test_status_polling(st)){ // Valida o valor para ver se o AUX está desativado e OUT_BUFF está cheio
      read_out_buffer(&scancode); //Lê o valor do buffer
      clean_scan_info(&scan_info); //Limpa todos os valores que se encontravam antes na struct
    }

    if(!receive_keyboard_scan(&scan_info,&scancode))
      valid = false;
  }

  if(kbd_print_no_sysinb(cnt) != 0){
    return 1;
  }

  if(kbc_enable_interrupts() != 0){
    return 1;
  }

  return 0;
}

extern int counter;

int(kbd_test_timed_scan)(uint8_t n) {

  //stops when either the ESC_KEY is released or when X seconds passed
  int ipc_status,r;
  message msg;

  uint8_t irq_set_timer;
  uint8_t irq_set_keyboard;

  uint8_t timer = n;
  //uint n seconds

  timer_subscribe_int(&irq_set_timer);
  keyboard_subscribe_int(&irq_set_keyboard);

  bool valid = true;

  while(valid) { 
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE:			
                if (msg.m_notify.interrupts & BIT(irq_set_keyboard)) { 

                  kbc_ih();
                  clean_scan_info(&scan_info);

                  if(!receive_keyboard_scan(&scan_info,&scancode)){
                    valid = false;
                  }
                  timer = n;
                }
                if(msg.m_notify.interrupts & BIT(irq_set_timer)){
                  timer_int_handler();
                  if(counter % 60 == 0){
                    timer--;
                  }

                  if(timer == 0)
                    valid = false;
                }
                break;
              default:
                break;
            }
    }
 } 
  
  timer_unsubscribe_int();
  keyboard_unsubscribe_int();

  return 1;
}
