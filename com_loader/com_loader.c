#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <file.com>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    
    // Открываем .COM файл
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        return 1;
    }

    // Узнаем размер файла
    struct stat st;
    if (fstat(fd, &st) < 0) {
        perror("Error getting file size");
        close(fd);
        return 1;
    }
    
    size_t filesize = st.st_size;
    
    // Проверяем, что файл не больше 64К
    if (filesize > 0xFFFF) {
        printf("Error: .COM file too large (> 64KB)\n");
        close(fd);
        return 1;
    }

    // Выделяем память для загрузки .COM файла + 0x100 для адреса выполнения
    void *mem = mmap(NULL, 0x10000, PROT_READ | PROT_WRITE | PROT_EXEC,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) {
        perror("Error allocating memory");
        close(fd);
        return 1;
    }

    // Загружаем содержимое .COM файла в память, начиная с адреса 0x0100
    ssize_t bytes_read = read(fd, (unsigned char *)mem + 0x100, filesize);
    if (bytes_read != filesize) {
        perror("Error reading file");
        munmap(mem, 0x10000);
        close(fd);
        return 1;
    }

    // Закрываем файл
    close(fd);
    
    // Устанавливаем указатель на начало кода .COM файла
    void (*entry_point)() = (void (*)())((unsigned char *)mem + 0x100);
    
    // Переходим к коду .COM файла
    entry_point();

    // Освобождаем выделенную память
    munmap(mem, 0x10000);

    return 0;
}