all: barrier_simple.c
	gcc barrier_simple.c -o barrier_simple -pthread
	gcc multithreaded_sort.c -o multithreaded_sort -pthread
	gcc multithreaded_merge.c -o multithreaded_merge -pthread
	gcc thread_pool.c -o thread_pool -pthread
	
clean:
	rm barrier_simple
	rm multithreaded_sort
	rm multithreaded_merge
	rm thread_pool
