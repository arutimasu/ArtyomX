section .text
    global _start

_start:
    ; Вызов системного вызова для вывода строки
    mov eax, 4          ; номер системного вызова sys_write
    mov ebx, 1          ; номер файлового дескриптора stdout
    mov ecx, msg        ; указатель на сообщение
    mov edx, msg_len    ; длина сообщения
    int 0x80            ; выполнить системный вызов

    ; Вызов системного вызова для завершения программы
    mov eax, 1          ; номер системного вызова sys_exit
    xor ebx, ebx        ; код возврата 0
    int 0x80            ; выполнить системный вызов

section .data
msg db 'Hello, World!', 0xA  ; сообщение и перевод строки
msg_len equ $ - msg            ; длина сообщения