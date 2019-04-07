#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define UTIL_IMPLEMENTATION
#include "utils.h"

char filebuff[512];
char filenamebuff[256];

int main(char *args)
{

	int argc, i, fd;
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
		printstr("Tool chlayout:\n\nThis tool is used to change a part of user defined keyboard layout.\nOptions:\n\tKey table: -k file\n\tShift table: -s file\n\tAlt table: -a file\n\nUsage:\nchlayout -opt filename\n\n");

	}
	else
	{

		if(argc < 3)
		{
			printerr("No program args, use option --help\n");
			_exit(1);
		}
		int doflag;
		for(i = 1; i < argc; ++i)
		{
			opt1 = get_argv(args, i);
			doflag = -1;
			if(!strcmp(opt1, "-k"))
			{
				doflag = KEY_LAYOUT;
			}
			else if(!strcmp(opt1, "-s"))
			{
				doflag = SHIFT_LAYOUT;
			}
			else if(!strcmp(opt1, "-a"))
			{
				doflag = ALT_LAYOUT;
			}
			if(doflag != -1)
			{
				if(i + 1 >= argc)
				{
					printerr("Argument missing for: ");
					printerr(opt1);
					printerr("\n");
					_exit(1);
				}
				strcpy(filenamebuff, get_argv(args, ARG_PWD));
				if(strlen(filenamebuff) != 1) strcat(filenamebuff, "/");
				strcat(filenamebuff, get_argv(args, i + 1));
				fd = open(filenamebuff, O_RDONLY);
				if(fd < 0)
				{
					printerr("Could not open file!\n");
					printerr("Tried to open: ");
					printerr(filenamebuff);
					_exit(1);
				}
				read(fd, filebuff, 512);
				change_user_layout(filebuff, doflag);
				close(fd);
				i += 1;

			}
		}
		
	}

	_exit(0);
}
