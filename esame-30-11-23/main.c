#include <stdio.h>
#include <stdlib.h>

#include "prodcons.h"

#define NUM_THREAD_PROD 3
#define NUM_THREAD_CONS 3
#define NUM_PRODUZIONI 10
#define NUM_CONSUMAZIONI 10
#define CONTEGGIO 5

void * thread_produttore(void *);
void * thread_consumatore(void *);
void * thread_conteggio(void *);


int main() {

    /* TBD: Creare un oggetto monitor di tipo "MonitorProdCons" */

    pthread_t produttori[NUM_THREAD_PROD];
    pthread_t consumatori[NUM_THREAD_CONS];
    pthread_t threadconteggio;

    MonitorProdCons *buff = (MonitorProdCons *) malloc(sizeof(MonitorProdCons));

    init_monitor(buff);

    for(int i=0; i<NUM_THREAD_PROD; i++) {

        /* TBD: Creare un thread produttore, fargli eseguire la funzione "thread_produttore" */

        pthread_create(&produttori[i],NULL,thread_produttore,(void *) buff);
    }

    for(int i=0; i<NUM_THREAD_CONS; i++) {

        /* TBD: Creare un thread consumatore, fargli eseguire la funzione "thread_consumatore" */

        pthread_create(&consumatori[i],NULL,thread_consumatore,(void *) buff);
    }


    /* TBD: Creare un thread di "conteggio", fargli eseguire la funzione "thread_conteggio" */
    pthread_create(&threadconteggio,NULL,thread_conteggio,(void *) buff);


    /* TBD: Attendere la terminazione dei thread */
    for (int i = 0; i < NUM_THREAD_CONS; i++) pthread_join(consumatori[i],NULL);
    for (int i = 0; i < NUM_THREAD_PROD; i++) pthread_join(produttori[i],NULL);
    pthread_join(threadconteggio,NULL);

    remove_monitor(buff);

    /* TBD: Deallocare l'oggetto monitor */
    free(buff);
}

void * thread_produttore(void * x) {

    /* TBD: Completare il passaggio dell'oggetto "MonitorProdCons" */
    MonitorProdCons *buff = (MonitorProdCons *) x;

    for(int i=0; i<NUM_PRODUZIONI; i++) {

        /* TBD: Completare la chiamata a produzione() */

        int val = rand() % 10;
        produzione(buff,val);

        //printf("[PRODUTTORE] Ho prodotto: %d\n", val);

    }

    return NULL;
}

void * thread_consumatore(void * x) {

    /* TBD: Completare il passaggio dell'oggetto "MonitorProdCons" */
    MonitorProdCons *buff = (MonitorProdCons *) x;

    for(int i=0; i<NUM_CONSUMAZIONI; i++) {

        /* TBD: Completare la chiamata a consumazione() */

        int val = consumazione(buff);

        //printf("[CONSUMATORE] Ho consumato: %d\n", val);
    }

    return NULL;
}


void * thread_conteggio(void * x) {

    /* TBD: Completare il passaggio dell'oggetto "MonitorProdCons" */
    MonitorProdCons *buff = (MonitorProdCons *) x;

    int consumazioni_effettuate = 0;

    int consumazioni_da_effettuare = NUM_CONSUMAZIONI * NUM_THREAD_CONS;

    while(consumazioni_da_effettuare > 0) {

        int conteggio;

        if( consumazioni_da_effettuare >= CONTEGGIO ) {

            // Vi sono 5 o più consumazioni ancora da effettuare,
            // si pone a 5 il conteggio da attendere
            conteggio = CONTEGGIO;
        }
        else {

            // Vi sono meno di 5 consumazioni ancora da effettuare,
            // si pone il conteggio da attendere a "consumazioni_da_effettuare"
            conteggio = consumazioni_da_effettuare;
        }

        /* TBD: Completare la chiamata ad "attendi_consumazioni()" */

        int n = attendi_consumazioni(buff,conteggio);

        consumazioni_effettuate += n;

        consumazioni_da_effettuare -= n;

        printf("[CONTEGGIO] Sono stati consumati in totale %d elementi\n", consumazioni_effettuate);
    }

    return NULL;
}
