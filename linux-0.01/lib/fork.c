#define __LIBRARY__
#include <unistd.h>

_syscall0(int,fork)
_syscall0(int,getpid)
_syscall0(int,getppid)


_syscall1(int,nice,int,niceness)
_syscall1(int,delayed_exit,int,when);
//_syscall3(int,execve,const char *,filename,char **,argv,char **,envp)
