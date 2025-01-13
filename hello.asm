section .data
    hello db 'Hello, world!', 0x0A  ; строка для вывода, с символом новой строки
section .text
    ;global _start
org 0x0100
start:
	mov eax, 4               ; код системного вызова для write
	mov ebx, 1               ; файловый дескриптор 1 (stdout)
	mov ecx, hello           ; адрес строки
	mov edx, 13              ; длина строки
	int 0x80                 ; вызов прерывания для выполнения системного 
	
; section .text
; org 0x0100

; start:
    ;Печать "Hello, World!"
    ; mov ah, 0x0E          ; Функция BIOS для вывода символа
    ; mov si, message

; print_loop:
    ; lodsb                ; Загрузить следующий байт в AL
    ; cmp al, 0           ; Проверка конца строки
    ; je done             ; Если 0, то завершить
    ; int 0x10           ; Вызов BIOS для печати символа
    ; jmp print_loop

; done:
    ;Завершение программы
    ; mov ax, 0x4C00       ; Функция завершения
    ; int 0x21             ; Прерывание DOS

; section .data
; message db 'Hello, World!', 0
