#include <string.h>
#include <unistd.h>
#include "../utils.h"

static int a, b, c;

#define BUFFER_SIZE 128

void unos_trougao()
{
	char buffer[BUFFER_SIZE];
	int len;
	printstr("Unesite katetu a: ");
	read(0, buffer, BUFFER_SIZE);
	a = atoi(buffer);
	printstr("Unesite katetu b: ");
	read(0, buffer, BUFFER_SIZE);
	b = atoi(buffer);
	printstr("Unesite hipotenuzu c: ");
	read(0, buffer, BUFFER_SIZE);
	c = atoi(buffer);
}

int obim_trougao(void)
{
	int result;
	__asm__ __volatile__ (
		"addl (b), %%eax;"
		"addl (c), %%eax;"
		: "=a" (result)
		: "a" (a)

	);
	return result;
}

int povrsina_trougao(void)
{
	int result;
	__asm__ __volatile__ (
		"imull %%ebx, %%eax;"
		"shrl $1, %%eax;"
		: "=a" (result)
		: "a" (a), "b" (b)

	);
	return result;

}
