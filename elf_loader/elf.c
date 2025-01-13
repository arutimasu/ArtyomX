// File: elf.c

#include <stdio.h>

int square(int x)
{
    return x*x;
}

int main(int argc, char** argv)
{
    fprintf(stdout, "Hello world!\n");
    fprintf(stdout, "fprintf=%p, stdout=%p\n", fprintf, stdout);
    fprintf(stdout, "square(10) = %d\n", square(10));

    return 0;
}