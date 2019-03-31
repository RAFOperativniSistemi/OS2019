#include <string.h>
#include <unistd.h>

#define UTIL_IMPLEMENTATION
#include "utils.h"

int main(int argc, char *argv[])
{
	int a = 10, b = 20, r;
	
	int res = OS2019fnc(a, b, &r);

	vardump(r);
	vardump(res);
	
	_exit(0);
}
