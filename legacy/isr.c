#include "common.h"
#include "isr.h"
#include "monitor.h"
#include "keyboard_map.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define ENTER_KEY_CODE 0x1C

u32int tick = 0;

//char buffer[128];
//extern char keycode;
//extern int buffer_cursor;


#define BUFFER_SIZE 256

char keyboard_buffer[BUFFER_SIZE];
int buffer_head = 0;
int buffer_tail = 0;

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


void isr_handler(registers_t regs) {
	if(regs.int_no!=13 && regs.int_no!=6  && regs.int_no==80){
    monitor_write("recieved interrupt: ");
    monitor_write_dec(regs.int_no);
    //monitor_write(" ");
    //monitor_write_dec(tick++);
    monitor_put('\n');
	
	 switch(regs.eax) {
        case 4: // write syscall
            //write(1, "Hello, world!", 13);
			write(regs.ebx, regs.ecx, regs.edx);
            break;
		/*case 3: // write syscall
            //write(1, "Hello, world!", 13);
			read(regs.ebx, regs.ecx, regs.edx);
            break;
        // обработка других системных вызовов*/
    }
	}
	if (regs.int_no == 33) {
		
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
		
		outb(0x20, 0x20);
		status = inb(KEYBOARD_STATUS_PORT);
		/* Lowest bit of status will be set if buffer is not empty */
		if (status & 0x01) {
			keycode = inb(KEYBOARD_DATA_PORT);
			 if (keycode < 0/*1*/) {
				//goto end;
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
	
    /*if (regs.int_no >= IRQ_MASTER_0) {
        if (regs.int_no >= IRQ_SLAVE_0)
            outb(PIC_SLAVE_CMD, PIC_CMD_RESET);
        outb(PIC_MASTER_CMD, PIC_CMD_RESET);
    }*/
	
}
