#include <lcom/lcf.h>
#include <stdio.h>
#include <machine/int86.h>
#include "mvc/controller/graphics/VBE.h"
#include "mvc/controller/graphics/graphic.h"
#include "mvc/controller/keyboard/keyboard.h"
#include "mvc/controller/timer/timer.h"


const int desiredFrameRate = 60;
const double targetFrameTime = 1.0 / desiredFrameRate;

extern counter;
bool isRunning = true;

uint8_t timer_hook_id = 0;
uint8_t beyboard_hook_id = 0;
uint8_t mouse_hook_id = 0;


int main(int argc, char *argv[]){

  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g1/proj/stc/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g1/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int setup(){

  if(timer_subscribe_int(&timer_hook_id) != 0) return 1;
  if(keyboard_subscribe_int(&timer_hook_id) != 0) return 1;
  if(timer_set_frequency(0, desiredFrameRate) != 0) return 1;

  if(set_frame_mode(VBE_800x600_DC) != 0) return 1;
  if(set_graphic_mode(VBE_800x600_DC) != 0) return 1;
  //todo enable double buffer

  return 0;
}

int exit(){

  if(vg_exit() != 0) return 1;

  if(timer_unsubscribe_int() != 0) return 1;
  if(keyboard_unsubscribe_int() != 0) return 1;
  //if(mouse_unsubscribe_int() != 0) return 1;

  if(set_text_mode() != 0) return 1;
}

int (proj_main_loop)(int argc , char* argv[]){


  setup();

  int ipc_status,r;
  message msg;

  setup();

  while (isRunning)
  {
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)){
      switch (_ENDPOINT_P(msg.m_source)) {

        if(msg.m_notify.interrupts & BIT(timer_hook_id)){
        }

        if(msg.m_notify.interrupts & BIT(beyboard_hook_id)){
        }
        //if(msg.m_notify.interrupts & BIT(mouse_hook_id)){
      }
    }
  }

  exit();
  

  return 0;
}
