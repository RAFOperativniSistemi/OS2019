#include <stdio.h> 
#include <pthread.h> 
#include <stdlib.h>
#include <semaphore.h>

#define clear() printf("\033[H\033[J")

struct thread_args
{
	int id; //redni broj niti
	int start; //indeks pocetka sortiranja za ovu nit
	int *joblens; //za svaku nit, duzina njene sekcije
	int *arr; //kompletan niz
	pthread_barrier_t *barrier;
	int threads; //ukupan broj niti koje rade sortiranje
	int rings; //broj spajanja + 1
	sem_t *semaph; //zeleno zuto crveno
	
};



void merge(int arr[], int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 =  r - m;
	if(n2 == 0)
		return;


	/* create temp arrays */
	int L[n1], R[n2];

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1+ j];
  
	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray
	j = 0; // Initial index of second subarray
	k = l; // Initial index of merged subarray
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			arr[k] = L[i];
			i++;
		}
		else
		{
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1)
	{
		arr[k] = L[i];
		i++;
		k++;
	}

	while (j < n2)
	{
		arr[k] = R[j];
		j++;
		k++;
	}
}
  
void merge_sort(int arr[], int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
  
        merge_sort(arr, l, m);
        merge_sort(arr, m + 1, r);
  
        merge(arr, l, m, r);
    }
} 



int up_to_pow2(int x)
{
	x--;
	int i, ind = -1;
	for(i = 0; i < 8 * sizeof(x); i++)
		if(x & (1 << i))
			ind = i;
	return 1 << (ind + 1);


}

int log_base2(int x)
{
	int i;
	for(i = 0; i < 8 * sizeof(x); i++)
		if(x & (1 << i))
			return i;
	return -1;
}

void *sorter_thread(void *_args)
{
	struct thread_args *args = (struct thread_args *) _args;
	int id = args->id;
	int start = args->start;
	int *arr = args->arr;

	pthread_barrier_t *barrier = args->barrier;
	sem_t *semaph = args->semaph;

	int *joblens = args->joblens;
	int threads = args->threads;

	int rings = args->rings;

	int ring = 0;
	int eliminator, i;

	while(ring < rings)
	{
		eliminator = 1 << ring;

		if(id % eliminator != 0)
			pthread_exit(NULL);

		if(ring == 0) 	/* sorting ring */
			merge_sort(arr, start, start + joblens[id] - 1);
		else		/* merging rings */
		{
			if(id == 0) /* control thread, reset barrier */
			{
				pthread_barrier_destroy(barrier);
				int splits = 1 << ring;
				int survivors = threads / splits; /* n / 2^(ring + 1) and + 1 if not divisible */
				if(threads % splits) survivors++;
				pthread_barrier_init(barrier, NULL, survivors);
				for(i = 0; i < survivors - 1; i++)
					sem_post(semaph);
				
			}
			else
			{
				sem_wait(semaph);
			}
	
			int merge_left = start;
			int merge_middle = start;
			int blocksl = 0, blocksr = 0;

			int blocks_to_take = 1 << (ring - 1);

			for(i = id; i < id + blocks_to_take; i++)
			{
				if(i < threads)
				{
					merge_middle += joblens[i];
					blocksl++;
				}
			}
			merge_middle --;
			int merge_right = merge_middle;
			for(i = id + blocksl; i < id + blocksl + blocks_to_take; i++)
			{
				if(i < threads)
				{
					merge_right += joblens[i];
					blocksr++;
				}
			}

			merge(arr, merge_left, merge_middle, merge_right);


			
		}

		pthread_barrier_wait(barrier);
		ring++;
		
	}

}

int multithreaded_sort(int arr[], int n, int nthreads)
{
	if(nthreads < 1)
		nthreads = 1;

	int i;
	
	while(n / nthreads < 3)
		nthreads--;	/* this only works if the minimal load per thread is 3 */

	pthread_barrier_t barrier;
	pthread_barrier_init(&barrier, NULL, nthreads);

	sem_t semaph;
	sem_init(&semaph, 0, 0);

	int rings = log_base2(up_to_pow2(nthreads)) + 1;

	pthread_t threads[nthreads];
	struct thread_args args[nthreads];
	int joblens[nthreads];
	int joblen = n / nthreads;


	for(i = 0; i < nthreads; i++)
	{
		joblens[i] = joblen;
	}
	joblens[nthreads - 1] += n % nthreads;

	for(i = 0; i < nthreads; i++)
	{
		args[i] = (struct thread_args) {i, joblen * i, joblens, arr, &barrier, nthreads, rings, &semaph};
		pthread_create(&threads[i], NULL, sorter_thread, (void*)(&args[i]));
	}


	for(i = 0; i < nthreads; i++)
	{
		pthread_join(threads[i], NULL);
	}

	pthread_barrier_destroy(&barrier);

}

int show(int arr[], int n)
{
	int i;
	for(i = 0; i < n; i++)
	{
		printf("%3d ", arr[i]);
	}
	printf("\n");

}

int main(int argc, char *argv[])
{
	int nthreads = 0;
	clear();
	if(argc > 1)
		nthreads = atoi(argv[1]);

	printf("nthreads: %d\n\n", nthreads);

	int arr[] = {3, 7, 12, 1, 17, -3, 29, -32, 44, 11, 28, 352, 400, 217, 65, 72, 44, 13, 2};
	int n = sizeof(arr) / sizeof(arr[0]);
	int arr2[] = {3, 7, 12, 1, 17, -3, 29, -32, 44, 11, 28, 352, 400, 217, 65, 72, 44, 13, 2};
	int n2 = sizeof(arr) / sizeof(arr[0]);

	printf("Unsorted: ");
	show(arr, n);

	printf("Control:  ");
	merge_sort(arr2, 0, n2 - 1);
	show(arr2, n2);

	printf("Sorted:   ");
	multithreaded_sort(arr, n, nthreads);
	show(arr, n);

	int i, good = 1;
	for(i = 0; i < n; i++)
		if(arr[i] != arr2[i])
			good = 0;
	if(good)
		printf("GOOD!\n");
	else
		printf("BAD!\n");
	
}
