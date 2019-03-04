
#include <string.h>
#include <unistd.h>

//u counter.h se nalaze deklaracije za funkcije koje koristimo
#include "counter.h"

#include "../utils.h"

#define BUFFER_SIZE 128

int main(int argc, char** argv)
{
	char buf[BUFFER_SIZE];
	int len;

	print_counter(); //print 0
	increment_counter();
	increment_counter();
	decrement_counter();
	print_counter(); //print 1

	//counter++; //ovaj counter je sakriven
	glob_counter++; //mozemo i direktno da pristupimo promenljivoj jer .h sadrzi extern deklaraciju
	glob_counter++;

	len=itoa(glob_counter, buf);

	buf[len++] = '\n';
	write(1, buf, len); //print 2


	_exit(0);
}
