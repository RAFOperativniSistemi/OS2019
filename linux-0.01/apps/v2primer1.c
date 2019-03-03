/* Ovaj primer pokazuje osnove rada sa pokazivacima na funkcije */
#include <string.h>
#include <unistd.h>

#define UTIL_IMPLEMENTATION
#include "utils.h"

void test(int x)
{
	char buffer[32];
	int len;
	write(1, "Prosledjen broj: ", strlen("Prosledjen broj: "));
	len = itoa(x, buffer);
	write(1, buffer, len);
	write(1, "!\n", strlen("!\n"));
}

void pretty_test(int x)
{
	char buffer[32];
	int len;
	write(1, "Prosledjen broj: *** ", strlen("Prosledjen broj: *** "));
	len = itoa(x, buffer);
	write(1, buffer, len);
	write(1, "! ***\n", strlen("! ***\n"));
}


int main(int argc, char *argv[])
{
	/* 
		Deklarisemo pokazivac na funkciju sa potpisom
		void fptr(int);
		i inicijalizujemo ga na funkciju test.
	*/	
	void (*fptr)(int) = test;
	
	/* Pozivamo funkciju na koju pokazuje fptr */
	fptr(10);
	
	/* Menjamo na sta pokazuje fptr */
	fptr = pretty_test;

	/* Pozivamo novu funkciju na koju pokazuje fptr */
	fptr(10);

	_exit(0);	
}


