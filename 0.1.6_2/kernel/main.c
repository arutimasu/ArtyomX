// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
//           Made for JamesM's tutorials <www.jamesmolloy.co.uk>
#include <stdint.h>
#include <stddef.h>
#include "../drivers/monitor.h"
//#include "../drivers/keyboard.h"
#include "descriptor_tables.h"
//#include "../drivers/timer.h"
//#include "paging.h"
#include "../boot/multiboot.h"
#include "../fs/fs.h"
#include "../fs/initrd.h"

#include "../common/unistd.h"
#include "../common/syscalls.h"
#include "../common/memory.h"

struct multiboot;
extern int used;
//extern u32int placement_address;
//extern fs_node_t *root_nodes;
//extern initrd_header_t *initrd_header;     // The header.
//char *exec = NULL;
//char *taddr = NULL;
void init_shell(){
	//int (*ptr)(int, char**, char**);
	
	 init_descriptor_tables();
	 asm volatile("sti");
	init_keyboard();
	
	initialise_syscalls();
	
	//ASSERT(mboot_ptr->mods_count > 0);
    //u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
    //u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
    // Don't trample our module with placement accesses, please!
    //placement_address = initrd_end;

    // Start paging.
    //initialise_paging();
	
	
    // Initialise the initial ramdisk, and set it as the filesystem root.
    //fs_root = initialise_initrd(initrd_location);
	char buffer[1024]; //= "Hello";
	for(;;){
	
	char* tokens[64];
	
	
	
	//while(strcmp(tokens[0], "exit")!=0){
	monitor_write("\nX ");
	read(0, buffer, 1024);
	
	
	
	int position = 0;
	char *token = strtok(buffer, " ");
    while(token)
    {
		tokens[position] = token;
		position++;
        //monitor_write(token);
        token = strtok(0, " ");
    }
	//tokens[position] = 0;
	//for(int i=0;i<position;i++)
		//monitor_write(tokens[i]);
	if(strcmp(tokens[0], "echo")==0){
		monitor_write("\n");
		for(int i = 1; i < position; i++){
			monitor_write(tokens[i]);
			monitor_put(' ');
		}
	}
	if(strcmp(tokens[0], "cat")==0){
		
		fs_node_t *fsnode = finddir_fs(fs_root, tokens[1]);
		char buf[256];
        u32int sz = read_fs(fsnode, 0, 256, buf);
		write(1, buf, sz);
        /*int j;
        for (j = 0; j < sz; j++)
			monitor_put(buf[j]);*/
		//free(fsnode);
		simple_free(fsnode);
			monitor_write("\n");
		monitor_write_dec(32768-used);
		monitor_write(" bytes free\n");
	}
	if(strcmp(tokens[0], "ls")==0){
	 int i = 0;
    struct dirent *node = 0;
    while ( (node = readdir_fs(fs_root, i)) != 0)
    {
        monitor_write("Found file ");
        monitor_write(node->name);
        fs_node_t *fsnode = finddir_fs(fs_root, node->name);
		//monitor_write_dec(fsnode->inode);
        if ((fsnode->flags&0x7) == FS_DIRECTORY)
        {
            monitor_write("\n\t(directory)\n");
        }
        else
        {
            monitor_write("\n\t contents: \"");
            char buf[256];
            u32int sz = read_fs(fsnode, 0, 256, buf);
            int j;
            for (j = 0; j < sz; j++)
                monitor_put(buf[j]);
            
            monitor_write("\"\n");
        }
        i++;
    }
	monitor_write("\n");
		monitor_write_dec(32768-used);
		monitor_write(" bytes free\n");
	}
	if(strcmp(tokens[0], "rm")==0){
		
		
		initrd_remove(tokens[1]);
	}
	if (strcmp(tokens[0], "ex") == 0) {
    char buf[14000];  // Allocate buffer for the program
    memset(buf, 0x0, sizeof(buf));

    fs_node_t *fsnode = finddir_fs(fs_root, tokens[1]);
    u32int sz = read_fs(fsnode, 0, sizeof(buf), buf);
	
	//exec = mmap(sz, 1);
	
   int (*ptr)(int, char **, char**);
    ptr = image_load(buf, sz/*, &exec, &taddr*/);
	
    if (ptr != NULL) {
		
        monitor_write("Run the loaded program:\n");
        
        // Передаем управление запущенной программе
        ptr(0, NULL, NULL); 
        simple_free(ptr);
        // Программа вернулась в оболочку
        monitor_write("Program exited. Returning to shell.\n");
    } else {
        monitor_write("Loading unsuccessful...\n");
		//free(ptr);
		
    }
	//free(fsnode);
	simple_free(fsnode);
}


	
	}
	}
int main(struct multiboot *mboot_ptr)
{
	
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();
    // Initialise the screen (by clearing it)
    monitor_clear();
	monitor_write("Welcome to ArtyomX 0.1.6!\n");
	monitor_write_dec(32768-used);
	monitor_write(" bytes free\n");
	
	
	asm volatile("sti");
	init_keyboard();
	
	initialise_syscalls();
    // Find the location of our initial ramdisk.
    ASSERT(mboot_ptr->mods_count > 0);
    u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
    u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
    // Don't trample our module with placement accesses, please!
    //placement_address = initrd_end;

    // Start paging.
    //initialise_paging();
	
	
    // Initialise the initial ramdisk, and set it as the filesystem root.
    fs_root = initialise_initrd(initrd_location);

    // list the contents of /
    int i = 0;
    struct dirent *node = 0;
    while ( (node = readdir_fs(fs_root, i)) != 0)
    {
        monitor_write("Found file ");
        monitor_write(node->name);
        fs_node_t *fsnode = finddir_fs(fs_root, node->name);
		//monitor_write_dec(fsnode->inode);
        if ((fsnode->flags&0x7) == FS_DIRECTORY)
        {
            monitor_write("\n\t(directory)\n");
        }
        else
        {
            monitor_write("\n\t contents: \"");
            char buf[256];
            u32int sz = read_fs(fsnode, 0, 256, buf);
            int j;
            for (j = 0; j < sz; j++)
                monitor_put(buf[j]);
            
            monitor_write("\"\n");
        }
        i++;
    }

	
	init_shell();
	
	
    
	//}else{
	//	write(1, "\nCommand not found!", 20); 
	//}
 
	//write(1, str, 5);
	

    return 0;
}
