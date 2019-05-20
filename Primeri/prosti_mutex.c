#include <stdio.h>
#include <pthread.h>

pthread_mutex_t x_lock;
int x = 0;


#define N (5000)
#define M (100000)

void* incrementer(void *args)
{
		int i, j;
		for(i = 0; i < N; i++)
		{
			pthread_mutex_lock(&x_lock);
			for(j = 0; j < M; j++)
			{
				x += 1;
			}
			pthread_mutex_unlock(&x_lock);
		}
}

void* decrementer(void *args)
{
		int i, j;
		for(i = 0; i < N; i++)
		{
			pthread_mutex_lock(&x_lock);
			for(j = 0; j < M; j++)
			{
				x -= 1;
			}
			pthread_mutex_unlock(&x_lock);
		}
}


int main(int argc, char *argv[])
{
		pthread_mutex_init(&x_lock, NULL);
		
		pthread_t t1, t2;
		pthread_create(&t1, NULL, incrementer, NULL);
		pthread_create(&t2, NULL, decrementer, NULL);
		
		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
		
		printf("x: %d\n", x);
}
