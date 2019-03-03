#include <string.h>
#include <unistd.h>

#define UTIL_IMPLEMENTATION
#include "utils.h"

#define BUFFER_SIZE 128

#define printstr(x) write(1, x, strlen(x))

/* Euklidov algoritam za najveci zajednicki deilac */
int gcd(int a, int b)
{
	int result;
	/* Volatile stavljamo kada zelimo da kompajler preskoci optimizacije koda */
	/* mozemo da koristimo ili kljucnu rec volatile ili makro __volatile__ */
	/* isto tako mozemo da koristimo ili kljucnu rec asm ili makro __asm__ */
	/* 
	   Kod za euklidov algoritam bi u samo C-u bio:
	if(b == 0) return a;
	return gcd(b, a % b);		
	
	*/
	__asm__ __volatile__ (
		"movl %1, %%eax;"
		"movl %2, %%ebx;"
		"CONTD: cmpl $0, %%ebx;"
		"je DONE;"
		"xorl %%edx, %%edx;"
		"idivl %%ebx;"
		"movl %%ebx, %%eax;"
		"movl %%edx, %%ebx;"
		"jmp CONTD;"
		"DONE: movl %%eax, %0;"
		: "=g" (result)
		: "g" (a), "g" (b)
	);
	return result;
}

int main(int argc, char *argv[])
{

	char buffer[BUFFER_SIZE];
	int len, x, y, i;
	
	printstr("Unesite dva broja odvojena razmakom: ");
	len = read(0, buffer, BUFFER_SIZE);
	x = atoi(buffer);
	for(i = 0; i < len; ++i)
		if(buffer[i] == ' ') break;
	if(len == i) _exit(1);
	y = atoi(buffer + i + 1);
	
	printstr("NZD ova dva broja je ");
	len = itoa(gcd(x, y), buffer);
	buffer[len++] = '\n';
	write(1, buffer, len);
	
	_exit(0);	
}


