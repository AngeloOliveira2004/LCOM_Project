#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "Keyboard.h"
#include "i8042.h"


extern  uint8_t scancode ;
extern uint32_t cnt;

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
  uint8_t irq_set;
  if (kbc_si(&irq_set)) {
    return 1;
  }
  int ipc_status, r;
  
  message msg;

  while(scancode != ESC) { /* You may want to use a different condition */
     /* Get a request message. */
  
    uint8_t ar[2];
    bool segundoByte = false;
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */				
            if (msg.m_notify.interrupts & BIT(irq_set)) { /* subscribed interrupt */
              kbc_ih();                                    /* process it */            
              if (segundoByte) {
                ar[1] = scancode;
                kbd_print_scancode(!(scancode & BIT(7)), 2, ar);

              }
              else if (scancode != 0xe0)
              {
                ar[0] = scancode;
                kbd_print_scancode(!((scancode >> 7) & 1), 1, ar);
              }
               else if (scancode == 0xe0) //2 bytes
              {
            
                ar[0] = 0xe0;
                segundoByte = true;
              }
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
 if (kbd_print_no_sysinb(cnt) != 0) {
   return 1;
 }

 return 0;
}

int(kbd_test_poll)() {
  uint8_t st;
  while (1) {
    read_status_reg(st);
    if ()
    {
      /* code */
    }
    
  }

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
