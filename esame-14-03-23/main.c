#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include "header.h"


int main(){

    
     //TODO: completare richiesta shared memory ed inizializzazione
     key_t c_mem1= IPC_PRIVATE;
     key_t c_mem2= IPC_PRIVATE;
     Info_Volo *volo1, *volo2;
     // ...
     int shmid1 = shmget(c_mem1,sizeof(Info_Volo),IPC_CREAT|0664);
     int shmid2 = shmget(c_mem2,sizeof(Info_Volo),IPC_CREAT|0664);

     volo1 = (Info_Volo *) shmat(shmid1,0,0);
     volo2 = (Info_Volo *) shmat(shmid2,0,0);

     //TODO inizializzare id_volo pari a 1 per volo1 e pari a 2 per volo2
     volo1->id_volo = 1;
     volo2->id_volo = 2;
     volo1->numlettori = 0;
     volo2->numlettori = 0;

     //TODO: compleare richiesta semafori ed inizializzazione
     key_t c_sem1 = IPC_PRIVATE;
     key_t c_sem2 = IPC_PRIVATE;
     
     volo1->id_sem = semget(c_sem1,2,IPC_CREAT|0664);
     volo2->id_sem = semget(c_sem2,2,IPC_CREAT|0664);

     semctl(volo1->id_sem,MUTEXL,SETVAL,1);
     semctl(volo1->id_sem,SYNCH,SETVAL,1);
     semctl(volo2->id_sem,MUTEXL,SETVAL,1);
     semctl(volo2->id_sem,SYNCH,SETVAL,1);

     //TODO: compleatare richiesta coda messaggi
     key_t c_coda= ftok(".",'a');
     // ...
     int coda = msgget(c_coda,IPC_CREAT|IPC_EXCL|0664);

     if (coda < 0)
     {
          printf("Risorsa gia' esistente\n");
          coda = msgget(c_coda,0);
          msgctl(coda,IPC_RMID,0);
          exit(0);
     }
     
     //TODO generazione tramite fork di scrittore e 4 lettori, 2 per il volo 1 e 2 per il volo 2

     pid_t pid = fork();
     if (pid == 0)
     {
          Scrittore(volo1,volo2);
          exit(0);
     }

     for (int i = 0; i < NUM_LETTORI; i++)
     {
          pid_t pid1 = fork();
          if (pid1 == 0)
          {
               if ((i%2) == 0) Lettore(volo1,coda);
               else Lettore(volo2,coda);
               exit(0);
          }
     }
    
     //TODO generazione tramite fork/exec del visualizzatore

     pid_t pid2 = fork();
     if (pid2 == 0)
     {
          execl("./visualizzatore","visualizzatore",NULL);
          exit(0);
     }
     

     //TODO: attesa terminazione dei "numprocessi"  figli
     int numprocessi = NUM_LETTORI+2;

     for (int i = 0; i < numprocessi; i++) wait(NULL);
    

     //TODO: Deallocazione risorse

     shmctl(shmid1,IPC_RMID,0);
     shmctl(shmid2,IPC_RMID,0);
     semctl(volo1->id_sem,0,IPC_RMID);
     semctl(volo2->id_sem,0,IPC_RMID);
     msgctl(coda,IPC_RMID,0);

     return 0;
}
