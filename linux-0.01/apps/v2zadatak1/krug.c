#include <string.h>
#include <unistd.h>
#include "../utils.h"

static int a;

#define BUFFER_SIZE 128

void unos_krug()
{
	char buffer[BUFFER_SIZE];
	int len;
	printstr("Unesite poluprecnik: ");
	read(0, buffer, BUFFER_SIZE);
	a = atoi(buffer);

}

int obim_krug(void)
{
	int result;
	__asm__ __volatile__ (
		"shll $1, %%eax;"
		"imull $3, %%eax;" /* PI = 3*/
		: "=a" (result)
		: "a" (a)

	);
	return result;
}

int povrsina_krug(void)
{
	int result;
	__asm__ __volatile__ (
		"imull %%eax, %%eax;"
		"imull $3, %%eax;" /* PI = 3*/
		: "=a" (result)
		: "a" (a)

	);
	return result;

}
