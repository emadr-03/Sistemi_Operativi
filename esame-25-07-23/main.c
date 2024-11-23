#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "procedure.h"

#define QUANTITA_INIZIALE 500
#define VALORE_INIZIALE 100

void * aggiornatore(void * p);
void * azionista(void * p);

int main() {

    pthread_t threadaggiornatore;
    pthread_t threadazionisti[3];
    MonitorPortafoglio * m[3];

    for(int i = 0; i<3; i++) {

        /* TBD: Creare ed inizializzare gli oggetti monitor */
        m[i] = (MonitorPortafoglio *) malloc(sizeof(MonitorPortafoglio));

        inizializza(m[i],QUANTITA_INIZIALE,VALORE_INIZIALE);
    }


    /* TBD: Creare il thread aggiornatore, passargli il vettore di oggetti monitor */
    pthread_create(&threadaggiornatore,NULL,aggiornatore,(void *) m);


    for(int i = 0; i<3; i++) {
            
        /* TBD: Creare i thread azionisti, passando a ognuno uno degli oggetti monitor */
        pthread_create(&threadazionisti[i],NULL,azionista,(void *) m[i]);
    }


    /* TBD: Attendere la terminazione del thread aggiornatore */
    pthread_join(threadaggiornatore,NULL);

    for(int i = 0; i<3; i++) {

        /* TBD: Attendere la terminazione dei thread azionisti, deallocare gli oggetti */
        pthread_join(threadazionisti[i],NULL);

        termina(m[i]);

        free(m[i]);
    }

    return 0;
}



void * aggiornatore(void * p) {

    /* TBD: Gestire il passaggio dei parametri

        Suggerimento: Dichiarare l'array di oggetti con la sintassi "MonitorPortafoglio ** m = ..." 
    */
    MonitorPortafoglio ** m = p;


    for(int i=0; i<10; i++) {

        for(int j=0; j<3; j++) {

            int valore = 100 + ((rand()%21) - 10);

            printf("[AGGIORNATORE] Aggiornamento titolo %d, valore %d\n", j, valore);

            
            /* TBD: Invocare il metodo "aggiorna" sull'oggetto monitor */
            aggiorna(m[j],valore);
        }

        sleep(1);
    }

    printf("[AGGIORNATORE] Terminazione\n");

    for(int j=0; j<3; j++) {

        /* TBD: Invocare il metodo "aggiorna" sull'oggetto monitor, con valore 200 */

        aggiorna(m[j],200);

        sleep(1);

        /* TBD: Invocare il metodo "aggiorna" sull'oggetto monitor, con valore 0 */

        aggiorna(m[j],0);
    }

    return NULL;
}


void * azionista(void * p) {

    /* TBD: Gestire il passaggio dei parametri */

    MonitorPortafoglio * m = p;

    printf("[AZIONISTA] Richiesta di vendita azioni, prezzo offerto 105\n");

    /* TBD: Invocare il metodo "vendita" sull'oggetto monitor, con valore 105 e quantità 50 */
    vendita(m,50,105);


    int valore = leggi(m);

    printf("[AZIONISTA] Vendita effettuata, valore attuale %d\n", valore);


    printf("[AZIONISTA] Richiesta di acquisto azioni, prezzo offerto 95\n");

    /* TBD: Invocare il metodo "acquisto" sull'oggetto monitor, con valore 95 e quantità 50 */
    acquisto(m,50,95);


    valore = leggi(m);

    printf("[AZIONISTA] Acquisto effettuato, valore attuale %d\n", valore);


    printf("[AZIONISTA] Terminazione\n");

    return NULL;
}