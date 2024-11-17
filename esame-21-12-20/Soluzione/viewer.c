#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header.h"

int main(){
    
    //Creazione code richieste e risposte
    int ds_queue_request, ds_queue_response;

    //create queues
    int key_queue_request = ftok(PATH_REQ, ID_REQ);
    int key_queue_response = ftok(PATH_RESP, ID_RESP);
    
    ds_queue_request = msgget(key_queue_request, 0);
    ds_queue_response = msgget(key_queue_response, 0);
    
    printf("[viewer] Coda ds_queue_request ID: %d\n", ds_queue_request);
    printf("[viwwer] Coda ds_queue_response ID: %d\n", ds_queue_response);
    
    
    Msg_Req m_req;
    Msg_Resp m_res;

    m_req.tipo_richiesta = MSG_TYPE_READ;
    m_req.mittente = getpid();
    
    int i, ret;

    srand(time(NULL)*getpid());
    
    for (i=0; i<4; i++){

        m_req.id_prodotto = rand()%MAX_PRODOTTI;
        
        ret = msgsnd(ds_queue_request, &m_req, sizeof(Msg_Req) - sizeof(long), 0);
        if (ret<0){
            perror("[viewer] msgsnd invio richiesta FALLITA!");
            exit(-1);
        }

        ret = msgrcv(ds_queue_response, &m_res, sizeof(Msg_Resp) - sizeof(long), getpid(), 0);
        if (ret<0){
            perror("[viewer] Errore rcv ds_queue_response");
            exit(-1);
        }
        printf("[viewer] Sono disponibili %d unità del prodotto %d\n",m_res.scorta,m_res.id_prodotto);
    }
    
    return 0;
}


