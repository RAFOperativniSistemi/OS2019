/* Ovaj primer demonstrira najosnovniji C program, Hello World */

#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	/* Ispisujemo na stdout(fd = 1) string "Hello, World!\n" */
	write(1, "Hello, World!\n", strlen("Hello, World!\n"));

	/* Izlazak iz programa koji je obavezan. Ako ne postoji ceo sistem crash-uje */
	_exit(0);
}
