#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define UTIL_IMPLEMENTATION
#include "utils.h"

int main(char *args)
{
	int cpid1, cpid2;
	int argc = get_argc(args);
	int delta_nice = 10;
	if(argc == 2)
	{
		delta_nice = atoi(get_argv(args, 1));
	}
	

	if((cpid1=fork()) == 0)
	{
		cprintln(0x04, "Child 1 speaking!");
		_exit(0);
	}

	nice(delta_nice);

	if((cpid1=fork()) == 0)
	{
		cprintln(0x02, "Child 2 speaking!");
		_exit(0);
	}

	_exit(0);
}
