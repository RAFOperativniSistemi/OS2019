#include <stdio.h>
#include <sys/types.h>

int main(char *args)
{
	pid_t pid1, pid2;
	printf("A");
	pid1 = fork();
	if(pid1 == 0) pid2 = fork();
	else wait(NULL);
	while(1) { }
	return 0;
}
