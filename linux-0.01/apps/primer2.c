/* Ovaj primer demonstrira ucitavanje jednog stringa u buffer i njegovo ispisivanje nazad korisniku */

#include <string.h>
#include <unistd.h>

#define MAX_LEN 100

int main(int argc, char *argv[])
{
	char buffer[MAX_LEN];
	int len;

	/* Citanje stringa do karaktera \n sa stdin(fd = 0). Ucitava string u buffer, maksimalno ucitano MAX_LEN karaktera */
	len = read(0, buffer, MAX_LEN);

	write(1, buffer, len);

	_exit(0);

}
