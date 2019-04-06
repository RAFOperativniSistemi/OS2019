#define __LIBRARY__
#include <unistd.h>

#include <videowrite.h>

_syscall3(int,videowrite,const char *,buffer,int,len,struct videowrite_args *,args)
