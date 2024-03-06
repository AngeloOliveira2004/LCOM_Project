#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"
#include "i8042.h"

extern uint8_t scancode;
extern uint32_t cnt;

bool valid = true;

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

  if(keyboard_subscribe_int(&irq_set) != 0){
    return 1;
  }

  while(valid) { 
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    uint8_t bytes[2];
    uint8_t counter = 0;
    bool make_code = false;

    if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE:			
                if (msg.m_notify.interrupts & BIT(irq_set)) { 
                  kbc_ih();
                  memset(bytes, 0, sizeof(bytes));

                  if(scancode == ESC_BREAK_CODE){
                    valid = false;
                    make_code = false;
                    counter = 1;
                    bytes[0] = ESC_BREAK_CODE;
                  }else{

                    if(scancode == 0xE0){ 
                      bytes[0] = scancode;
                      counter++;
                      read_out_buffer(&scancode);
                    }

                    if((scancode >> 7) & 1){ //Vê se o bit msb do scancode é break code (1)
                      make_code = false;
                    }else{
                      make_code = true;
                    }

                    bytes[counter] = scancode; //Adiciona ao index de byte 0 ou 1 dependendo do   número de bytes do scancode
                    counter++; //Incrementa o counter para ser passado para a print_scancode
                  }

                  kbd_print_scancode(make_code,counter,bytes); // Dá print ao valor do scancode

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
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
