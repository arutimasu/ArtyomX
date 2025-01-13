#include "unistd.h"
#include "syscalls.h"
#include "../drivers/monitor.h"
#include "../drivers/keyboard.h"

#include "../kernel/isr.h"
void write(int fd, char* buf, int nbytes) {
	// unsigned int i = 0;
	// switch(fd){
		// case 1:
		// /* this loop writes the string to video memory */
		// for(int j=0; j<nbytes; ++j){
			// /* the character's ascii */
			// vidptr[i] = str[j];
			// /* attribute-byte: give character black bg and light grey fg */
			// vidptr[i+1] = 0x07;
			//++j;
			// i = i + 2;
		// }
		// break;
	// }
	switch(fd){
		case 1:
		for(int i = 0; i < strlen(buf); i++){
			monitor_put(buf[i]);
		}
		break;
	}
}
void read(int fd, char *buf, int nbytes) {
	int i = 0;
    switch (fd) {
        case 0:
          	while (!is_enter_pressed) {
			//for(int i = 0; i < nbytes; i++){
				char c = keyboard_read();
				if (c != -1) {
					
					buf[i] = c;
					i++;
					// Обработка полученного символа
					// Например, вывод на экран, интерпретация команд и т.д.
					monitor_put(c);
					//vidptr[current_loc++] = 0x07;  // Атрибут цвета
				}
			}
			is_enter_pressed = 0;
			buf[i] = 0;
			monitor_write("\n");
            break;
    }
}


//DEFN_SYSCALL1(monitor_write_dec, 2, const char*);

static void syscall_handler(registers_t *regs);

DEFN_SYSCALL3(read, 3,  int, const char*, int);
DEFN_SYSCALL3(write, 4, int, const char*, int);
//DEFN_SYSCALL1(monitor_write, 0, const char*);
//DEFN_SYSCALL1(monitor_write_hex, 1, const char*);
//DEFN_SYSCALL1(monitor_write_dec, 2, const char*);

static void *syscalls[5] =
{
	0,
	0,
	0,
	&read,
	&write,
	
};


u32int num_syscalls = 5;

void initialise_syscalls()
{
    // Register our syscall handler.
    register_interrupt_handler (0x80, &syscall_handler);
}

void syscall_handler(registers_t *regs)
{
    // Firstly, check if the requested syscall number is valid.
    // The syscall number is found in EAX.
    if (regs->eax >= num_syscalls)
        return;

    // Get the required syscall location.
    void *location = syscalls[regs->eax];

    // We don't know how many parameters the function wants, so we just
    // push them all onto the stack in the correct order. The function will
    // use all the parameters it wants, and we can pop them all back off afterwards.
    int ret;
    asm volatile (" \
      push %1; \
      push %2; \
      push %3; \
      push %4; \
      push %5; \
      call *%6; \
      pop %%ebx; \
      pop %%ebx; \
      pop %%ebx; \
      pop %%ebx; \
      pop %%ebx; \
    " : "=a" (ret) : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (location));
    regs->eax = ret;
}

