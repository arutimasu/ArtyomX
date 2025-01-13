MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
; NOTE: We do not use MBOOT_AOUT_KLUDGE. It means that GRUB does not
; pass us a symbol table.
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


[BITS 32]

[GLOBAL mboot]
[EXTERN code]
[EXTERN bss]
[EXTERN end]

section .multiboot
    align 4
    dd MBOOT_HEADER_MAGIC
    dd MBOOT_HEADER_FLAGS
    dd MBOOT_CHECKSUM
    dd start
    dd 0     ; Код должен быть загружен непосредственно после заголовка Multiboot
; section .mbheader
; align 4
; mboot:
    ; dd  MBOOT_HEADER_MAGIC
    ; dd  MBOOT_HEADER_FLAGS
    ; dd  MBOOT_CHECKSUM
    ; dd  mboot
    ; dd  code
    ; dd  bss
    ; dd  end
    ; dd  start

section .data
    hello db 'Hello, world!', 0x0A  ; строка для вывода, с символом новой строки

section .text
align 4
  dd  MBOOT_HEADER_MAGIC
  dd  MBOOT_HEADER_FLAGS
  dd  MBOOT_CHECKSUM
  ;dd  mboot
  dd  code
  dd  bss
  dd  end
  dd  start
[GLOBAL start]
[EXTERN main]

;global isr33

	
start:
    ; Load multiboot information:
    push    ebx
    cli
    call main
	  ; Системный вызов для вывода строки
	 ; mov eax, 4               ; код системного вызова для write
     ; mov ebx, 1               ; файловый дескриптор 1 (stdout)
     ; mov ecx, hello           ; адрес строки
     ; mov edx, 13              ; длина строки
     ; int 0x80                 ; вызов прерывания для выполнения системного 
	 ; int 0x80                 ; вызов прерывания для выполнения системного 
	
    jmp $
	;hlt