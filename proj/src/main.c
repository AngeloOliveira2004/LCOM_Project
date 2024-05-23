#include <lcom/lcf.h>
#include <stdio.h>
#include <machine/int86.h>
#include "mvc/controller/graphics/VBE.h"
#include "mvc/controller/graphics/graphic.h"
#include "mvc/controller/keyboard/keyboard.h"
#include "mvc/controller/timer/timer.h"
#include "mvc/controller/mouse/mouse.h"
#include "mvc/controller/mouse/kbc.h"
#include "mvc/view/view.h"
#include "mvc/model/board.h"
#include "mvc/model/menu.h"

extern int counter;
int elapsed_seconds = true;

const int desiredFrameRate = 30;
const double targetFrameTime = 1.0 / desiredFrameRate;

bool isRunning = true;

uint8_t timer_hook_id = 0;
uint8_t keyboard_hook_id = 1;
uint8_t mouse_hook_id = 2;

int main(int argc, char *argv[]){

  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g1/proj/src/trace.txt");

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


  if(timer_set_frequency(0, desiredFrameRate) != 0) return 1;

  uint16_t mode = VBE_800x600_DC;

  if(initialize_graphics( &mode) != 0) return 1;

  return 0;
}

int _exit_(){

  //if(mouse_unsubscribe_int() != 0) return 1;
  if(set_text_mode() != 0) return 1;
  if(timer_unsubscribe_int() != 0) return 1;
  if(keyboard_unsubscribe_int() != 0) return 1; 

  if(vg_exit() != 0) return 1;
  
  return 0;
}


int (proj_main_loop)(int argc , char* argv[]){

  int ipc_status,r;
  message msg;
  uint8_t irq_timer, irq_keyboard;  //,irq_mouse ;

  struct Board* board = create_board();

  init_board(board);

  if(timer_subscribe_int(&irq_timer) != 0) return 1;
  if(keyboard_subscribe_int(&irq_keyboard) != 0) return 1;
  //if(mouse_subscribe_int(&irq_mouse) !=0) return 1;

  setup();
    

  if(draw_backBackGround() != 0){
    return 1;
  }

  copy_BackGroundBuffer();
  //swap_BackgroundBuffer();

  if(draw_board(board) != 0){
    return 1;
  }

  struct Position initalPos = board->pieces[0].position;
  initalPos.y = 1;
  initalPos.x = 6;
  board->pieces[0].position.x = 4;
  board->pieces[0].position.y = 7;


  if(return_to_initial_pos(&board->pieces[0] , &initalPos , board) != 0){
    return 1;
  }

  while (isRunning)
  {
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
      }

    if (is_ipc_notify(ipc_status)){
      printf("received message\n");
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if(msg.m_notify.interrupts & irq_timer){
            
            printf("timer interrupt\n");
            timer_int_handler();
            if(counter % 30){
              elapsed_seconds += 1; 
              counter = 0;
              /*
              if(draw() != 0){
                printf("Error drawing\n");
              }*/
            }
          }

          if(msg.m_notify.interrupts & irq_keyboard){
            printf("keyboard interrupt\n");
            if(check_ESC() != 0) isRunning = false;
          }
          /*
          if(msg.m_notify.interrupts & irq_mouse){
            //do nothing for now
          }*/
          break;
        default:
          break; 
      }
    }

    
  }

  _exit_();

  return 0;
}
