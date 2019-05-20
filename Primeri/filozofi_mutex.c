#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N (5)

pthread_mutex_t stapici[N];
pthread_mutex_t konobar;
int neko_jede = 0;

/*
	Ova aplikacija ce da radi u beskonacnoj petlji.
	Za prekidanje njenog rada, mozemo da uradimo CTRL+C u konzoli.
*/

void* checker_thread(void *_args)
{
	do
	{
		neko_jede = 0;
		sleep(5);
	}while(neko_jede);
	printf("Niko vise ne jede, posto niko ne moze da dobije drugu viljusku!\n");
}

void* filozof_thread(void *_args)
{
	int id = *((int *) _args);
	int levi = id;
	int desni = (id + 1) % N;

	while(1)
	{
		/*
			Tek kada dobijemo dozvolu od konobara (uzmemo mutex) mozemo da pristupimo uzimanju stapica.
			Ovo znaci da kada filozof krene sa uzimanjem levog stapica on ce uzeti i desni, tj. nece biti prekinut
			tokom ovog procesa od strane drugog filozofa koji bi takodje zeleo da uzme neki stapic.
		*/
		pthread_mutex_lock(&konobar);
		pthread_mutex_lock(&stapici[levi]); /* razmislja dok je zauzet levi stapic */

		pthread_mutex_lock(&stapici[desni]); /* razmislja dok je zauzet desni stapic */
		pthread_mutex_unlock(&konobar);

		printf("Filozof %d jede!\n", id);
		neko_jede = 1;
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
	pthread_mutex_init(&konobar, NULL);
	
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
	pthread_mutex_destroy(&konobar);

	return 0;
}
