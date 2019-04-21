#include <stdio.h>
#include <sys/types.h>

int main(char *args)
{
	pid_t pid;
	printf("A");
	pid = fork();
	if(pid == 0)
	{
		fflush(stdout);
	}
	else
	{
		wait(NULL);
		printf("\n");
	}
	return 0;
}
