/* This is a small utilities library for C in linux 0.01 */

/* 
	In the file that will have the implementation use 
	#define UTIL_IMPLEMENTATION
	before including this file
*/

#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <string.h>

/* Standards for get_argv */
#define ARG_PATH -1
#define ARG_PWD -2
#define ARG_SHLVL -3
#define ARG_HOME -4
#define ARG_EXECUTABLE -5

/* 
	In order to use these two functions, main should have a declaration like: 
		int main(char *);
*/
/* Gets the number of program arguments */
int get_argc(char *args);
/* Gets the program argument argnum */
char *get_argv(char *args, int argnum);

/* Base 10 only */
int itoa(int n, char *buf);

/* Parses a number from the provided pointer to the first non-digit character */
int atoi(const char *buf);

/* Pauses execution until enter is pressed */
void pause(void);

/* Reads from file fd until it hits maxlen or \n */
int fgets(char *buffer, int maxlen, int fd);

inline int printstr(char *s);
inline int printerr(char *s);
inline int println(char *s);


#define vardump(x) \
{\
	char __vardump_buff[128];\
	int __vardump_len;\
	__vardump_len = itoa(x, __vardump_buff);\
	write(1, #x ": ", strlen(#x ": "));\
	write(1, __vardump_buff, __vardump_len);\
	write(1, "\n", 1);\
}


#ifdef UTIL_IMPLEMENTATION

#ifndef __isdigit
#define __isdigit(x) ((x >= '0') && (x <= '9'))
#endif
/* isdigit endif */

inline int printstr(char *s)
{
	return write(1, s, strlen(s));
}

inline int printerr(char *s)
{
	return write(2, s, strlen(s));
}

inline int println(char *s)
{
	return printstr(s) + printstr("\n");
}

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

void pause()
{
	char b[10];
	printstr("Press enter to continue...");
	read(0, b, 10);
}

int fgets(char *buffer, int maxlen, int fd)
{
	int i = 0;
	char c;
	do
	{
		if(!read(fd, &c, 1) || i == maxlen - 1) break;
		buffer[i++] = c;	

	} while(c != '\n' && c!='\0');
	buffer[i] = '\0';
	return i;
}

int get_argc(char *args)
{
	int r = 0, i = 0;
	while(strncmp(args + i, "PATH=", 5))
	{
		while(args[i++]);
		r++;
	}
	return r;
}

char *get_argv(char *args, int argnum)
{
	int i = 0, __tmp;
	static int __offset[] = {5, 4, 6, 5, 2};
	if(argnum >= 0) while(argnum--) while(args[i++]);
	else
	{
		__tmp = argnum = -argnum - 1;
		while(strncmp(args + i, "PATH=", 5)) while(args[i++]);
		while(argnum--) while(args[i++]);
		return args + i + __offset[__tmp];
	}	
	return args + i;
} 


#endif
/* impl endif */
#endif
/* headerguard endif */
