#include <string.h>
#include <unistd.h>

/* util implementation ide samo u 1 .c fajl */
#define UTIL_IMPLEMENTATION
#include "../utils.h"
#include "oblici.h"

#define BUFFER_SIZE 128

int main(int argc, char *argv[])
{
	void (*unosf[])(void) = {unos_kvadrat, unos_krug, unos_trougao};
	int (*obimf[])(void) = {obim_kvadrat, obim_krug, obim_trougao};
	int (*povrsinaf[])(void) = {povrsina_kvadrat, povrsina_krug, povrsina_trougao};
	
	char buffer[BUFFER_SIZE];
	int len, obim, povrsina, op;
	printstr("Unesite koju figuru zelite!\n(1 - kvadrat, 2 - krug, 3 - trougao)\n");
	read(0, buffer, BUFFER_SIZE);
	op = atoi(buffer);
	if(op < 1 || op > 3) _exit(0);
	unosf[op-1]();
	obim = obimf[op-1]();
	povrsina = povrsinaf[op-1]();

	vardump(obim);
	vardump(povrsina);

	_exit(0);
}
