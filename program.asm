section .data
    message db 'Hello, World!', 0xA ; строка с символом новой строки

section .text
    global _start

_start:
    ; Системный вызов write
    mov eax, 4          ; syscall: sys_write
    mov ebx, 1          ; файл: stdout (1)
    mov ecx, message    ; указатель на сообщение
    mov edx, 14         ; длина строки (14 байт)
    int 0x80            ; вызвать системный вызов

    ; Системный вызов exit
    mov eax, 1          ; syscall: sys_exit
    xor ebx, ebx        ; статус выхода 0
    int 0x80            ; вызвать системный вызов