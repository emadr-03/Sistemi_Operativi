#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>

#include "store.h"

extern int ds_queue_request, ds_queue_response;

void Wait_Sem(int ds_sem, int id_sem) {
    struct sembuf buf;
    buf.sem_flg = 0;
    buf.sem_num = id_sem;
    buf.sem_op = -1;
    semop(ds_sem,&buf,1);
}

void Signal_Sem(int ds_sem, int id_sem) {
    struct sembuf buf;
    buf.sem_flg = 0;
    buf.sem_num = id_sem;
    buf.sem_op = 1;
    semop(ds_sem,&buf,1);
}

void init_magazzino(Magazzino *magazzino){
    // TODO inizializzazione dei semafori , delle variabili necessarie alla sincronizzazione
    // e delle scorte di magazzino iniziali
    
    magazzino->mutexl = semget(IPC_PRIVATE,MAX_PRODOTTI,IPC_CREAT|0664);
    magazzino->synch = semget(IPC_PRIVATE,MAX_PRODOTTI,IPC_CREAT|0664);

    for (int i = 0; i < MAX_PRODOTTI; i++)
    {
        magazzino->scorta[i] = SCORTA_INIZIALE;
        semctl(magazzino->mutexl,i,SETVAL,1);
        semctl(magazzino->synch,i,SETVAL,1);
        magazzino->numlettori[i] = 0;
    }
}

void destroy_magazzino(Magazzino *magazzino){
    for (int i = 0; i < MAX_PRODOTTI; i++)
    {
        semctl(magazzino->mutexl,i,IPC_RMID);
        semctl(magazzino->synch,i,IPC_RMID);
    }
}

void lettore(Magazzino *magazzino) {
    int ret;
    Msg_Req m_req;
    Msg_Resp m_resp;
    int k;

    for (k=0; k<4; k++) {
        // TODO aggiungere codice per la ricezione comando
        
        ret = msgrcv(ds_queue_request,&m_req,sizeof(Msg_Req)-sizeof(long),MSG_TYPE_READ,0);

        // TODO Aggiungere codice per la lettura:
        // InizioLettura
        InizioLettura(magazzino,m_req.id_prodotto);
        // Lettura del livello di scorta del prodotto il cui id è indicato nella richiesta

        m_resp.id_prodotto = m_req.id_prodotto;
        m_resp.pid = m_req.mittente;
        m_resp.scorta = magazzino->scorta[m_req.id_prodotto];

        // FineLettura
        FineLettura(magazzino,m_req.id_prodotto);

        // TODO Costruire il messaggio di risposta m_resp

        printf("[lettore] Ricevuta richiesta di lettura per prodotto %d con livello scorte %d\n",m_resp.id_prodotto,m_resp.scorta);

        // TODO aggiungere codice per l'invio del messaggio di risposta

        ret = msgsnd(ds_queue_response,&m_resp,sizeof(Msg_Resp)-sizeof(long),0);
    }
}

void scrittore(Magazzino *magazzino){
    int ret;
    Msg_Req m_req;
    int k;

    for (k = 0; k < 2; k++) {
        // TODO agiungere codice per la ricezione del comando
        
        ret = msgrcv(ds_queue_request,&m_req,sizeof(Msg_Req)-sizeof(long),MSG_TYPE_WRITE,0);

        // TODO aggiungere codice per la scrittura
        // InizioScrittura
        InizioScrittura(magazzino,m_req.id_prodotto);
        // Decremento del livello di scorta del prodotto indicato nella richiesta della quantità indicata nella richiesta

        magazzino->scorta[m_req.id_prodotto] -= m_req.value;

        // FineScrittura
        FineScrittura(magazzino,m_req.id_prodotto);

        printf("[scrittore] Ricevuta richiesta di decremento di %d per prodotto %d\n",m_req.value, m_req.id_prodotto);
    }
}

void InizioLettura(Magazzino *magazzino, int id_prodotto){
    //TODO aggiungere il codice di inizio lettura, utilizzando i semafori
    // secondo lo schema lettori-scrittori con starvation degli scrittori

    Wait_Sem(magazzino->mutexl,id_prodotto);
    magazzino->numlettori[id_prodotto]++;

    if (magazzino->numlettori[id_prodotto] == 1) Wait_Sem(magazzino->synch,id_prodotto);

    Signal_Sem(magazzino->mutexl,id_prodotto);
}

void FineLettura(Magazzino *magazzino, int id_prodotto){
    //TODO aggiungere il codice di fine lettura, utilizzando i semafori
    // secondo lo schema lettori-scrittori con starvation degli scrittori

    Wait_Sem(magazzino->mutexl,id_prodotto);
    magazzino->numlettori[id_prodotto]--;

    if (magazzino->numlettori[id_prodotto] == 0) Signal_Sem(magazzino->synch,id_prodotto);

    Signal_Sem(magazzino->mutexl,id_prodotto);
}

void InizioScrittura(Magazzino *magazzino, int id_prodotto){
    //TODO aggiungere il codice di inizio scrittura, utilizzando i semafori
    // secondo lo schema lettori-scrittori con starvation degli scrittori
    Wait_Sem(magazzino->synch,id_prodotto);
}
void FineScrittura(Magazzino *magazzino, int id_prodotto){
    //TODO aggiungere il codice di fine scrittura, utilizzando i semafori
    // secondo lo schema lettori-scrittori con starvation degli scrittori
    Signal_Sem(magazzino->synch,id_prodotto);
}
