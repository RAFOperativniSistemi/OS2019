#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define UTIL_IMPLEMENTATION
#include "utils.h"

#define BUFFER_SIZE 128
#define ARRAY_SIZE 128

int main(int argc, char *argv[])
{
	int len, n, x, i, sum;
	char buffer[BUFFER_SIZE];
	int array[ARRAY_SIZE];

	int fd = open("ulaz.txt", O_RDONLY);

	if(fd == -1)
	{
		printerr("Fajl neuspesno otvoren!\n");
		_exit(1);
	}

	len = fgets(buffer, BUFFER_SIZE, fd);
	
	n = atoi(buffer);
	if(n > ARRAY_SIZE)
	{
		printerr("Prevelik broj elemenata niza!\n");
		_exit(1);
	}
	
	/* ucitavanje */
	for(i = 0; i < n; ++i)
	{
		len = fgets(buffer, BUFFER_SIZE, fd);
		x = atoi(buffer);
		array[i] = x;
	}	

	/* assembly block koji racuna sumu */

	__asm__ __volatile__ (
		"xorl %%edx, %%edx;"
		"MORE: lodsl;"
		"addl %%eax, %%edx;"
		"loop MORE;"
		: "=d" (sum)
		: "S" (array), "c" (n)
		: "%eax"
	);

	/* vardump(sum); */

	close(fd);
	_exit(0);
}
