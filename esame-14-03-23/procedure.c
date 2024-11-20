#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"


/*******PROCEDURE PER I SEMAFORI******/


void Wait_Sem(int id_sem, int numsem)     {
       struct sembuf sem_buf;
       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=-1;
       semop(id_sem,&sem_buf,1);   //semaforo rosso
}


  void Signal_Sem (int id_sem,int numsem)     {
       struct sembuf sem_buf;
       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=1;
       semop(id_sem,&sem_buf,1);   //semaforo verde
}


/***********************************************/

void Lettore(Info_Volo *volo, int coda){
	int i;
        int ret;
        msg messaggio;
        for (i=0; i<NUM_OPERAZIONI; i++) {

                //TODO: accesso alla risorsa secondo soluzione lettori/scrittori, starvation scrittori
                Wait_Sem(volo->id_sem,MUTEXL);
                volo->numlettori++;

                if (volo->numlettori == 1) Wait_Sem(volo->id_sem,SYNCH);

                Signal_Sem(volo->id_sem,MUTEXL);
                
                //TODO: creazione e invio messaggio al visualizzatore

                printf("[Lettore] ID_VOLO: %d; GATE: %d\n",volo->id_volo,volo->gate);

                messaggio.volo = volo->id_volo;
                messaggio.gate = volo->gate;

                ret = msgsnd(coda,(void *)&messaggio,sizeof(msg)-sizeof(long),0);
               
                sleep(1);

               //TODO: rilascio risorsa secondo soluzione lettori/scrittori, starvation scrittori
                Wait_Sem(volo->id_sem,MUTEXL);
                volo->numlettori--;

                if (volo->numlettori == 0) Signal_Sem(volo->id_sem,SYNCH);

                Signal_Sem(volo->id_sem,MUTEXL);
        }

}

void Scrittore(Info_Volo* volo1, Info_Volo* volo2) {
        int i;
        int val;
        for (i=0; i<NUM_OPERAZIONI; i++) {

                //TODO: accesso alle risorse secondo soluzione lettori/scrittori, starvation scrittori 
                Wait_Sem(volo1->id_sem,SYNCH);
                Wait_Sem(volo2->id_sem,SYNCH);

                //TODO: gate del volo 2 = gate del volo 1
                volo2->gate = volo1->gate;
                printf("[Scrittore] GATE volo 1: %d\n",volo1->gate);
                printf("[Scrittore] GATE volo 2: %d\n",volo2->gate);
                //TODO: gate del volo 1 = valore casuale da 1 a 100 
                srand(time(NULL)*getpid());
                val = (rand() % 100) + 1;
                volo1->gate = val;
                printf("[Scrittore] Valore AGGIORNATO del GATE volo 1: %d\n",volo1->gate);
                sleep(1);
                Signal_Sem(volo1->id_sem,SYNCH);
                Signal_Sem(volo2->id_sem,SYNCH);
        }

}


