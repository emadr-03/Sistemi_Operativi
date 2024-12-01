#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "prodcons.h"


void * produttore_1(void * p)
{
	MonitorPC *m = (MonitorPC *) p;

	for(int i=0; i<4; i++) {

		int valore = rand() % 10;

		/* TBD: chiamare il metodo di produzione tipo 1 */
		produci_tipo_1(m,valore);		

		sleep(1);
	}

	pthread_exit(NULL);
}


void * produttore_2(void * p)
{
	MonitorPC *m = (MonitorPC *) p;
    
	for(int i=0; i<4; i++) {

		int valore = rand() % 10;

		/* TBD: chiamare il metodo di produzione tipo 2 */
		produci_tipo_2(m,valore);

		sleep(1);
	}

	pthread_exit(NULL);
}


void * consumatore_1(void * p)
{
	MonitorPC *m = (MonitorPC *) p;

	for(int i=0; i<8; i++) {

		int valore;

		/* TBD: chiamare il metodo di consumazione tipo 1 */
		consuma_tipo_1(m,&valore);

		sleep(1);
	}

	pthread_exit(NULL);
}


void * consumatore_2(void * p)
{
	MonitorPC *m = (MonitorPC *) p;

	for(int i=0; i<8; i++) {

		int valore;

		/* TBD: chiamare il metodo di consumazione tipo 1 */
		consuma_tipo_2(m,&valore);

		sleep(1);
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

	pthread_t thread_produttori_1[2];
	pthread_t thread_produttori_2[2];
	pthread_t thread_consumatore_1;
	pthread_t thread_consumatore_2;

	pthread_attr_t attr;
    
	int rc;
	int i;

	srand(time(NULL));

    
	/* TBD: creare e inizializzare l'oggetto monitor */
    
	MonitorPC *m = (MonitorPC *) malloc(sizeof(MonitorPC));

	inizializza(m);

	for(i=0; i<2; i++) {

		/* TBD: avviare 2 produttori di tipo 1 */
		pthread_create(&thread_produttori_1[i],NULL,produttore_1,(void *)m);
	}

	for(i=0; i<2; i++) {

		/* TBD: avviare 2 produttori di tipo 2 */
		pthread_create(&thread_produttori_2[i],NULL,produttore_2,(void *)m);
	}

	/* TBD: avviare consumatore di tipo 1 */
	pthread_create(&thread_consumatore_1,NULL,consumatore_1,(void *)m);

	/* TBD: avviare consumatore di tipo 2 */

	pthread_create(&thread_consumatore_2,NULL,consumatore_2,(void *)m);

	
	
	for(i=0; i<2; i++) {

		/* TBD: attendere terminazione produttori di tipo 1 */
		pthread_join(thread_produttori_1[i],NULL);
	}


	for(i=0; i<2; i++) {

		/* TBD: attendere terminazione produttori di tipo 2 */
		pthread_join(thread_produttori_2[i],NULL);
	}

	/* TBD: attendere terminazione consumatori di tipo 1 */
	pthread_join(thread_consumatore_1,NULL);

	/* TBD: attendere terminazione consumatori di tipo 2 */
	pthread_join(thread_consumatore_2,NULL);

	

	/* TBD: rimuovere l'oggetto monitor */

	rimuovi(m);
	free(m);
	pthread_exit(0);
}
