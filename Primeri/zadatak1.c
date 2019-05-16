#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "codon.h"

void bubble(int arr[], int n, int (*cmp_fptr)(int, int))
{
	int sorted = 0, i, tmp;
	while(!sorted)
	{
		sorted = 1;
		for(i = 0; i < n - 1; ++i)
		{
			if(cmp_fptr(arr[i], arr[i + 1]))
			{
				tmp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = tmp;
				sorted = 0;
			}
		}
	}

}

int cmp_a(int x, int y)
{
	return x > y;
}

struct thread_args
{
	char *s;
	int pos, len;
};

int analyze(char *s, int n, int threads);
void *analyzer_thread(void *_args);


int main(int argc, char *argv[])
{
	int n;
	printf("Unesite broj thread-ova: ");
	scanf("%d", &n);

	analyze(test_codon, strlen(test_codon), n);

	exit(0);
}

int array_len(int *arr)
{
	int i = 0;
	while(arr[i] != -1)
		i++;
	return i;
	
}

void show(int arr[], int n)
{
	int i;
	for(i = 0; i < n; ++i)
	{
		printf("%5d ", arr[i]);
		if(!(i%12)) printf("\n");
	}
	printf("\n");

}


int analyze(char *s, int len_str, int nthreads)
{
	pthread_t threads[nthreads];
	struct thread_args args[nthreads];
	int i, alen = len_str/nthreads;

	for(i = 0; i < nthreads; i++)
	{
		args[i] = (struct thread_args) {s, i*alen, alen};
		if(i == nthreads - 1)
		{
			args[i].len += len_str % nthreads;
		}
		pthread_create(&threads[i], NULL, analyzer_thread, &args[i]);
		
	}

	int *thread_returns[nthreads];
	int pom, sum = 0, sizes[nthreads];

	for(i = 0; i < nthreads; i++)
	{
		pthread_join(threads[i], (void**) (&thread_returns[i]));
		pom = array_len(thread_returns[i]);
		sum += pom;
		sizes[i] = pom;
	}

	int final_array[sum];

	int j, k = 0;
	for(i = 0; i < nthreads; i++)
	{
		for(j = 0; j < sizes[i]; j++)
		{
			final_array[k++] = thread_returns[i][j];
		}
		free(thread_returns[i]);
	}

	bubble(final_array, sum, cmp_a);
	show(final_array, sum);

	
	


}
void *analyzer_thread(void *_args)
{
	struct thread_args *args = (struct thread_args *) _args;
	int len = args->len;
	int pos = args->pos;
	char *s = args->s;
	int i, k = 0;

	int *niz = malloc((len / 3 + 5) * sizeof(int));

	for(i = 0; i < len; i++)
	{
		if(*(s+pos+i+1) == '\0' || *(s+pos+i+2) == '\0')
			break;
		if(!strncmp(s + pos + i, "taa", 3) || !strncmp(s + pos + i, "tga", 3) || !strncmp(s + pos + i, "tag", 3))
		{
			//printf("%d\n", pos + i);
			niz[k++] = pos + i;
		}

	}
	niz[k] = -1;
	pthread_exit((void *)(niz));	
}








