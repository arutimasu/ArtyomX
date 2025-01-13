/*int main() {
    //write(1, "Hello, world!\n", 14);
	printf("Hello, World!\n");
    return 0;
}*/

//Tell the compiler incoming stack alignment is not RSP%16==8 or ESP%16==12
__attribute__((force_align_arg_pointer))
void _start() {

  
	
	//const char *message = "Hello, world!\n";
    const char *message = "Hello, world!\n";
    /*asm (
        "mov $1, %%rax\n"      // Системный вызов write
        "mov $1, %%rdi\n"      // stdout
        "mov %0, %%rsi\n"      // адрес сообщения
        "mov $14, %%rdx\n"     // длина сообщения
        "syscall\n"
        : // no output
        : "r"(message)         // input
        : "%rax", "%rdi", "%rsi", "%rdx"
    );*/
	asm (
        "mov $4, %%eax\n"      // Системный вызов write
        "mov $1, %%ebx\n"      // stdout
        "mov %0, %%ecx\n"      // адрес сообщения
        "mov $14, %%edx\n"     // длина сообщения
        "int $0x80\n"
		"hlt\n"
        : // no output
        : "r"(message)         // input
        : "%eax", "%ebx", "%ecx", "%edx"
    );
	 
   __builtin_unreachable();  // tell the compiler to make sure side effects are done before the asm statement
   
}

// example.c

