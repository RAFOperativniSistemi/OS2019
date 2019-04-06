#include <string.h>
#include <unistd.h>

int main(char *args)
{

	write(1, "Hello, World!\n", strlen("Hello, World!\n"));
	_exit(0);
}
