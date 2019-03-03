/* Ovaj primer demonstrira cuvanje pokazivaca na funkcije u jednom nizu i onda kasniji odabir i primena jedne od tih funkcija */
#include <string.h>
#include <unistd.h>

#define UTIL_IMPLEMENTATION
#include "utils.h"

#define printstr(x) write(1, x, strlen(x))
#define printerr(x) write(2, x, strlen(x))

#define BUFFER_SIZE 128

int saberi(int x, int y)
{
	return x + y;
}

int oduzmi(int x, int y)
{
	return x - y;
}

int pomnozi(int x, int y)
{
	return x * y;
}

int main(int argc, char *argv[])
{
	/* 
		Deklarisemo niz pokazivaca na funkcijue sa potpisom
		int fptr(int, int);
		i inicijalizujemo ga.
	*/	
	int (*fptr_array[])(int, int) = {saberi, oduzmi, pomnozi};

	char buffer[BUFFER_SIZE];
	int len, x, y, i, op, r;
	
	printstr("Unesite dva broja odvojena razmakom: ");
	len = read(0, buffer, BUFFER_SIZE);
	x = atoi(buffer);
	for(i = 0; i < len; ++i)
		if(buffer[i] == ' ') break;
	if(len == i) _exit(1);
	y = atoi(buffer + i + 1);
	printstr("Unesite kod operacije koju zelite\n(1 - sabiranje, 2 - oduzimanje, 3 - mnozenje)\n");
	
	len = read(0, buffer, BUFFER_SIZE);
	op = atoi(buffer);

	if(op < 1 || op > 3)
	{
		printerr("Unet nepodrzan kod operacije!\n");
		_exit(1);
	}

	/* Uzimamo trazenu funkciju i pozivamo je iz niza pokazivaca */
	r = fptr_array[op - 1](x, y);
	len = itoa(r, buffer);
	buffer[len++] = '\n';

	printstr("Rezultat je ");
	write(1, buffer, len);

	_exit(0);	
}


