#ifndef _MOUSE_H_
#define _LCOM_I8254_H_

#pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "../kbc/i8042.h"
#include "../kbc/kbc.h"

extern struct Board * boardArray[1024];
extern int index_;
extern int max_index;


int(mouse_subscribe_int)(uint8_t *bit_no);
int(mouse_unsubscribe_int)();
int (read_status_register)(uint8_t *st);
int(reset_mouse_struct)(struct packet *mouse);
int(parse_values)(uint8_t data,int *cnt,struct packet *pp);
int(disable_mouse_report)();
int(enable_mouse_report)();
void clean_packet(struct packet *mouse);
int (in_game_mouse_movement)();
int(cursor_draw_start)();

enum InGameStates{
  INITIAL,
  PIECE_SELECTED,
  PIECE_CLICKED,
  PIECE_DRAGGED,
  COMPLETE,
};



#endif

