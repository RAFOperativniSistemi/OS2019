#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define UTIL_IMPLEMENTATION
#include "utils.h"

#define SIZE 20
int size = SIZE;

char buffer[SIZE][SIZE];

int main(int argc, char *argv[])
{

	int i, sum;
	__asm__ __volatile__ (
		"movl (size), %%ebx;"
		"leal buffer, %%edi;"
		"xorl %%ecx, %%ecx;"
		"L1:;"
		"xorl %%edx, %%edx;"
		"movl %%ecx, %%eax;"
		"divl %%ebx;" /* ax je sada red a dx kolona*/
		"cmpl %%edx, %%eax;"
		"je ZVEZDA;"
		"movl $32, %%eax;"
		"jmp CONT;"
		"ZVEZDA:;"
		"movl $42, %%eax;"
		"CONT: stosb;"
		"incl %%ecx;"
		"cmpl $400, %%ecx;"
		"je EXIT;"
		"jmp L1;"
		"EXIT:"
		: "=c" (sum)
		:
		: "%eax", "%edi", "%ebx", "memory"
	);

	for(i = 0; i < SIZE; ++i)
	{
		write(1, buffer[i], SIZE);
		write(1, "\n", 1);
	}
	
	_exit(0);
}
