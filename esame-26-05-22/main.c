#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#include "prodcons.h"

void Produttore(MonitorVettoreCircolare *p, MonitorBufferSingolo *b);
void Consumatore(MonitorVettoreCircolare *p, MonitorBufferSingolo *b);

int main() {

    pid_t pid;
    int shmidcircolare = shmget(IPC_PRIVATE,sizeof(MonitorVettoreCircolare),IPC_CREAT|0664);

    MonitorVettoreCircolare * p = (MonitorVettoreCircolare *) shmat(shmidcircolare,0,0);

    init_monitor_circolare(p);


    int shmidsingolo = shmget(IPC_PRIVATE,sizeof(MonitorBufferSingolo),IPC_CREAT|0664);
 
    MonitorBufferSingolo * b = (MonitorBufferSingolo *) shmat(shmidsingolo,0,0);

    init_monitor_buffer_singolo(b);


    /* TBD: Creare un processo figlio, che esegua la funzione "Produttore()" */
    pid = fork();
    if (pid == 0)
    {
        Produttore(p,b);
        exit(0);
    }

    /* TBD: Creare un processo figlio, che esegua la funzione "Consumatore()" */
    pid = fork();
    if (pid == 0)
    {
        Consumatore(p,b);
        exit(0);
    }

    /* TBD: Attenere la terminazione dei processi figli */
    wait(NULL);
    wait(NULL);

    remove_monitor_circolare(p);
    remove_monitor_buffer_singolo(b);

    /* TBD: Deallocare gli oggetti monitor */

    shmctl(shmidcircolare,IPC_RMID,0);
    shmctl(shmidsingolo,IPC_RMID,0);

    return 0;
}


void Produttore(MonitorVettoreCircolare *p, MonitorBufferSingolo *b) {

    srand(time(NULL)^getpid());
    int numero_elementi = rand() % 16;

    printf("[PRODUTTORE] Totale elementi da produrre: %d\n", numero_elementi);

    /* TBD: Effettuare una produzione su MonitorBufferSingolo con
            la funzione "produzione_buffer_singolo", inserendo "numero_elementi"
     */

    produzione_buffer_singolo(b,numero_elementi);

    for(int i=0; i<numero_elementi; i++) {
    
        sleep(1);

        int val = rand() % 81;

        //printf("[PRODUTTORE] Prodotto: %d\n", val);

        /* TBD: Chiamare la funzione "produzione_circolare" per produrre
                il valore "val"
         */

        produzione_circolare(p,val);
    }
}

void Consumatore(MonitorVettoreCircolare *p, MonitorBufferSingolo *b) {

    /* TBD: Effettuare una consumazione su MonitorBufferSingolo con
            la funzione "consumazione_buffer_singolo", prelevando "numero_elementi"
     */


    int numero_elementi = consumazione_buffer_singolo(b);

    printf("[CONSUMATORE] Totale elementi da consumare: %d\n", numero_elementi);


    for(int i=0; i<numero_elementi; i++) {

        /* TBD: Chiamare la funzione "consumazione_circolare" per prelevare
                un valore e copiarlo in "val"
         */

        int val = consumazione_circolare(p);

        //printf("[CONSUMATORE] Consumato: %d\n", val);
    }
}
