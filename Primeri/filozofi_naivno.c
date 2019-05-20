#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N (5)

/*
	Stapice cemo da ilustrujemo pomocu mutex-a. Tipicno bi to bio neki resurs koji je blokirajuc,
	npr. stampac ili skener ili slicno. Da se ne bismo majali sa pravim hardverom, reci cemo da je
	zakljucavanje mutex-a ekvivalentno uzimanju resursa, a otkljucavanje mutex-a ekvivalentno oslobadjanju.
*/
pthread_mutex_t stapici[N];
int neko_jede = 0;

/*
	Pomocna nit koja ce da nas izvuce iz deadlock situacije, ako se ona desi.
*/
void* checker_thread(void *_args)
{
	do
	{
		neko_jede = 0;
		sleep(2);
	}while(neko_jede);
	printf("Niko vise ne jede, posto niko ne moze da dobije drugi stapic!\n");
}

void* filozof_thread(void *_args)
{
	int id = *((int *) _args);
	int levi = id;
	int desni = (id + 1) % N;

	while(1)
	{

		pthread_mutex_lock(&stapici[levi]); /* razmislja dok je zauzet levi stapic */
		/*
			printf je relativno spora operacija, posto je sistemski poziv. Ovo ce sasvim lepo povecati verovatnocu
			da svih pet filozofa podigne levi stapic i bude skinuto sa izvrsavanja pre uzimanja desnog.
		*/
		printf("Filozof %d uzeo levi stapic!\n", id);
		pthread_mutex_lock(&stapici[desni]); /* razmislja dok je zauzet desni stapic */

		printf("Filozof %d jede!\n", id);
		neko_jede = 1;
		/*
			U praksi bi rad sa resursom trajao neko vreme, sto bismo mogli da simuliramo sa sleep.
			Nama to kvari primer posto bi nam taj sleep ozbiljno smanjio verovatnocu za deadlock.
			Morali bismo da pustamo aplikaciju da radi veoma veoma dugo pre nego se zablokira,
			ali deadlock bi i dalje bio tu kao realna mogucnost.
		*/
		//sleep(1);
		/* Jede */

		pthread_mutex_unlock(&stapici[levi]); /* Pusta levu viljusku */
		pthread_mutex_unlock(&stapici[desni]); /* Pusta desnu viljusku */

		/* Razmislja posle jela */
		//sleep(1);
	}
	

}

int main(int argc, char *argv[])
{
	int i;
	for(i = 0; i < N; i++)
	{
		pthread_mutex_init(&stapici[i], NULL);
	}
	
	pthread_t threads[N];
	pthread_t checker;
	int argumenti[N];	

	for(i = 0; i < N; i++)
	{
		argumenti[i] = i;
		pthread_create(&threads[i], NULL, filozof_thread, (void*)(&argumenti[i]));
	}

	pthread_create(&checker, NULL, checker_thread, NULL);

	pthread_join(checker, NULL);

	for(i=0; i < N; i++)
	{
		pthread_mutex_destroy(&stapici[i]);
	}

	return 0;
}
