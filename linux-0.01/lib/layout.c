#define __LIBRARY__
#include <unistd.h>

_syscall2(int,change_user_layout,const char *,layout,int,map)
