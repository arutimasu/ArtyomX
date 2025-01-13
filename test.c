/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>

// Функция для загрузки и выполнения .out файла
void load_out_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Cannot open file");
        return;
    }

    // Получение размера файла
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Выделение памяти для программы
    unsigned char *buffer = malloc(size);
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(file);
        return;
    }

    // Чтение файла в буфер
    fread(buffer, 1, size, file);
    fclose(file);

    // Простой вызов функции (предполагая, что код начинается с адреса 0)
    void (*program)() = (void (*)())buffer;
    program(); // Запуск программы

    free(buffer);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file.out>\n", argv[0]);
        return 1;
    }

    load_out_file(argv[1]);
    return 0;
}