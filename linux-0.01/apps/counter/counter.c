#include <string.h>
#include <unistd.h>

#define UTIL_IMPLEMENTATION
#include "../utils.h"

#define BUFFER_SIZE 128

static int counter;  //"counter" ce biti sakriven, cak i ako pokusamo da mu pristupimo pomocu extern u counter_test.c
int glob_counter; //ovde se alocira memorija za "glob_counter" promenljivu

void increment_counter()
{
	counter++;
}

void decrement_counter()
{
	counter--;
}

void print_counter()
{
	char buffer[BUFFER_SIZE];
	int len;

	len = itoa(counter, buffer);

	buffer[len++] = '\n';

	write(1, buffer, len);
}

