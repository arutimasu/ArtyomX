//#include "keyboard.h"
#include "monitor.h"
#include "keyboard_map.h"
#include "../kernel/isr.h"
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#define ENTER_KEY_CODE 0x1C

#define BUFFER_SIZE 256

char keyboard_buffer[BUFFER_SIZE];
int buffer_head = 0;
int buffer_tail = 0;
int is_enter_pressed = 0;

int is_buffer_empty() {
    return buffer_head == buffer_tail;
}

int is_buffer_full() {
    return (buffer_head + 1) % BUFFER_SIZE == buffer_tail;
}


char keyboard_read() {
    if (is_buffer_empty()) {
        return -1;  // Или какой-то код ошибки
    }

    char c = keyboard_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % BUFFER_SIZE;
    return c;
}

static void keyboard_callback(registers_t regs){
	unsigned char status;
	char keycode;

		
		/* write EOI */
		
		//outb(0x20, 0x20);
		//monitor_put('\n');
		//monitor_write("recieved interrupt: ");
		//monitor_write_dec(regs.int_no);
		//monitor_put('\n');
		//monitor_put('\n');
		//monitor_put(keyboard_map[(unsigned char) keycode]);
		//buffer[buffer_cursor++] = keyboard_map[(unsigned char) keycode];
		
		status = inb(KEYBOARD_STATUS_PORT);
		/* Lowest bit of status will be set if buffer is not empty */
		if (status & 0x01) {
			keycode = inb(KEYBOARD_DATA_PORT);
			if(keycode < 0)
				return;
			if(keycode == ENTER_KEY_CODE) {
				is_enter_pressed = 1;
				return;
			}
			 // Получаем символ из карты клавиатуры
			char c = keyboard_map[keycode];

			// Добавляем символ в буфер, если он не полный
			if (!is_buffer_full()) {
				keyboard_buffer[buffer_head] = c;
				buffer_head = (buffer_head + 1) % BUFFER_SIZE;
			}
			 
			//end:	
				//buffer[buffer_cursor++] = keyboard_map[(unsigned char) keycode];
				//outb(0x20, 0x20); /* end of interrupt */
		}
}

void init_keyboard()
{
	 register_interrupt_handler(IRQ1, &keyboard_callback);
	 outb(0x21, 0xFD);
}
//void keyboard_handler_main(void) {
	
	/* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
	//write_port(0x21 , 0xFD);
	
	/*char *buffer;
	unsigned char status;
	char keycode;
	int i = 0;*/
	/* write EOI */
	
	//write_port(0x20, 0x20);
	//outb(0x20, 0x20);

	//status = read_port(KEYBOARD_STATUS_PORT);
	//status = inb(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	/*if (status & 0x01) {
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
}*/

