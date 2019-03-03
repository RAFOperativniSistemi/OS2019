/* Ovaj primer demonstrira najosnovniju upotrebu inline asemblera kopirajuci sadrzaj varijable a u varijablu b*/
#include <string.h>
#include <unistd.h>

#define UTIL_IMPLEMENTATION
#include "utils.h"

#define BUFFER_SIZE 128

int main(int argc, char *argv[])
{
	int a = 10, b = 0, len;
	char buffer[BUFFER_SIZE];
	
	/* 
	Koristimo a kao input varijablu i sa "r" navodimo
	da se smesta u bilo koji registar opste namene
	Slicno tako navodimo varijablu b kao output

	Takodje moramo ekslplicitno da navedemo sta je izmenjeno od registara
	pod Clobbered registers
	*/
	asm(
		"movl %1, %%eax;"
		"movl %%eax, %0;"
		: "=r" (b)		/* Output */
		: "r" (a)		/* Input */
		: "%eax"		/* Clobbered registers */

	);

	len = itoa(b, buffer);
	write(1, buffer, len);
	write(1, "\n", strlen("\n"));	

	_exit(0);	
}


