#include <string.h>
#include <unistd.h>
#include <videowrite.h>

int main(char *args)
{
	struct videowrite_args wrargs = { {40, 10}, (short) 0x02};
	videowrite("Hello, Green World!", strlen("Hello, Green World!"), &wrargs);

	_exit(0);

}
