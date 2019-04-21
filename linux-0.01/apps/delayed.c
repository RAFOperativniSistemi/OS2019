#include <string.h>
#include <unistd.h>

#define UTIL_IMPLEMENTATION
#include "utils.h"

int main(char *args)
{
	char c = 'a';
	println("Starting!");
	delayed_exit(500);

	while(1)
	{
		if(c == 'z') c = 'a';
		else c++;
		write(1, &c, 1);
	}

	

	//_exit(0);
}
