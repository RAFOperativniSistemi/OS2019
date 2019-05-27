#include <stdio.h> 
#include <pthread.h> 
#include <stdlib.h>

#define THREADS 4

struct thread_args
{
	int id;
	int start, len;
	int *arr;
	pthread_barrier_t *barrier;
	int (*should_swap)(int, int);
};

int cmp_a(int x, int y)
{
	if(x > y)
		return 1;
	else
		return 0;
}

int cmp_d(int x, int y)
{
	if(x < y)
		return 1;
	else
		return 0;
}

int bubble(int arr[], int start, int len, int (*should_swap)(int, int))
{
	int i, sorted = 0, tmp;

	while(!sorted)
	{
		sorted = 1;
		for(i = start; i < len - 1; i++)
		{
			if(should_swap(arr[i], arr[i + 1]))
			{
				tmp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = tmp;
				sorted = 0;
			}
		}

	}
	return 0;
}

int reverse_bubble(int arr[], int start, int len, int (*should_swap)(int, int))
{
	int i, sorted = 0, tmp;

	while(!sorted)
	{
		sorted = 1;
		for(i = len - 2; i >= start; i--)
		{
			if(should_swap(arr[i], arr[i + 1]))
			{
				tmp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = tmp;
				sorted = 0;
			}
		}

	}
	return 0;
}

void merge(int arr[], int start1, int len1, int start2, int len2, int (*should_swap)(int, int))
{
	int left[len1];
	int right[len2];
	int i, l, r;
	for(i = 0; i < len1; i++)
		left[i] = arr[start1 + i];

	for(i = 0; i < len2; i++)
		right[i] = arr[start2 + i];

	l = 0;
	r = 0;

	for(i = 0; i < len1 + len2; i++)
	{

		if(l >= len1)
		{
			arr[start1 + i] = right[r++];
			continue;
		}

		if(l >= len2)
		{
			arr[start1 + i] = left[l++];
			continue;
		}

		if(should_swap(left[l], right[r]))
		{
			arr[start1 + i] = right[r++];
		}
		else
		{
			arr[start1 + i] = left[l++];
		}
	}
	
}

void *sorter_thread(void *_args)
{
	struct thread_args *args = (struct thread_args *) _args;
	int id = args->id;
	int start = args->start;
	int len = args->len;
	int *arr = args->arr;
	pthread_barrier_t *barrier = args->barrier;
	
	bubble(arr, start, len, args->should_swap);

	pthread_barrier_wait(barrier);

	if(id % 2 == 0)
		merge(arr, start, len, start + len, len, args->should_swap);
	
}

int multithreaded_sort(int arr[], int n, int nthreads, int (*should_swap)(int, int))
{
	nthreads += nthreads % 2;
	pthread_t threads[nthreads];
	struct thread_args arguments[nthreads];

	pthread_barrier_t barrier;
	pthread_barrier_init(&barrier,NULL,nthreads);

	int partlen = n / nthreads, i;

	for(i = 0; i < nthreads; i++)
	{
		arguments[i] = (struct thread_args) {i, i * partlen, partlen, arr, &barrier, should_swap};
		if(i == nthreads - 1)
			arguments[i].len += n % nthreads;
		pthread_create(&threads[i], NULL, sorter_thread, (void*)(&arguments[i]));
	}

	for(i = 0; i < nthreads; i++)
	{
		pthread_join(threads[i],NULL);
	}

	reverse_bubble(arr, 0, n, cmp_a);

	pthread_barrier_destroy(&barrier);

}

int show(int arr[], int n)
{
	int i;
	for(i = 0; i < n; i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");

}

int main(int argc, char *argv[])
{

	int arr[] = {3, 7, 12, 1, 17, -3, 29, -32, 44, 11, 28, 352, 400, 217, 65, 72, 44, 13, 2};
	int n = sizeof(arr) / sizeof(arr[0]);

	show(arr, n);
	multithreaded_sort(arr, n, THREADS, cmp_a);
	show(arr, n);
	
	
}
