#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header.h"

#define MAX_DECR 4;

int main(){
    
    //Creazione code richieste e risposte
    int ds_queue_request, ds_queue_response;

    //create queues
    int key_queue_request = ftok(PATH_REQ, ID_REQ);
    int key_queue_response = ftok(PATH_RESP, ID_RESP);
    
    ds_queue_request = msgget(key_queue_request, 0);
    ds_queue_response = msgget(key_queue_response, 0);
    
    printf("[updater] Coda ds_queue_request ID: %d\n", ds_queue_request);
    printf("[updater] Coda ds_queue_response ID: %d\n", ds_queue_response);
    
    
    Msg_Req m_req;

    m_req.tipo_richiesta = MSG_TYPE_WRITE;
    m_req.mittente = getpid();
    
    int i, ret;

    srand(time(NULL)*getpid());
    
    for (i=0; i<2; i++){

        m_req.id_prodotto = rand()%MAX_PRODOTTI;
        m_req.value = 1+rand()%MAX_DECR;
        
        ret = msgsnd(ds_queue_request, &m_req, sizeof(Msg_Req) - sizeof(long), 0);
        if (ret<0){
            perror("[updater] msgsnd invio richiesta FALLITA!");
            exit(-1);
        }

        printf("[updater] Richiesto decremento di %d unità del prodotto %d\n",m_req.value,m_req.id_prodotto);
    }
    
    return 0;
}


