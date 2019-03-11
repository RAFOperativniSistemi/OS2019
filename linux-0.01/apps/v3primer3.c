#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define UTIL_IMPLEMENTATION
#include "utils.h"

#define SIZE 22
int size = SIZE;
int size_half = (int)(SIZE / 2) - (1 - SIZE % 2) - 1;
int size_third = (int)(SIZE / 3) + (SIZE % 3 > 0);
int size_end = SIZE * SIZE;

int r;

char buffer[SIZE][SIZE];

void printb(int b)
{
	vardump(b);
}

int main(int argc, char *argv[])
{
	/* za random */
	__asm__ __volatile__ (
		"rdtsc;"
		: "=a" (r)
		:
		: "%edx"
	);
	/* niz stringova */
	/* ovo zapravo ne kreira matricu karaktera */
	/* s je niz pokazivaca, a same vrednosti se drze */
	/* na statickom bloku, string koji se pojavljuje vise puta */
	/* u nizu se alocira samo jedanput pa se koristi isti pokazivac*/
	char *s[] = {
		"\n Tap to Jingle Bells!\n",
		"\n     Jin\n",
		"\n     Jingle\n",
		"\n     Jingle bells!\n",
		"\n     Jin\n",
		"\n     Jingle\n",
		"\n     Jingle bells!\n",
		"\n  Jin\n",
		"\n  Jingle\n",
		"\n  Jingle all\n",
		"\n  Jingle all the\n",
		"\n  Jingle all the way!\n",
		"\n     Oh\n",
		"\n     Oh what\n",
		"\n     Oh what fun,\n",
		"\n    It\n",
		"\n    It is\n",
		"\n    It is to\n",
		"\n    It is to ride,\n",
		"\n   In\n",
		"\n   In a\n",
		"\n   In a one\n",
		"\n   In a one-horse\n",
		"\n     open\n",
		"\n     open sleigh!\n",
		"\n       Again!\n",
		"\n     Jin\n",
		"\n     Jingle\n",
		"\n     Jingle bells!\n",
		"\n     Jin\n",
		"\n     Jingle\n",
		"\n     Jingle bells!\n",
		"\n  Jin\n",
		"\n  Jingle\n",
		"\n  Jingle all\n",
		"\n  Jingle all the\n",
		"\n  Jingle all the way!\n",
		"\n     Oh\n",
		"\n     Oh what\n",
		"\n     Oh what fun,\n",
		"\n    It\n",
		"\n    It is\n",
		"\n    It is to\n",
		"\n    It is to ride,\n",
		"\n   In\n",
		"\n   In a\n",
		"\n   In a one\n",
		"\n   In a one-horse\n",
		"\n     open\n",
		"\n     open sleigh!\n",
		"\n\n",
		"\n   HAPPY CHRISTMAS!\n",
		"\n     AND NEW YEAR!\n"
	};
	int num_lines = sizeof(s) / sizeof(s[0]); /* racunamo koliko ima stringova */
	int j;
	for(j = 0; j < num_lines; j++)
	{

		int i, sum, times = 0;
		__asm__ __volatile__ (
			"movl $1, %%ebx;"
			"leal buffer, %%edi;"
			"xorl %%ecx, %%ecx;"
			"movl $-3, %%eax;"
			"L1:;"
			"xorl %%edx, %%edx;"
			"pushl %%eax;"
			"movl %%ecx, %%eax;"
			"pushl %%ebx;"
			"movl (size), %%ebx;"
			"divl %%ebx;" /* posle ove div instrukcije, dx sadrzi trenutnu kolonu */
			"popl %%ebx;" /* eax i ebx odrzavamo zbog generisanja drveta */
			"popl %%eax;"

			"pushl %%edx;"
			"pushl %%eax;"
			"pushl %%ebx;"
			"movl %%ecx, %%eax;"
			"movl (size), %%ebx;"
			"divl %%ebx;"

			"popl %%ebx;"
			"popl %%eax;"

			"cmpl $0, %%edx;"
			"jne NOINC;"
			"incl %%eax;"		/* svaki novi red povecavamo ax (debljina drveta) */
			"cmpl %%eax, %%ebx;"
			"jne NOINC;"		/* kada ax sustigne bx, resetujemo ga*/
			"movl $1, %%eax;"   	/* na 1 da bi dobili stepenasto drvo */
			"addl $2, %%ebx;"	/* a bx povecavamo da bi svaki sledeci stepen */
			"NOINC:;"		/* bio siri */
			"popl %%edx;"
		
			"pushl %%eax;"

			"cmpl (size_third), %%ebx;"
			"jbe NOTRUNK;"
			"movl $1, %%eax;"
			"NOTRUNK:;"

		
			"pushl %%eax;"
			"pushl %%ebx;"
			"movl (size_half), %%ebx;"
			"subl %%eax, %%ebx;"
			"movl %%ebx, %%eax;"
			""
			"popl %%ebx;"

			"cmpl %%edx, %%eax;"
			"jl DRVO;" 

			/* ako kolona nije stigla do leve strane drveta */
			/* tj nebo sa leve strane */


			"SPACE:;"	/* SPACE ce da ispise ili prazno mesto ili zvezdu (tacku) */
			"pushl %%ebx;"
			"pushl %%edx;"
			"pushl %%ecx;"

			/* iterativno generisanje random broja r*/
			"movl (r), %%eax;"
			"imull $7621, %%eax;"
			"addl $1, %%eax;"
			"movl $32768, %%ebx;"
			"xorl %%edx, %%edx;"
			"idivl %%ebx;"
			"movl %%edx, (r);"

			"movl %%edx, %%eax;"
			"xorl %%edx, %%edx;"
			
			
			"movl $12, %%ebx;"
			"idivl %%ebx;"

			/* slicno kao rand() % 12 == 0 uslov */
			"cmp $0, %%edx;"
			"jne NODOT;"
			"movl $46, %%eax;" /* ima zvezda na nebu (tacka) */
			"jmp DOT;"
			"NODOT: movl $32, %%eax;" /* nema zvezda na nebu (tacka) */
			"DOT:"

			"popl %%ecx;"
			"popl %%edx;"
			"popl %%ebx;"
			"jmp CONT;"

			"DRVO:;"
			"popl %%eax;"	/* pop ax pa push ax */
			"pushl %%eax;" 
			"addl (size_half), %%eax;"
			"incl %%eax;"
			"cmpl %%edx, %%eax;"
			"jl SPACE;" /* nebo sa desne strane */
			"movl $42, %%eax;"
			"CONT: stosb;" /* stosb smesta karakter iz al gde edi pokazuje i pomera edi za 1 bajt*/
			"popl %%eax;"
			"popl %%eax;"
			"incl %%ecx;"
			"cmpl (size_end), %%ecx;"
			"je EXIT;"
			"jmp L1;"
			"EXIT:"
			: "=c" (sum)
			:
			: "%eax", "%edi", "%ebx", "memory"
		);

	
		printstr(s[j]);
		for(i = 0; i < SIZE; ++i)
		{
			write(1, buffer[i], SIZE);
			write(1, "\n", 1);
		}
		for(i = 0; i < SIZE; ++i)
			write(1, "#", 1);
		printstr("\n");
		pause();
	}
	
	_exit(0);
}
