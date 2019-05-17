#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "codon.h"

struct thread_args
{
	char *s;
	int pos, len;
};

int prelazi[5][128];

void init_prelazi(void)
{
	static int can_run = 1;
	if(!can_run)
		return;
	can_run = 0;

	prelazi[0]['t'] = 1;
	
	prelazi[1]['a'] = 2;
	prelazi[1]['g'] = 3;
	prelazi[1]['t'] = 1;

	prelazi[2]['a'] = 4;
	prelazi[2]['g'] = 4;
	prelazi[2]['t'] = 1;

	prelazi[3]['a'] = 4;
	prelazi[3]['t'] = 1;

	prelazi[4]['t'] = 1;


}

int cmp_a(int x, int y)
{
	return x > y;
}

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

void show(int arr[], int n)
{
	int i;
	for(i = 0; i < n; ++i)
	{
		printf("%7d", arr[i]);
		if(i % 8 == 7)
			printf("\n");
	}
	printf("\n");

}


void* thread_analyzer(void *_args)
{
	struct thread_args *args = (struct thread_args *) _args;
	char *s = args->s;
	int len = args->len;
	int pos = args->pos;
	int i;

	int state = 0;

	int *results = malloc((len / 3 + 5) * sizeof(int));

	int k = 1;
	for(i = 0; i < len; i++)
	{
		if(s[pos + i] == '\0')
			break;
		state = prelazi[state][ s[pos + i] ];
		
		if(state == 4)
		{
			/* pronasao */
			//printf("%d\n", pos + i - 2);
			results[k++] = pos + i - 2;
		}
	}
	results[0] = k - 1;
	pthread_exit((void*) results);

}

void analyze(char *s, int slen, int n)
{
	init_prelazi();
	int alen = slen / n, i, j, k;
	pthread_t threads[n];
	struct thread_args arguments[n];

	for(i = 0; i < n; i++)
	{
		arguments[i] = (struct thread_args) {s, i * alen, alen};
//		if(i == n - 1)
//			arguments[i].len += slen % n;
		pthread_create(&threads[i], NULL, thread_analyzer, &arguments[i]);
	}

	int *nizrez[n];
	int sum = 0;

	for(i = 0; i < n; i++)
	{
		pthread_join(threads[i],(void**)(&nizrez[i]));
		sum += nizrez[i][0];
	}

	int final_results[sum];
	k = 0;
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < nizrez[i][0]; j++)
		{
			final_results[k++] = nizrez[i][j + 1];
		}
		free(nizrez[i]);
	}

	bubble(final_results, sum, cmp_a);
	show(final_results, sum);

}

int main(int argc, char *argv[])
{
	int n;
	printf("Unesite broj thread-ova: ");
	scanf("%d", &n);
	
	analyze(test_codon, strlen(test_codon), n);

	exit(0);
}
