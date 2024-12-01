#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "prodcons.h"

void inizializza(MonitorPC * m) {

    printf("Inizializzazione monitor\n");

    /* TBD: Inizializzazione */

    m->num_liberi = DIM;
    m->num_occupati_tipo1 = 0;
    m->num_occupati_tipo2 = 0;
    
    for (int i = 0; i < DIM; i++)
    {
        m->vettore[i] = 0;
        m->stato[i] = LIBERO;
    }

    pthread_mutex_init(&(m->mutex),NULL);
    pthread_cond_init(&(m->cond_produttori),NULL);
    pthread_cond_init(&(m->cond_consumatori),NULL);
}


void rimuovi(MonitorPC * m) {

    printf("Rimozione monitor\n");

    /* TBD: Rimozione */

    pthread_mutex_destroy(&(m->mutex));
    pthread_cond_destroy(&(m->cond_consumatori));
    pthread_cond_destroy(&(m->cond_produttori));
}


void produci_tipo_1(MonitorPC * m, int valore) {

    int index = 0;


    /* TBD: Sincronizzazione, e selezione dell'indice su cui produrre */

    pthread_mutex_lock(&(m->mutex));

    while (m->num_liberi == 0) pthread_cond_wait(&(m->cond_produttori),&(m->mutex));

    while (index < DIM && m->stato[index] != LIBERO) index++;

    m->stato[index] = INUSO;
    m->num_liberi--;

    pthread_mutex_unlock(&(m->mutex));

    printf("Produzione di tipo 1 in corso (valore=%d, index=%d)\n", valore, index);

    sleep(1);
    m->vettore[index] = valore;

    pthread_mutex_lock(&(m->mutex));

    m->stato[index] = OCCUPATO1;
    m->num_occupati_tipo1++;

    pthread_cond_signal(&(m->cond_consumatori));

    pthread_mutex_unlock(&(m->mutex));
}


void produci_tipo_2(MonitorPC * m, int valore) {

    int index = 0;


    /* TBD: Sincronizzazione, e selezione dell'indice su cui produrre */

    pthread_mutex_lock(&(m->mutex));

    while (m->num_liberi == 0) pthread_cond_wait(&(m->cond_produttori),&(m->mutex));

    while (index < DIM && m->stato[index] != LIBERO) index++;

    m->stato[index] = INUSO;
    m->num_liberi--;

    pthread_mutex_unlock(&(m->mutex));


    printf("Produzione di tipo 2 in corso (valore=%d, index=%d)\n", valore, index);

    sleep(1);
    m->vettore[index] = valore;

    pthread_mutex_lock(&(m->mutex));

    m->stato[index] = OCCUPATO2;
    m->num_occupati_tipo2++;

    pthread_cond_signal(&(m->cond_consumatori));

    pthread_mutex_unlock(&(m->mutex));
}


void consuma_tipo_1(MonitorPC * m, int * valore) {

    int index = 0;


    /* TBD: Sincronizzazione, e selezione dell'indice su cui consumare */

    pthread_mutex_lock(&(m->mutex));

    while (m->num_occupati_tipo1 == 0) pthread_cond_wait(&(m->cond_consumatori),&(m->mutex));

    while (index < DIM && m->stato[index] != OCCUPATO1) index++;

    m->stato[index] = INUSO;
    m->num_occupati_tipo1--;

    pthread_mutex_unlock(&(m->mutex));

    sleep(1);
    *valore = m->vettore[index];

    printf("Consumazione di tipo 1 (valore=%d, index=%d)\n", *valore, index);

    pthread_mutex_lock(&(m->mutex));

    m->stato[index] = LIBERO;
    m->num_liberi++;

    pthread_cond_signal(&(m->cond_produttori));

    pthread_mutex_unlock(&(m->mutex));
}


void consuma_tipo_2(MonitorPC * m, int * valore) {

    int index = 0;


    /* TBD: Sincronizzazione, e selezione dell'indice su cui consumare */

    pthread_mutex_lock(&(m->mutex));

    while (m->num_occupati_tipo2 == 0) pthread_cond_wait(&(m->cond_consumatori),&(m->mutex));

    while (index < DIM && m->stato[index] != OCCUPATO1) index++;

    m->stato[index] = INUSO;
    m->num_occupati_tipo2--;

    pthread_mutex_unlock(&(m->mutex));


    sleep(1);
    *valore = m->vettore[index];

    printf("Consumazione di tipo 2 (valore=%d, index=%d)\n", *valore, index);

    pthread_mutex_lock(&(m->mutex));

    m->stato[index] = LIBERO;
    m->num_liberi++;

    pthread_cond_signal(&(m->cond_produttori));

    pthread_mutex_unlock(&(m->mutex));
}