/* Ovaj primer demonstrira smestanje input i output argumenata u specificne registre, kao i instrukciju addl */
#include <string.h>
#include <unistd.h>

#define UTIL_IMPLEMENTATION
#include "utils.h"

#define BUFFER_SIZE 128

int main(int argc, char *argv[])
{
	int a = 10, b = 7, res, len;
	char buffer[BUFFER_SIZE];
	
	/* 
		navodimo da se varijabla a smesta u EAX registar a varijavla b u EBX registar
		za output navodimo da se u res varijablu smesta vredenost iz EAX registra
	*/
	asm(
		"addl %%ebx, %%eax;"
		: "=a" (res)		/* Output */
		: "a" (a), "b" (b)	/* Input */
	);

	len = itoa(res, buffer);
	write(1, buffer, len);
	write(1, "\n", strlen("\n"));	

	_exit(0);	
}


