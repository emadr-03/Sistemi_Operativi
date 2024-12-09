#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "header.h"

typedef struct {
    int buffer[MAX_VALUES];
    // TODO aggiungere le variabili per la gestione circolare
    // e per la sincronizzazione dei thread
    int testa;
    int coda;
    int numelementi;
    int somma;
    int numvalorisommati;

    pthread_mutex_t mutex;
    pthread_cond_t cond_produttori;
    pthread_cond_t cond_consumatori;
    pthread_cond_t cond_stampa;
} data_str;

// VARIABILI GLOBALI:
// la struttura
// le code
int queue_req;
int queue_res;

int consuma(data_str *buff) {
    int value;
    //TODO consumatore, con gestione coda circolare
    pthread_mutex_lock(&(buff->mutex));

    while (buff->numelementi == 0) pthread_cond_wait(&(buff->cond_consumatori),&(buff->mutex));

    value = buff->buffer[buff->coda];
    buff->coda = (buff->coda + 1) % MAX_VALUES;
    buff->numelementi--;

    pthread_cond_signal(&(buff->cond_produttori));

    pthread_mutex_unlock(&(buff->mutex));

    return value;
}

void produci(data_str *buff,int new_value) {
    //TODO produttore, con gestione coda circolare
    pthread_mutex_lock(&(buff->mutex));

    while (buff->numelementi == MAX_VALUES) pthread_cond_wait(&(buff->cond_produttori),&(buff->mutex));

    buff->buffer[buff->testa] = new_value;
    buff->testa = (buff->testa + 1) % MAX_VALUES;
    buff->numelementi++;

    pthread_cond_signal(&(buff->cond_consumatori));

    pthread_mutex_unlock(&(buff->mutex));
}


void * stampa_somma(void* par) {
    
    data_str *buff = (data_str *) par;
    
    //TODO attesa che siano stati sommati NUM_CONS valori utilizzando una condition
    // e stampa della somma una volta verificata la condizione
    pthread_mutex_lock(&(buff->mutex));

    while (buff->numvalorisommati < NUM_CONS) pthread_cond_wait(&(buff->cond_stampa),&(buff->mutex));

    printf("[Visualizzatore] Valore somma: %d\n",buff->somma);

    pthread_mutex_unlock(&(buff->mutex));

    pthread_exit(NULL);
}

void * produttore(void* par) {
    // par qui non serve, può essere ignorato
    data_str *buff = (data_str *) par;

    int new_value;
    int i;
    for (i = 0; i < NUM_UPDATES*MAX_VALUES; i++) {
        new_value = rand()%10+1;
        printf("PRODUTTORE: inserimento nuovo dato: %d\n",new_value);
        produci(par,new_value);
        sleep(rand()%3+1);
    }
    pthread_exit(NULL);
}


void * consumatore(void* par) {
    data_str *buff = (data_str *) par;
    int i;
    for (i = 0; i < NUM_CONS; i++) {
        req msg;
        //TODO ricezione messaggio di richiesta
        msgrcv(queue_req,(void *)&msg,sizeof(req)-sizeof(long),MSG_RICHIESTA,0);
        printf("CONSUMATORE_SERV: ricevuta richiesta di consumo\n");

        res risp;
        //TODO preparazione messaggio di risposta, usando funzione "consuma"
        risp.type = MSG_RISPOSTA;
        risp.value = consuma(buff);
        printf("CONSUMATORE_SERV: invio valore al consumatore client %d\n",risp.value);
        //TODO invio risposta
        msgsnd(queue_res,(void *)&risp,sizeof(res)-sizeof(long),0);
        
        //TODO aggiornamento della somma in 's' con il nuovo valore appena consumato
        //TODO e notifica della condizione quando sommati NUM_CONS elementi
        pthread_mutex_lock(&(buff->mutex));
        buff->somma += risp.value;
        buff->numvalorisommati++;
        if (buff->numvalorisommati == NUM_CONS) pthread_cond_signal(&(buff->cond_stampa));
        pthread_mutex_unlock(&(buff->mutex));
    }

    pthread_exit(NULL);
}

int main() {

    //TODO: completare l'inizializzazione code
    key_t msg_req_key = ftok(".",'a');
	key_t msg_res_key = ftok(".",'b');

        queue_req = msgget(msg_req_key,0);
	queue_res = msgget(msg_res_key,0);

    pthread_t prod, cons, sum;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    srand(time(NULL));

    data_str *buff = (data_str *) malloc(sizeof(data_str));
    // TODO e inizializzazione valori
    for (int i = 0; i < MAX_VALUES; i++) buff->buffer[i] = 0;
    buff->coda = 0;
    buff->testa = 0;
    buff->numelementi = 0;
    buff->somma = 0;
    buff->numvalorisommati = 0;
    pthread_mutex_init(&(buff->mutex),NULL);
    pthread_cond_init(&(buff->cond_consumatori),NULL);
    pthread_cond_init(&(buff->cond_produttori),NULL);
    pthread_cond_init(&(buff->cond_stampa),NULL);
    
    //TODO creazione dei tre thread prod cons e sum
    // nota: ai thread cons e sum bisogna passare la struttura s
    pthread_create(&cons,&attr,consumatore,(void *)buff);
    pthread_create(&sum,&attr,stampa_somma,(void *)buff);
    pthread_create(&prod,&attr,produttore,(void *)buff);

    //TODO join
    pthread_join(cons,NULL);
    pthread_join(sum,NULL);
    pthread_join(prod,NULL);

    //TODO rimozione code e struttura s
    pthread_mutex_destroy(&(buff->mutex));
    pthread_cond_destroy(&(buff->cond_consumatori));
    pthread_cond_destroy(&(buff->cond_produttori));
    pthread_cond_destroy(&(buff->cond_stampa));

    free(buff);

    return 0;
}
