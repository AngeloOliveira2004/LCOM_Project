// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <lcom/lab4.h>
#include <lcom/timer.h>
#include <kbc.h>
#include <i8042.h>
#include <mouse.h>
#include <i8254.h>

extern struct packet pp;
extern uint8_t byte_index;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

/*
  configure the mouse properly
  receive the number of mouse packets specified in its argument via interrupts
  parse the mouse packets
  print them in a friendly way
  reset the mouse to Minix's default configuration, before exiting
*/
int (mouse_test_packet)(uint32_t cnt) {
    int ipc_status , r;
    message msg;
    bool valid = true;

    uint8_t irq_set;

    if(mouse_suscribe_int(&irq_set)){
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
                if(msg.m_notify.interrupts & irq_set) {
                    mouse_ih();
                    mouse_sync_bytes();
                }


                break;  
              }
                break;
            }
    }

    if (mouse_unsubscribe_int() != OK){
        return 1;
    }

    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    


    return 1;
}

int (mouse_test_gesture)() {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
