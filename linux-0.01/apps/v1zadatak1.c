#include <string.h>
#include <unistd.h>

#define UTIL_IMPLEMENTATION
#include "utils.h"

#define BUFFER_SIZE 128

int main(int argc, char *argv[])
{
	char buffer[BUFFER_SIZE];
	int len, x, y, i;
	
	len = read(0, buffer, BUFFER_SIZE);
	
	x = atoi(buffer);

	for(i = 0; i < len; ++i)
		if(buffer[i] == ' ') break;

	if(len == i)
	{
		write(1, "Space not found!\n", strlen("Space not found!\n"));
		_exit(1);
	}
	
	y = atoi(buffer + i + 1);

	len = itoa(x + y, buffer);

	buffer[len++] = '\n';

	write(1, buffer, len);
	

	_exit(0);
}
