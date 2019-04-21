#include <string.h>
#include <unistd.h>

#define UTIL_IMPLEMENTATION
#include "utils.h"

int main(char *args)
{
	
	int ppid = getppid();

	
	

	if(ppid == 0)
	{
		println("Hello World iz programa hello.bin!");
		vardump(ppid);
		println("Nemam parenta!");
	}
	if(ppid == 2)
	{
		println("Hello World iz programa hello.bin!");
		vardump(ppid);
		println("Parent mi je shell");
	}
	else
	{
		cprintln(0x02, "Hello World iz programa hello.bin!");
		cvardump(0x02, ppid);
		cprintln(0x02, "Parent mi nije shell");
	}

	

	_exit(0);
}
