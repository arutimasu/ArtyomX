section .data
    hello db 'Hello, world!', 0  ; строка для вывода, с символом новой строки


;global _start

;extern idt_pointer


;;kernel.asm

;nasm directive - 32 bit
bits 32
section .text
        ;multiboot spec
        align 4
        dd 0x1BADB002            ;magic
        dd 0x00                  ;flags
        dd - (0x1BADB002 + 0x00) ;checksum. m+f+c should be zero

global start

global interrupt_handler

global load_idt

extern kmain	        ;kmain is defined in the c file
extern isr80

;extern idt_init

;extern idt_pointer
;load_idt:
;	mov edx, [esp + 4]
;	lidt [edx]
;	sti
;	ret


;load_idt:
;    ; Загружает указатель на IDT в регистр IDTR
;    lidt [esp + 4] ; загружаем указатель на IDT из стека
;    ret

;load_idt:
;	mov edx, [esp + 4]
;	lidt [edx]
;	sti 				;turn on interrupts
;	ret
;interrupt_handler:     
;	mov [esp + 4], eax
;	mov [esp + 4 + 4], ebx
;	mov [esp + 4+4+4], ecx
;	mov [esp + 4 + 4+4+4], edx
;	call    isr80
;	iretd
start:
  cli 			;block interrupts
  mov esp, stack_space	;set stack pointer
  call main
   ; Системный вызов для вывода строки
    mov eax, 4               ; код системного вызова для write
    mov ebx, 1               ; файловый дескриптор 1 (stdout)
    mov ecx, hello           ; адрес строки
    mov edx, 14              ; длина строки
    int 0x80                 ; вызов прерывания для выполнения системного вызова
  hlt		 	;halt the CPU

section .bss
resb 8192		;8KB for stack
stack_space:


;_start:
    ; Инициализация IDT
    ;call idt_init

    ;Установка указателя на IDT
    ; lidt [idt_pointer] ; idt_pointer это структура с адресом и размером IDT