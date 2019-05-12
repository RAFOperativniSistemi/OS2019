#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* print_message_function(void *ptr);

int main(int argc, char *argv[])
{
    pthread_t thread1, thread2;
    char *message1 = "Thread 1";
    char *message2 = "Thread 2";


    pthread_create(&thread1, NULL, print_message_function, (void*) message1);
    pthread_create(&thread2, NULL, print_message_function, (void*) message2);

    printf("Waiting for threads to finish!");
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("All threads finished!");
    exit(0);
}

void* print_message_function(void *ptr)
{
    char *message;
    message = (char *) ptr;
    printf("%s \n", message);
}