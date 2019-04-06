#ifndef VIDEOWRITE_H_INCLUDED
#define VIDEOWRITE_H_INCLUDED

struct position
{
	int x, y;
};

struct videowrite_args
{
	struct position pos;
	short col;
};


#endif
