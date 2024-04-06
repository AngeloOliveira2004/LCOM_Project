#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "Keyboard.h"
#include "kbc.c"
#include "i8042.h"
#include "timer.c"


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
  uint8_t st;
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
              read_status_reg(&st);
              if (Check_oub_full(st))
              {
                tickdelay(micros_to_ticks(DELAY_US));

              }
              
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
  uint8_t command_byte;
  uint8_t ar[2];
  bool segundoByte = false;
  while (scancode != ESC)
  {
    read_status_reg(&st);
    if(Check_oub_full(st)){
      read_out_buf(&scancode);
      if (!(st & ERROR_KEYB_KBC))
      {
        if (segundoByte) {
          ar[1] = scancode;
          kbd_print_scancode(!(scancode & BIT(7)), 2, ar);
        }
        else if (scancode != 0xe0)
        {
          ar[0] = scancode;
          kbd_print_scancode(!((scancode >> 7) & 1), 1, ar);
        }
        else if (scancode == 0xe0) 
        {
          ar[0] = 0xe0;
          segundoByte = true;
        }
      }
    }
  }
  if (kbd_print_no_sysinb(cnt) != 0) {
   return 1;
  }
  if (write_command(READ_COMMAND)) return 1;
  if(write_command_return(&command_byte)) return 1;
  command_byte = command_byte | BIT(0);
  command_byte = command_byte | BIT(6);

  if (write_command(WRITE_COMMAND))
    return 1;
  if (write_command_argumments(&command_byte))
    return 1;
  
  return 0;
}


int(kbd_test_timed_scan)(uint8_t n) {
  uint8_t irq_set_timer;
  uint8_t irq_set_kbd;
  uint8_t time = 0;
  bool segundoByte = false;
  uint8_t ar[2];
  uint8_t st;

  if(kbc_si(&irq_set_kbd))
    return 1;
  if (timer_subscribe_int(&irq_set_timer))
    return 1;
  
  int ipc_status, r;
  message msg;
 
  while(time ==
   0 && scancode != ESC) { /* You may want to use a different condition */
     /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */				
            if (msg.m_notify.interrupts & BIT(irq_set_timer)) { /* subscribed interrupt */
              timer_int_handler();                   /* process it */
              if (count % 60 == 0)
              {
                time--;
              }
            }
            if (msg.m_notify.interrupts & BIT(irq_set_kbd)){
              kbc_ih();
              read_status_reg(&st);
              if (Check_oub_full(st))
              {
                tickdelay(micros_to_ticks(DELAY_US));

              }
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
              count = 0;
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
  if (kbc_ui())
  {
    return 1;
  }
  if (kbd_print_no_sysinb(cnt) != 0) {
   return 1;
  }

  return 0;
}
