#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#include <semaphore.h>

struct __job
{
	void * (*fun)(void *);
	void *arg;
};

typedef struct _tp
{
	int nthreads;
	pthread_t *threads;
	struct __job *job_queue;
	int qsize;
	sem_t full, empty, control_q, control_self;
	int running, paused, in, out;

} thread_pool_t;

void *worker_thread(void *_args)
{
	thread_pool_t *tp = (thread_pool_t *) _args;
	struct __job current_job;
	while(tp->running)
	{
		sem_wait(&(tp->full));
		sem_wait(&(tp->control_q));

		current_job = tp->job_queue[tp->out];
		tp->job_queue[tp->out] = (struct __job) {NULL, NULL};
		tp->out = (tp->out + 1) % tp->qsize;

		sem_post(&(tp->control_q));
		sem_post(&(tp->empty));

		if(current_job.fun !=NULL)
			current_job.fun(current_job.arg);
	}

}

int thread_pool_init(thread_pool_t *tp, int nthreads, int jobqsize)
{
	tp->nthreads = nthreads;
	tp->qsize = jobqsize;
	tp->threads = (pthread_t *) malloc(nthreads * sizeof(pthread_t));
	if(!tp->threads) return -1;
	tp->job_queue = malloc(jobqsize * sizeof(struct __job));
	if(!tp->job_queue)
	{
		free(tp->threads);
		return -1;
	}

	sem_init(&(tp->full), 0, 0);
	sem_init(&(tp->control_q), 0, 1);
	sem_init(&(tp->control_self), 0, 1);
	sem_init(&(tp->empty), 0, jobqsize);
	tp->running = 0;
	tp->paused = 0;
	tp->in = 0;
	tp->out = 0;

	return 0;
}

int thread_pool_run(thread_pool_t *tp)
{
	int i;
	sem_wait(&(tp->control_self));
	if(tp->running)
	{
		sem_post(&(tp->control_self));
		return -1;
	}
	tp->running = 1;
	for(i = 0; i < tp->nthreads; i++)
	{
		pthread_create(tp->threads + i, NULL, worker_thread, (void *) tp);
	}
	sem_post(&(tp->control_self));
	return 0;
}

int thread_pool_pause(thread_pool_t *tp)
{
	sem_wait(&(tp->control_self));
	if(!tp->running || tp->paused)
	{
		sem_post(&(tp->control_self));
		return -1;
	}
	tp->paused = 1;
	sem_wait(&(tp->control_q));
	sem_post(&(tp->control_self));
	return 0;
}

int thread_pool_continue(thread_pool_t *tp)
{
	sem_wait(&(tp->control_self));
	if(!tp->running || !tp->paused)
	{
		sem_post(&(tp->control_self));
		return -1;
	}
	tp->paused = 0;
	sem_post(&(tp->control_q));
	sem_post(&(tp->control_self));
	return 0;
}


int thread_pool_add_job(thread_pool_t *tp, void * (*fun)(void *), void *arg)
{
	sem_wait(&(tp->empty));
	sem_wait(&(tp->control_q));

	tp->job_queue[tp->in] = (struct __job) {fun, arg};
	tp->in = (tp->in + 1) % tp->qsize;

	sem_post(&(tp->control_q));
	sem_post(&(tp->full));
	return 0;
}


int thread_pool_destroy(thread_pool_t *tp)
{
	sem_wait(&(tp->control_self));
	tp->running = 0;
	int i;
	for(i = 0; i < tp->qsize; i++)
	{
		sem_post(&(tp->full));
		sem_post(&(tp->control_q));
	}

	for(i = 0; i < tp->nthreads; i++)
		pthread_join(tp->threads[i], NULL);
	free(tp->job_queue);
	free(tp->threads);
	sem_destroy(&(tp->full));
	sem_destroy(&(tp->empty));
	sem_destroy(&(tp->control_q));
	sem_post(&(tp->control_self));
	sem_destroy(&(tp->control_self));
	return 0;
}

int x = 0;
sem_t mt;

void *test_thread(void *_s)
{
	char *s = _s;
	int me;

	sem_wait(&mt);
	me = x;
	x++;
	sem_post(&mt);

	usleep(100000 + rand() % 2000000);
	printf("[%d]: %s\n", me, s);
}

char *sarr[] = {"Hi!", "Hello!", "How are you?", "What is going on?", "...", "Good! How about you?"};

int main(int argc, char *argv[])
{
	srand(time(NULL));
	int sarrlen = sizeof(sarr)/sizeof(sarr[0]);
	sem_init(&mt, 0, 1);
	thread_pool_t t;
	thread_pool_init(&t, 10, 50);

	printf("Adding jobs...\n");
	thread_pool_add_job(&t, test_thread, "Hello, World 1!");
	thread_pool_add_job(&t, test_thread, "Hello, World 2!");
	thread_pool_add_job(&t, test_thread, "Hello, World 3!");

	printf("Waitning 2 sec...\n");
	sleep(2);

	printf("Starting jobs...\n");
	thread_pool_run(&t);
	sleep(1);
	printf("Adding more jobs!\n");
	int i;
	for(i = 0; i < 30; i++)
	{
		thread_pool_add_job(&t, test_thread, sarr[rand()%sarrlen]);
	}

	sleep(6);
	thread_pool_destroy(&t);

	return 0;
}
