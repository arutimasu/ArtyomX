#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>

char prog[] = {
   0x55,             // push   %rbp
   0x48, 0x89, 0xe5, // mov    %rsp,%rbp
   0xf2, 0x0f, 0x10, 0x05, 0x00, 0x00, 0x00,
       //movsd  0x0(%rip),%xmm0        # c <x+0xc>
   0x00,
   0x5d,             // pop    %rbp
   0xc3,             // retq
};

int main()
{
    long pagesize = sysconf(_SC_PAGE_SIZE);
    long page_no = (long)prog/pagesize;
    int res = mprotect((void*)(page_no*pagesize), (long)page_no+sizeof(prog), PROT_EXEC|PROT_READ|PROT_WRITE);
    if(res)
    {
        fprintf(stderr, "mprotect error:%d\n", res);
        return 1;
    }
    typedef double (*dfunc)(void);

    dfunc d = (dfunc)(&prog[0]);
    double x = (*d)();
    printf("x=%f\n", x);
    fflush(stdout);
    return 0;
}