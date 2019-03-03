/* This is a small utilities library for C in linux 0.01 */

/* 
	In the file that will have the implementation use 
	#define UTIL_IMPLEMENTATION
	before including this file
*/

#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <string.h>

/* Base 10 only */
int itoa(int n, char *buf);

/* Parses a number from the provided pointer to the first non-digit character */
int atoi(const char *buf);

#ifdef UTIL_IMPLEMENTATION

#ifndef __isdigit
#define __isdigit(x) ((x >= '0') && (x <= '9'))
#endif
/* isdigit endif*/

static void __reverse(char *buf, int len)
{
	int i, j;
	char c;
	for(i = 0, j = len - 1; i < j; ++i, --j)
	{
		c = buf[i];
		buf[i] = buf[j];
		buf[j] = c;
	}
}

int itoa(int n, char *buf)
{
	int i = 0, sign;
	if((sign = n) < 0)
		n = -n;
	do
	{
		buf[i++] = n % 10 + '0';
	} while((n /= 10) > 0);

	if(sign < 0)
		buf[i++] = '-';
	buf[i] = '\0';
	__reverse(buf, i);
	return i;
}

int atoi(const char *buf)
{
	int r = 0, i;
	for(i = 0; __isdigit(buf[i]); ++i)
		r = r * 10 + buf[i] - '0';
	return r;
}


#endif
/* impl endif*/
#endif
/* headerguard endif */
