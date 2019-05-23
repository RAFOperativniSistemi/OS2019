#include <stdio.h> 
#include <pthread.h> 
#include <stdlib.h>

int t1,t2,t3,t4;
pthread_barrier_t our_barrier;

void* thread1(void *_args) {
		sleep(2);
		t1 = 10;
		
		pthread_barrier_wait(&our_barrier);
		
		printf("\nvalues entered by the threads are  %d %d %d %d \n",t1,t2,t3,t4);
}


void* thread2(void *_args) {
		sleep(4);
		t2 = 20;
		
		pthread_barrier_wait(&our_barrier);

		printf("\nvalues entered by the threads are  %d %d %d %d \n",t1,t2,t3,t4);
}

void* thread3(void *_args) {

		sleep(6);

		t3 = 30;
		pthread_barrier_wait(&our_barrier);

		printf("\nvalues entered by the threads are  %d %d %d %d \n",t1,t2,t3,t4);
}

void *thread4(void *_args) {

		sleep(8);

		t4 = 40;
		pthread_barrier_wait(&our_barrier);

		printf("\nvalues entered by the threads are  %d %d %d %d \n",t1,t2,t3,t4);
}


int main(int argc, char *argv[]) {

		pthread_t thread_id_1,thread_id_2,thread_id_3,thread_id_4;
		pthread_attr_t attr;

		pthread_barrier_init(&our_barrier,NULL,4);

		pthread_create(&thread_id_1,NULL,&thread1,NULL);
		pthread_create(&thread_id_2,NULL,&thread2,NULL);
		pthread_create(&thread_id_3,NULL,&thread3,NULL);
		pthread_create(&thread_id_4,NULL,&thread4,NULL);

		printf("\n Created threads \n");

		pthread_join(thread_id_1,NULL);
		pthread_join(thread_id_2,NULL);
		pthread_join(thread_id_3,NULL);
		pthread_join(thread_id_4,NULL);

		pthread_barrier_destroy(&our_barrier);
}
