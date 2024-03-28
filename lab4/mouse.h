#ifndef MOUSE_H
#define MOUSE_H

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
/*
struct packet
{
    uint8_t bytes[3];
    bool rb; //right button
    bool mb; //middle button
    bool lb; //left button
    int16_t delta_x;
    int16_t delta_y;
    bool x_ov; //x overflow
    bool y_ov; //y overflow
};
*/


int mouse_enable_ui();
int mouse_diable_ui();

int mouse_subscribe_int(uint8_t *bit_no);
int mouse_unsubscribe_int();

int mouse_write_command(uint8_t command);
int read_status_register(uint8_t *status);

void mouse_ih(); //interrupt handler
void mouse_sync_bytes();
void mouse_clear_data();
void mouse_parse_packet();



#endif
