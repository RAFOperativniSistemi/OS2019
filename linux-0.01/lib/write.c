#define __LIBRARY__
#include <unistd.h>

_syscall3(int,write,int,fd,const char *,buf,off_t,count)

//int col_write(int colour,const char *buff, int len);
_syscall3(int,col_write,int,colour,const char *,buff,int,len)

