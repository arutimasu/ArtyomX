#include <stdint.h>
#include <stddef.h>
#include "monitor.h"

#define IDT_ENTRY_COUNT 256

extern void load_idt(unsigned long *idt_ptr);
extern void interrupt_handler(void);
// Структура для описания обработчика прерывания
struct InterruptDescriptor {
    // uint16_t offset_low;
    // uint16_t selector;
    // uint8_t  zero;
    // uint8_t  type_attr;
    // uint16_t offset_high;
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};

// Таблица векторов прерываний
struct InterruptDescriptor idt[IDT_ENTRY_COUNT];

/*struct idtr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct idtr idt_pointer;
*/
//extern uint32_t idt_pointer;

//uint32_t idt_pointer[2] = { /* инициализация IDT */ };


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
// Пример обработчика прерывания
void isr20() {
    // Это простой обработчик, который будет выполнять цикл
    while (1) {
        // Вы можете сделать что-то полезное здесь, например, отправить сигнал
    }
}


// Пример обработчика прерывания
// Обработка прерывания 0x80
void isr80(int eax_value, int ebx_value, char* ecx_value, int edx_value) {
     // Пример для считывания аргументов
    
    // asm volatile (
       // "mov %0, eax\n\t"
        // "mov %1, ebx\n\t"
        // "mov %2, ecx\n\t"
        // "mov %3, edx\n\t"
        // : "=r" (eax_value), "=r" (ebx_value), "=r" (ecx_value), "=r" (edx_value)
    // );

    switch(eax_value) {
        case 4: // write syscall
            //write(1, "Hello, world!", 13);
			write(ebx_value, ecx_value, edx_value);
            break;
        // обработка других системных вызовов
    }
}
//Осталось написать программу вывода строки Hello world так чтобы она как-то вызывалась из ядра (через ассемблерную вставку или как-то ещё)
// Инициализация таблицы векторов прерываний
void idt_init() {
    // Заполним IDT. В случае прерывания 20 (0x20)
    //idt[0x20].offset_low = (uint16_t)((uint32_t)isr20 & 0xFFFF);
    //idt[0x20].selector = 0x08; // сегмент кода
    //idt[0x20].zero = 0;       
    //idt[0x20].type_attr = 0x8E; // Прерывание, разрешено, 32-бит
    //idt[0x20].offset_high = (uint16_t)(((uint32_t)isr20 >> 16) & 0xFFFF);
	
	    // Заполним IDT. В случае прерывания 20 (0x20)
    // idt[0x80].offset_low = (uint16_t)((uint32_t)isr20 & 0xFFFF);
    // idt[0x80].selector = 0x08; // сегмент кода
    // idt[0x80].zero = 0;       
    // idt[0x80].type_attr = 0x8E; // Прерывание, разрешено, 32-бит
    // idt[0x80].offset_high = (uint16_t)(((uint32_t)isr20 >> 16) & 0xFFFF);

    //Здесь вы должны установить значение в регистре IDTR
    //Обычно это делается через ассемблерный код
	
	// idt_pointer.limit = (sizeof(struct InterruptDescriptor) * IDT_ENTRY_COUNT) - 1;
    // idt_pointer.base = (uint32_t)&idt;
	
	// load_idt(&idt_pointer);
	
	unsigned long handler_address;
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	/* populate IDT entry of keyboard's interrupt */
	handler_address = (unsigned long)interrupt_handler; 
	idt[0x80].offset_lowerbits = handler_address & 0xffff;
	idt[0x80].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idt[0x80].zero = 0;
	idt[0x80].type_attr = 0x8e; /* INTERRUPT_GATE */
	idt[0x80].offset_higherbits = (handler_address & 0xffff0000) >> 16;
	
	/* fill the IDT descriptor */
	idt_address = (unsigned long)idt ;
	idt_ptr[0] = (sizeof (struct InterruptDescriptor) *  IDT_ENTRY_COUNT) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	load_idt(idt_ptr);
}

int my_write(const char *str) {
	int result;
    // asm volatile (
        // "mov $4, %%eax;"    // номер системного вызова write&lt;br&gt;        
		// "mov $1, %%ebx;"    // файл дескриптор (1 - stdout)
        // "mov %0, %%ecx;"    // указатель на строку
        // "mov $13, %%edx;"   // длина строки&lt;br&gt;        
		// "int $0x80;"        // вызов системного вызова
        // :                    // нет выходных аргументов
        // : "r"(str)          // входной аргумент
        // : "%eax", "%ebx", "%ecx", "%edx" // регистры, которые будут изменены
    // );
	 // __asm__ (
		// "int $0x80"
        // : "=a" (result)  /* возвращаемое значение */
        // : "a" ((unsigned int) 4)  /* номер системного вызова */
        // , "b" ((unsigned int) 1)  /* номер файла для вывода */
        // , "c" ((unsigned int) &str)  /* адрес буфера */
        // , "d" ((unsigned int) strlen(str))  /* сколько байт вывести */
        // : "memory"  /* будет читаться основная память */
		// );
		 return result;
}
void kmain(void)
{
	idt_init();
	monitor_clear(); // Очистка экрана
	
    //char *s = "Hello, world!";
    //size_t len = strlen(s);

     //my_write("Hello, World!\n");
	 //while (1); // Бесконечный цикл
	while(1);
}