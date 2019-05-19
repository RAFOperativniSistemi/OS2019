#include <stdio.h>
#include <pthread.h>

int x = 0;

void* incrementer(void *args)
{
		int i;
		for(i = 0; i < 100000; i++)
		{
				x++;
		}
}

void* decrementer(void *args)
{
		int i;
		for(i = 0; i < 100000; i++)
		{
				x--;
		}
}

int main(int argc, char *argv[])
{
		pthread_t t1, t2;
		pthread_create(&t1, NULL, incrementer, NULL);
		pthread_create(&t2, NULL, decrementer, NULL);
		
		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
		
		printf("x: %d\n", x);
}