// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "mouse.h"

extern  uint8_t packet;
extern struct packet pp;
extern uint8_t data;
int code = 0;

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


int (mouse_test_packet)(uint32_t cnt1) {
  uint8_t irq_set;

  mouse_report(ENABLE_MRP);
  if (mouse_subscribe_int(&irq_set)!=0){
    return 1;
  }
  int ipc_status, r, packet_count = 0;
  uint8_t index = 0;
  struct pp;

  message msg;

  while(cnt1) { /* You may want to use a different condition */
     /* Get a request message. */
  
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */				
            if (msg.m_notify.interrupts & BIT(irq_set)) { /* subscribed interrupt */
              mouse_ih();                                 /* process it */
              if (code == 0)
              {
                return 1;
              }

              if (packet_count == 0)
              {              
                if ((index & BIT(3))!= 0)
                {
                  pp.bytes[0] = data;
                  packet_count++;
                }
                else continue;
              }
              else if (packet_count == 1)
              {
                pp.bytes[1] = data;
                packet_count++;
              }
              else{
                pp.bytes[2] = data;
                packet__(&pp);
                mouse_print_packet(&pp);
                packet_count = 0;
                cnt1--;
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
 mouse_unsubscribe_int(&irq_set);
 mouse_report(DISABLE_MRP);
 return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    /* To be completed */
    printf("%s(%u): under construction\n", __func__, idle_time);
    return 1;
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
