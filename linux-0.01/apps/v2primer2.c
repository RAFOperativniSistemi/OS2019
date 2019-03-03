/* Ovaj primer demonstrira koriscenje pokazivaca na funckiju koji se koristi kao komparator pri algoritmu za sortiranje */
#include <string.h>
#include <unistd.h>

#define UTIL_IMPLEMENTATION
#include "utils.h"

int cmp_d(int x, int y)
{
	return x < y;
}


int cmp_a(int x, int y)
{
	return x > y;
}

/* 
	Pokazivac na funkciju moze biti parametar drugoj funkciji 
*/
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
	char buffer[128];
	int i, len;
	for(i = 0; i < n; ++i)
	{
		len = itoa(arr[i], buffer);
		buffer[len++] = ' ';
		write(1, buffer, len);
	}
	write(1, "\n", strlen("\n"));

}

int main(int argc, char *argv[])
{
	int arr[] = {3, 1, 5, 7, 11, 22, 4, 0};
	int n = sizeof(arr) / sizeof(arr[0]);
	
	bubble(arr, n, cmp_a);
	show(arr, n);

	bubble(arr, n, cmp_d);
	show(arr, n);
	
	_exit(0);	
}


