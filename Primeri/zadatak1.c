#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "codon.h"

struct thread_args
{
	char *s;
	int start_pos, len;
};

void *analyzer_thread(void *_args);
void analyze(char *s, int slen, int n);
void table_init(void);

int main(int argc, char *argv[])
{

	int n;
	printf("Unesite broj threadova: ");
	scanf("%d", &n);
	analyze(test_codon, strlen(test_codon), n);


	exit(0);
}

int tabela[5][128];

void table_init(void)
{
	static int inited = 0;

	if(inited)
		return;
	inited = 1;

	tabela[0]['t'] = 1;

	tabela[1]['a'] = 2;
	tabela[1]['g'] = 3;
	tabela[1]['t'] = 1;

	tabela[2]['a'] = 4;
	tabela[2]['g'] = 4;
	tabela[2]['t'] = 1;

	tabela[3]['a'] = 4;
	tabela[3]['t'] = 1;

	tabela[4]['t'] = 1;

}


void *analyzer_thread(void *_args)
{
	struct thread_args *args = (struct thread_args *) _args;
	char *s = args->s;
	int pos = args->start_pos;
	int len = args->len;
	int i;

	int state = 0, k = 0;

	int *result = malloc((len / 3 + 5) * sizeof(int));

	
	for(i = 0; i < len + 2; i++)
	{
		if(s[pos + i] == '\0')
			break;
		state = tabela[state][s[pos + i]];

		if(state == 4)
		{
			/* Pronasli */
			//printf("%d\n", pos + i);
			
			result[k++] = pos + i - 2;
			
		}
		
	}
	result[k] = -1;
	pthread_exit(result);

}

int nizlen(int niz[])
{
	int i = 0;
	while(niz[i] != -1)
		i++;
	return i;
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


int cmp_a(int x, int y)
{
	return x > y;
}

void show(int arr[], int n)
{
	int i, len;
	for(i = 0; i < n; ++i)
	{
		printf("%5d ", arr[i]);
		if(i % 8 == 7)
			printf("\n");
	}
	printf("\n");

}



void analyze(char *s, int slen, int n)
{

	table_init();
	
	pthread_t threads[n];
	struct thread_args arguments[n];
	int i, j, alen = slen / n;

	for(i = 0; i < n; i++)
	{
		
		arguments[i] = (struct thread_args) {s, i * alen, alen};
		if(i == n - 1)
			arguments[i].len += slen % n;
		pthread_create(&threads[i], NULL, analyzer_thread,(void*) (arguments + i));
	}

	int *results[n];
	int lens[n];
	int sum = 0;

	for(i = 0; i < n; i++)
	{
		pthread_join(threads[i], (void **)(results + i));
		lens[i] = nizlen(results[i]);
		sum += lens[i];
	}

	int k = 0;

	int rezultati[sum];
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < lens[i]; j++)
		{
			rezultati[k++] = results[i][j];
		}
		free(results[i]);
	}

	bubble(rezultati, sum, cmp_a);
	show(rezultati, sum);

	printf("sum: %d\n", sum);	

}

