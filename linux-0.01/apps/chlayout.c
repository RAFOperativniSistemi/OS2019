#include <string.h>
#include <unistd.h>

#define UTIL_IMPLEMENTATION
#include "utils.h"



int main(char *args)
{
	int normal_key_map, reverse_key_map;
	asm(
		".text;"
		"jmp cdstart;"
		".data;"
		"nmmap:;"
		".byte 0,27;"
		".ascii \"1234567890-=\";"
		".byte 127,9;"
		".ascii \"qwertyuiop[]\";"
		".byte 10,0;"
		".ascii \"asdfghjkl;'\\\\\";"
		".byte 0, 0;"
		".ascii \"\\zxcvbnm,./\";"
		".byte 0,'*,0,32;"
		".fill 16,1,0;"
		".byte '-,0,0,0,'+;"
		".byte 0,0,0,0,0,0,0;"
		".byte '<;"
		".fill 10,1,0;"
		"rvmap:;"
		".byte 0,27;"
		".ascii \"0987654321-=\";"
		".byte 127,9;"
		".ascii \"qwertyuiop[]\";"
		".byte 10,0;"
		".ascii \"asdfghjkl;'\\\\\";"
		".byte 0, 0;"
		".ascii \"\\zxcvbnm,./\";"
		".byte 0,'*,0,32;"
		".fill 16,1,0;"
		".byte '-,0,0,0,'+;"
		".byte 0,0,0,0,0,0,0;"
		".byte '<;"
		".fill 10,1,0;"
		".text;"
		"cdstart:;"
		"movl $nmmap, %%eax;"
		"movl $rvmap, %%ebx;"
		:"=a" (normal_key_map), "=b" (reverse_key_map)
	);

	char *layout_names[] = {"NORMAL_US", "REVERSE_NUMBER_US"};
	char *layouts[] = {(char *)normal_key_map, (char *)reverse_key_map};
	int argc;
	char *opt1;
	char buff[128];

	argc = get_argc(args);
	if(argc < 2)
	{
		printerr("No program args, use option --help\n");
		_exit(1);
	}
	
	opt1 = get_argv(args, 1);
	if(!strcmp(opt1, "--help"))
	{
		printstr("Tool chlayout:\n\nThis tool is used to change the layout of the user defined keyboard layout (layouts[2]). \nPlease use one of the currently available options:\n");
		int i, n = sizeof(layouts) / sizeof(layouts[0]);
		for(i = 0; i < n; ++i)
		{
			itoa(i, buff);
			printstr(buff);
			printstr(": ");
			printstr(layout_names[i]);
			printstr("\n");
		}
	}
	else
	{
		change_user_layout(layouts[atoi(opt1)], KEY_LAYOUT);
	}

	_exit(0);
}
