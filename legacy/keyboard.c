#include "keyboard.h"
#include "common.h"
#include "monitor.h"

#include "keyboard_map.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64


void keyboard_handler_main(void) {
	
	/* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
	//write_port(0x21 , 0xFD);
	
	char *buffer;
	unsigned char status;
	char keycode;
	int i = 0;
	/* write EOI */
	
	//write_port(0x20, 0x20);
	outb(0x20, 0x20);

	//status = read_port(KEYBOARD_STATUS_PORT);
	status = inb(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	if (status & 0x01) {
		//keycode = read_port(KEYBOARD_DATA_PORT);
		keycode = inb(KEYBOARD_DATA_PORT);
		if(keycode < 0)
			return;
		
		monitor_put(keyboard_map[(unsigned char) keycode]);
		buffer[i++] = keyboard_map[keycode];
		
		//vidptr[current_loc++] = keyboard_map[keycode];
		//vidptr[current_loc++] = 0x07;	
	}
	
	//outb(0x21, 0xFF);
}

