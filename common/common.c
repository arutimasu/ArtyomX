// common.c -- Defines some global functions.
//             From JamesM's kernel development tutorials.
#include <stddef.h>
#include "common.h"

// Write a byte out to the specified port.
void outb(u16int port, u8int value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int inb(u16int port)
{
    u8int ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

u16int inw(u16int port)
{
    u16int ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

// Copy len bytes from src to dest.
void memcpy(u8int *dest, const u8int *src, u32int len)
{
    const u8int *sp = (const u8int *)src;
    u8int *dp = (u8int *)dest;
    for(; len != 0; len--) *dp++ = *sp++;
}

// Write len copies of val into dest.
void memset(u8int *dest, u8int val, u32int len)
{
    u8int *temp = (u8int *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

void *memmove(void *dest, const void *src, u32int n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    if (d == s || n == 0) {
        return dest; // ничего не нужно делать
    }

    // Если область назначения перекрывает область источника, сначала копируем в буфер
    if (d < s || d >= s + n) {
        while (n--) {
            *d++ = *s++;
        }
    } else {
        // Если область назначения перекрывает область источника, копируем в обратном порядке
        d += n;
        s += n;
        while (n--) {
            *(--d) = *(--s);
        }
    }

    return dest;
}

// Compare two strings. Should return -1 if 
// str1 < str2, 0 if they are equal or 1 otherwise.
int strcmp(char *str1, char *str2)
{
      int i = 0;
      int failed = 0;
      while(str1[i] != '\0' && str2[i] != '\0')
      {
          if(str1[i] != str2[i])
          {
              failed = 1;
              break;
          }
          i++;
      }
      // why did the loop exit?
      if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
          failed = 1;
  
      return failed;
}

// Copy the NULL-terminated string src into dest, and
// return dest.
char *strcpy(char *dest, const char *src)
{
    do
    {
      *dest++ = *src++;
    }
    while (*src != 0);
}

// Concatenate the NULL-terminated string src onto
// the end of dest, and return dest.
char *strcat(char *dest, const char *src)
{
    while (*dest != 0)
    {
        *dest = *dest++;
    }

    do
    {
        *dest++ = *src++;
    }
    while (*src != 0);
    return dest;
}

int strlen(char *src)
{
    int i = 0;
    while (*src++)
        i++;
    return i;
}

unsigned int is_delim(char c, char *delim)
{
    while(*delim != '\0')
    {
        if(c == *delim)
            return 1;
        delim++;
    }
    return 0;
}
char *strtok(char *srcString, char *delim)
{
    static char *backup_string; // start of the next search
    if(!srcString)
    {
        srcString = backup_string;
    }
    if(!srcString)
    {
        // user is bad user
        return 0;
    }
    // handle beginning of the string containing delims
    while(1)
    {
        if(is_delim(*srcString, delim))
        {
            srcString++;
            continue;
        }
        if(*srcString == '\0')
        {
            // we've reached the end of the string
            return 0; 
        }
        break;
    }
    char *ret = srcString;
    while(1)
    {
        if(*srcString == '\0')
        {
            /*end of the input string and
            next exec will return NULL*/
            backup_string = srcString;
            return ret;
        }
        if(is_delim(*srcString, delim))
        {
            *srcString = '\0';
            backup_string = srcString + 1;
            return ret;
        }
        srcString++;
    }
}

#define HEAP_SIZE 32768  // Размер пула памяти


static u8int heap[HEAP_SIZE]; // Пул памяти
int used = 0; // Использованное пространство в пуле

void *malloc(int size) {
    // Выравнивание по 4 байта
    size += (4 - (size % 4)) % 4;

    // Проверьте, есть ли достаточное место
    if (used + size + sizeof(size) > HEAP_SIZE) {
        return NULL; // Недостаточно места
    }

    // Указатель на размер перед выделяемым пространством
    *(size_t *)&heap[used] = size; // Сохраняем размер перед указателем
    void *ptr = &heap[used + sizeof(size)]; // Указатель на выделяемое пространство
    used += size + sizeof(size); // Обновляем текущее использованное пространство
    return ptr; // Возвращаем указатель
}

// Реализация free
void free(void *ptr) {
    if (!ptr) return; // Если указатель нулевой, ничего не делать

    // Для восстановления индекса использованного пространства вычисляем
    // размер блока, используя указатель, идвижемся на 4 байта назад
    size_t *size_ptr = (size_t *)((u8int *)ptr - sizeof(size_t));
    size_t size = *size_ptr;

    // Освобождаем память, но в данном случае низкоуровневая память
    // не может быть "освобождена", но мы могли бы управлять свободным списком.
    // Просто обновляем переменную <code>used</code>.
    used -= (size + sizeof(size)); // Уменьшаем использованное пространство
}

#define PAGE_SIZE 4096  // Размер страницы

// Структура для управления выделенной памятью
typedef struct {
    void *addr;   // Указатель на выделенную память
    size_t size;  // Размер выделенной памяти
    int is_executable;  // Флаг для контроля за исполнением
} MemBlock;

static MemBlock memory_blocks[10];  // Массив для отслеживания выделенных памяти
static int current_block = 0;        // Текущий индекс в массиве

// Простая реализация mmap
void* mmap(size_t size, int is_executable) {
    if (size == 0) return NULL;

    // Выравнивание размера по размеру страницы
    size = ((size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;

    void *addr = malloc(size);
    if (!addr) return NULL;  // Ошибка выделения памяти

    // Сохраняем информацию о выделенной памяти
    if (current_block < sizeof(memory_blocks) / sizeof(MemBlock)) {
        memory_blocks[current_block].addr = addr;
        memory_blocks[current_block].size = size;
        memory_blocks[current_block].is_executable = is_executable;
        current_block++;
    }

    return addr;  // Возвращаем указатель на выделенный блок
}

// Освобождение памяти
void munmap(void *addr) {
    for (int i = 0; i < current_block; i++) {
        if (memory_blocks[i].addr == addr) {
            free(addr);  // Освобождаем память
            memory_blocks[i] = memory_blocks[--current_block];  // Удаляем блок
            return;
        }
    }
}

extern void panic(const char *message, const char *file, u32int line)
{
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.

    monitor_write("PANIC(");
    monitor_write(message);
    monitor_write(") at ");
    monitor_write(file);
    monitor_write(":");
    monitor_write_dec(line);
    monitor_write("\n");
    // Halt by going into an infinite loop.
    for(;;);
}

extern void panic_assert(const char *file, u32int line, const char *desc)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.

    monitor_write("ASSERTION-FAILED(");
    monitor_write(desc);
    monitor_write(") at ");
    monitor_write(file);
    monitor_write(":");
    monitor_write_dec(line);
    monitor_write("\n");
    // Halt by going into an infinite loop.
    for(;;);
}
