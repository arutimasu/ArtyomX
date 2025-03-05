//
// isr.c -- High level interrupt service routines and interrupt request handlers.
//          Part of this code is modified from Bran's kernel development tutorials.
//          Rewritten for JamesM's kernel development tutorials.
//

#include "../common/common.h"
#include "isr.h"
#include "../drivers/monitor.h"

isr_t interrupt_handlers[256];
extern init_shell();
//extern char *exec;
//extern char *taddr;
void register_interrupt_handler(u8int n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

// This gets called from our ASM ifnterrupt handler stub.
void isr_handler(registers_t regs)
{
	u8int int_no = regs.int_no & 0xFF;
    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(&regs);
    }
    else
    {
		//munmap(&exec);
		//munmap(&taddr);
		init_shell();
        //monitor_write("unhandled interrupt: ");
        //monitor_write_dec(regs.int_no);
        //monitor_put('\n');
    }
}

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
{
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (regs.int_no >= 40)
    {
        // Send reset signal to slave.
        outb(0xA0, 0x20);
    }
    // Send reset signal to master. (As well as slave, if necessary).
    outb(0x20, 0x20);

    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(&regs);
    }

}
