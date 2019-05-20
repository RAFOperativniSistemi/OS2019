#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include <semaphore.h>

/* Broj saltera */
#define N (5)

/* Broj ljudi koji cekaju */
#define M (100)

#define clear() printf("\033[H\033[J")

/* Pomocni semafor koji cemo koristiti da bebedno dovedemo klijenta na salter */
sem_t mutx;

sem_t salteri;
int salteri_ljudi[N];

int cekaju[M];
int zavrsili[M];

int zauzmi_salter(int id)
{
	int i;
	/*
		Ovde koristimo pomocni semafor jer bi bilo prilicno lose
		ako bi ova nit bila prekinuta u sred ove for petlje.
	*/
	sem_wait(&mutx);
	for(i = 0; i < N; i++)
	{
		if(salteri_ljudi[i] == -1)
		{
			salteri_ljudi[i] = id;
			break;
		}
	}
	sem_post(&mutx);
	return i;
}

void oslobodi_salter(int sid)
{
	sem_wait(&mutx); //verovatno mozemo i bez ovoga, ali nece da se baci
	salteri_ljudi[sid] = -1;
	sem_post(&mutx);
}

/*
	Funkcija niti, tj. klijenta u posti. Radi sledece:
	-Dolazi na pocetak reda u nasumicnom trenutku.
	-Ceka da se neki salter oslobodi, i zauzme ga.
	-Radi na salteru nasumicnu kolicinu vremena.
	-Oslobodi salter.
*/
void* cekaj(void *_args)
{
	int id = *((int *) _args);

	sleep(id + rand() % 3); /* simuliraj razlicita vremena dolaska */

	cekaju[id] = 1;

	sem_wait(&salteri);
	/* znamo da sada ima neki slobodan salter - zauzmemo ga */
	int x = zauzmi_salter(id); 

	cekaju[id] = 0;
	sleep(5 + rand() % 5);
	zavrsili[id] = 1;

	oslobodi_salter(x);
	sem_post(&salteri);
}

void* prikaz_thread(void *_args)
{
	int i, id;
	
	while(1)
	{
		usleep(10000);
		clear();
		printf("Salteri:\n");
		for(i = 0; i < N; i++)
		{
			id = salteri_ljudi[i];
			if(id == -1)
				printf("\tSalter [%d]: Prazan\n", i);
			else
				printf("\tSalter [%d]: %d\n", i, id);

		}
		printf("\n");
		printf("Cekaju: ");
		for(i = 0; i < M; i++)
		{
			if(cekaju[i])
				printf("%d ", i);
		}

		printf("\n\nZavrsili: ");
		for(i = 0; i < M; i++)
		{
			if(zavrsili[i])
				printf("%d ", i);
		}
		printf("\n");
	}
}


int main(int argc, char *argv[])
{
	srand(time(NULL));
	sem_init(&salteri, 0, N);
	sem_init(&mutx, 0, 1);
	int i;
	for(i = 0; i < N; i++)
	{
		salteri_ljudi[i] = -1;
	}

	pthread_t ljudi[M];
	int ids[M];

	for(i = 0; i < M; i++)
	{
		ids[i] = i;
		pthread_create(&ljudi[i], NULL, cekaj, (void *)(&ids[i]));
	}

	pthread_t prikazt;
	pthread_create(&prikazt, NULL, prikaz_thread, NULL);

	for(i = 0; i < M; i++)
	{
		pthread_join(ljudi[i], NULL);
	}

	sleep(1);

	sem_destroy(&mutx);
	sem_destroy(&salteri);

	return 0;
}
