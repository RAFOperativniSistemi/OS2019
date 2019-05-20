#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include <semaphore.h>

#define BUFFER_SIZE (15)

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

#define PRODUCES 100

#define PRODUCERS 5
#define CONSUMERS 1


int productions = PRODUCES;

int consumes = PRODUCES;

sem_t mutx; /* mutex */
sem_t full; /* brojac za popunjena mesta u deljenom redu - inicijalizuje se na 0 */
sem_t empty; /* brojac za slobodna mesta u deljenom redu - inicijalizuje se na BUFFER_SIZE */

int consumed[PRODUCES];
int consumed_index = 0;

sem_t mutx_consume;

/*
	Operacija koju consumer niti izvrsavaju kada uzmu podatak iz deljenog reda.
	Upisuje podatak u "consumed" niz na bezbedan nacin.
*/
void consume(int product)
{
	sem_wait(&mutx_consume);

	consumed[consumed_index] = product;
	consumed_index ++;

	sem_post(&mutx_consume);
}

/*
	Funkcija za producer niti. Radi sledece u petlji:
	-Prestaje sa radom ako je globalni brojac produkcija pao na 0.
	-Spava 1-6 sekundi.
	-Generise nasumicni podatak.
	-Blokira se ako je blokirajuci red pun.
	-Pod mutex-om upisuje u deljeni red.
	-Javlja da je nesto upisano u red.
*/
void* producer(void *_args)
{
	int product;
	while(1)
	{
		if(productions <= 0)
			break;

		/* Generisi podatke */
		usleep(1000000 + rand() % 5000000);
		product = rand() % 100;

		/*
			"empty" je semafor koji je brojac praznih mesta u redu.
			Ovde njega dekrementiramo, i ako je on pao na 0, znaci da je red pun i mi se blokiramo.
			Ako smo se ovde blokirali, bicemo probudjeni od strane consumer
			niti koja ce ovaj brojac da inkrementira nakon sto uzme nesto iz reda.
		*/
		sem_wait(&empty); 
		sem_wait(&mutx);

		/* Upisi u buffer */
		productions --;

		buffer[in] = product;
		in = (in + 1) % BUFFER_SIZE;

		sem_post(&mutx);

		/*
			Inkrementiramo "full" - brojac popunjenih mesta u deljenom redu.
			Ako je blokirajuci red pre ovoga bio prazan, ovo ce eventualno da
			oslobodi neku consumer nit koja je bila blokirana nad tim uslovom.
		*/
		sem_post(&full);

	}

	pthread_exit(NULL);
}

/*
	Funkcija za consumer niti. Radi sledece u petlji:
	-Prestaje sa radom ako je globalni brojac konzumiranja pao na 0.
	-Blokira se ako je blokirajuci red prazan.
	-Pod mutex-om cita iz deljenog reda.
	-Javlja da je nesto uzeto iz reda.
	-Obradjuje podatak (operacija consume() )
	-Spava 1-6 sekundi.
*/
void* consumer(void *_args)
{
	int product;
	while(1)
	{
		if(consumes <= 0)
			break;

		/*
			Posto je "full" brojac popunjenih mesta u deljenom redu, ovde ga dekrementiramo,
			ako je on na 0, znaci da ne mozemo da citamo, i blokiramo se.
			Ako smo se blokirali ovde, bicemo probudjeni od strane producer niti,
			koja ce povecati ovaj brojac nakon sto upise nesto u red.
		*/
		sem_wait(&full);
		sem_wait(&mutx);

		consumes --;

		product = buffer[out];

		buffer[out] = 0;
		out = (out + 1) % BUFFER_SIZE;

		sem_post(&mutx);

		/*
			Inkrementiramo "empty" brojac, koji broji koliko ima praznih mesta u blokirajucem redu.
			Ovime cemo da potencijalno probudimo nekog producera koji se blokirao jer je red bio pun.
		*/
		sem_post(&empty);

		consume(product);
		usleep(1000000 + rand() % 5000000);
		
	}

	pthread_exit(NULL);

}

#define clear() printf("\033[H\033[J")
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"

#define RESET "\x1B[0m"

#define KYELB "\x1B[43m"

#define CONSUMES_PER_ROW (8)
#define NEEDS_NEW_ROW(x) ((x) % CONSUMES_PER_ROW == CONSUMES_PER_ROW - 1)


void* printer_thread(void *_args)
{
	int i, value, ci, fl, em;
	printf(RESET);
	while(1)
	{
		usleep(1000);
		clear();
		printf(RESET);
		sem_getvalue(&full, &fl);
		sem_getvalue(&empty, &em);


		printf("\n\nProduces left: %d\n\n", productions);

		printf("Buffer size: %d\n", BUFFER_SIZE);
		printf("%d full slots!\n", fl);
		printf("%d empty slots!\n", em);


		printf("\n");
		int tmp_out;
		printf("Buffer:\n[ ");
		for(i = 0; i < BUFFER_SIZE; i++)
		{
			
			if(i == out)
				printf(KBLU);
			if(i == in)
				printf(KYELB);
			
			value = buffer[i];
			if(value)
				printf("%2d", value);
			else
				printf("  ");

			printf(RESET);
			printf(" ");
		}
		printf("]\n\nConsumed:\n");
		ci = consumed_index;
		for(i = 0; i < ci; i++)
		{
			printf("%2d ", consumed[i]);
			if(NEEDS_NEW_ROW(i))
				printf("\n");
		}

		printf("\n");

	}
	
}

int main(int argc, char *argv[])
{
	srand(time(NULL));

	sem_init(&mutx, 0, 1);
	sem_init(&mutx_consume, 0, 1);

	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);

	int i;

	pthread_t producers[PRODUCERS];
	pthread_t consumers[CONSUMERS];

	for(i = 0; i < PRODUCERS; i++)
	{
		pthread_create(&producers[i], NULL, producer, NULL);
	}

	for(i = 0; i < CONSUMERS; i++)
	{
		pthread_create(&consumers[i], NULL, consumer, NULL);
	}

	pthread_t prikazt;
	pthread_create(&prikazt, NULL, printer_thread, NULL);

	for(i = 0; i < CONSUMERS; i++)
	{
		pthread_join(consumers[i], NULL);
	}

	sleep(1);

	sem_destroy(&mutx);
	sem_destroy(&mutx_consume);

	sem_destroy(&full);
	sem_destroy(&empty);

	return 0;
}
