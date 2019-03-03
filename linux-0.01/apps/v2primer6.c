/*
	Ovaj program demonstrira osnovne aritmeticke operacije uz pomoc inline asemblera
*/
#include <string.h>
#include <unistd.h>

#define UTIL_IMPLEMENTATION
#include "utils.h"

#define BUFFER_SIZE 128

#define printstr(x) write(1, x, strlen(x))

/* pomocna funkcija show result koja ispisuje lepo na ekran, postoji radi urednosti koda. */
static void show_result(int x, const char *op, int y, int res)
{
	char buffer[BUFFER_SIZE];
	int len;
	len = itoa(x, buffer);
	write(1, buffer, len);
	printstr(op);
	len = itoa(y, buffer);
	write(1, buffer, len);
	printstr(" = ");
	len = itoa(res, buffer);
	write(1, buffer, len);
	printstr("\n");
}

int main(int argc, char *argv[])
{
	char buffer[BUFFER_SIZE];
	int len, x, y, i, add, sub, mul, quo, rem;
	
	printstr("Unesite dva broja odvojena razmakom: ");
	len = read(0, buffer, BUFFER_SIZE);
	x = atoi(buffer);
	for(i = 0; i < len; ++i)
		if(buffer[i] == ' ') break;
	if(len == i) _exit(1);
	y = atoi(buffer + i + 1);
	
	/* umesto kljucne reci asm mozemo da koristimo makro __asm__ */
	/* radimo osnovne aritmeticke operacije */
	__asm__ ("addl %%ebx, %%eax;" : "=a" (add) : "a" (x), "b" (y));
	__asm__ ("subl %%ebx, %%eax;" : "=a" (sub) : "a" (x), "b" (y));
	__asm__ ("imull %%ebx, %%eax;" : "=a" (mul) : "a" (x), "b" (y));

	/* ako za ulazni ili izlazni argument navedemo opciju "g" onda se on drzi u bilo kom slobodnom registru koji ne utice na izvrsavanje koda */
	__asm__ (
		"movl $0x0, %%edx;"
		"movl %2, %%eax;"
		"movl %3, %%ebx;"
		"idivl %%ebx;"
		: "=a" (quo), "=d" (rem)
		: "g" (x), "g" (y)
		: "%ebx"
	);

	show_result(x, " + ", y, add);
	show_result(x, " - ", y, sub);
	show_result(x, " * ", y, mul);
	show_result(x, " / ", y, quo);
	show_result(x, " %% ", y, rem);

	_exit(0);	
}


