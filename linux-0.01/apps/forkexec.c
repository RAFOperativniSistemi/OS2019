#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define UTIL_IMPLEMENTATION
#include "utils.h"

static char * argv[] = { "/root/hello.bin",NULL };
static char * envp[] = { "HOME=/root","PATH=/root","PWD=/", NULL };


int main(char *args)
{
	int child_wstatus;
	int child_pid;
	if((child_pid=fork()) == 0)
	{
		cprintln(0x02, "Ispis iz child procesa");
		cvardump(0x02, getpid());
		cprintln(0x02, "Menjamo child proces novim procesom!");
		execve("/root/hello.bin", argv, envp);
	}
	else
	{
		nice(10);
		println("Ispis iz parent procesa!");
		vardump(child_pid);
		waitpid(-1, &child_wstatus, 0);
		if(WIFEXITED(child_wstatus)))
		{
			println("Child exited correctly!");
		}
	}


	_exit(0);
}
