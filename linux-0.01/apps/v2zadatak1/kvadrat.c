#include <string.h>
#include <unistd.h>
#include "../utils.h"

static int a;

#define BUFFER_SIZE 128

void unos_kvadrat()
{
	char buffer[BUFFER_SIZE];
	int len;
	printstr("Unesite stranicu kvadrata: ");
	read(0, buffer, BUFFER_SIZE);
	a = atoi(buffer);

}

int obim_kvadrat(void)
{
	int result;
	__asm__ __volatile__ (
		"imull $4, %%eax;"
		: "=a" (result)
		: "a" (a)

	);
	return result;
}

int povrsina_kvadrat(void)
{
	int result;
	__asm__ __volatile__ (
		"imull %%eax, %%eax;"
		: "=a" (result)
		: "a" (a)

	);
	return result;

}
