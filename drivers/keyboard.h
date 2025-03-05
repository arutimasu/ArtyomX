#ifndef KETBOARD_H
#define KEYBOARD_H
#include "../common/common.h"


//extern char *buffer;
//void keyboard_handler_main(void);
void init_keyboard();
char keyboard_read();
extern int is_enter_pressed;
//int is_enter_pressed;
#endif