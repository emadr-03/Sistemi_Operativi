#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "header.h"

typedef struct {
    int value;
    //TODO: aggiunta delle variabili necessarie alla sincronizzazione
    // con lettori-scrittori con staravtion di entrambi
    int numlettori;
    int numscrittori;
    int num_scrittori_wait;

    pthread_mutex_t mutex;
    pthread_cond_t cond_lettori;
    pthread_cond_t cond_scrittori;
} data_str;

typedef struct {
    int queue_res;
    int sender;
    data_str* data;
} reader_param;

int read_data(data_str* data) {
    int value;
    
    //TODO: lettura valore da "data" attraverso
    // lettori-scrittori con starvation di entrambi
    pthread_mutex_lock(&(data->mutex));

    while (data->numscrittori > 0) pthread_cond_wait(&(data->cond_lettori),&(data->mutex));

    data->numlettori++;

    pthread_mutex_unlock(&(data->mutex));

    value = data->value;

    pthread_mutex_lock(&(data->mutex));

    data->numlettori--;

    if (data->numlettori == 0) pthread_cond_signal(&(data->cond_scrittori));

    pthread_mutex_unlock(&(data->mutex));

    return value;
}

void write_data(data_str* data, int new_value) {
    //TODO: scrittura "new_value" in "data" attraverso
    // lettori-scrittori con starvation di entrambi
    pthread_mutex_lock(&(data->mutex));

    while (data->numlettori > 0 || data->numscrittori > 0)
    {
        data->num_scrittori_wait++;
        pthread_cond_wait(&(data->cond_scrittori),&(data->mutex));
        data->num_scrittori_wait--;
    }

    data->numscrittori++;

    pthread_mutex_unlock(&(data->mutex));

    data->value = new_value;
    
    pthread_mutex_lock(&(data->mutex));

    data->numscrittori--;

    if (data->num_scrittori_wait > 0) pthread_cond_signal(&(data->cond_scrittori));
    else pthread_cond_broadcast(&(data->cond_lettori));

    pthread_mutex_unlock(&(data->mutex));
}

void * updater(void* par) {
    data_str* data = (data_str*) par;
    int new_value;
    int i;
    for (i = 0; i < NUM_UPDATES; i++) {
        new_value = rand()%10+1;
        printf("UPDATER: aggiornamento dato al valore: %d\n",new_value);
        write_data(data,new_value);
        sleep(rand()%3+1);
    }
    pthread_exit(NULL);
}

void * reader(void* par) {
    reader_param* p = (reader_param *) par;

    res risp;
    //TODO: Costruzione risposta con prelievo valore dalla funzione read_data
    risp.type = MSG_RISPOSTA;
    risp.sender = p->sender;
    risp.value = read_data(p->data);
    printf("READER_SERV: invio valore al client %d\n",risp.value);
    //TODO: Invio risposta al reader client
    msgsnd(p->queue_res,(void *)&risp,sizeof(res)-sizeof(long),0);

    pthread_exit(NULL);
}

int main() {

    int pid;
    key_t msg_req_key = ftok(".",'a');
	key_t msg_res_key = ftok(".",'b');

    //TODO: INIZIALIZZAZIONE CODE

    int queue_req = msgget(msg_req_key,0);
    int queue_res = msgget(msg_res_key,0);

    data_str* d = (data_str *) malloc(sizeof(data_str));

    //TODO: inizializzazione mutex e condition variables
    d->value = 0;
    d->num_scrittori_wait = 0;
    d->numscrittori = 0;
    d->numlettori = 0;
    pthread_mutex_init(&(d->mutex),NULL);
    pthread_cond_init(&(d->cond_lettori),NULL);
    pthread_cond_init(&(d->cond_scrittori),NULL);

    pthread_t upd, rd[NUM_READS*NUM_READERS];

    reader_param p[NUM_READS*NUM_READERS];

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    srand(time(NULL));
    
    //TODO: creazione thread updater
    pthread_create(&upd,&attr,updater,(void *)d);

    req msg;
    int i;
    for (i = 0; i < NUM_READS*NUM_READERS; i++) {
        //TODO: ricezione messaggio di richiesta
        msgrcv(queue_req,(void *)&msg,sizeof(req)-sizeof(long),MSG_RICHIESTA,0);
        printf("READER:SERV: ricevuta richiesta di lettura da: %d\n",msg.pid);
        //TODO: preparazione parametro p[i]
        p[i].queue_res = queue_res;
        p[i].sender = msg.pid;
        p[i].data = d;
        //TODO: creazione thread reader
        pthread_create(&rd[i],&attr,reader,(void *)&p[i]);
    }
    //TODO: join dei thread
    pthread_join(upd,NULL);
    for (int i = 0; i < NUM_READS*NUM_READERS; i++) pthread_join(rd[i],NULL);

    //TODO: deallocazione memoria e rimozione coda

    pthread_mutex_destroy(&(d->mutex));
    pthread_cond_destroy(&(d->cond_lettori));
    pthread_cond_destroy(&(d->cond_scrittori));

    free(d);

    return 0;
}
