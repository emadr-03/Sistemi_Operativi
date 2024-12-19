#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include "header.h"
#include <time.h>
#include <sys/wait.h>

int main() {
    /*
        TODO: recupero code
    */

    key_t chiave_coda_richieste = ftok(".",'a');
    key_t chiave_coda_risposte = ftok(".",'b');

    int coda_richieste = msgget(chiave_coda_richieste,0);
    int coda_risposte = msgget(chiave_coda_risposte,0);

    if (coda_richieste == -1 || coda_risposte == -1) {
        perror("Errore nell'apertura delle code");
        exit(1);
    }

    for (int i = 0; i < NUM_BAMBINI; i++) {
        /* TODO */
        pid_t pid = fork();

        MessaggioRichiesta m;
        MessaggioRisposta m1;
        int ret;

        if (pid == 0)
        {
            srand(time(NULL)*getpid());
            for (int j = 0; j < NUM_LETTERE; j++) {
                /*
                    TODO: costruzione e invio del messaggio
                */

               m.type = 1;

                m.val = (rand() % 5) + 1;

                ret = msgsnd(coda_richieste,(void *)&m,sizeof(MessaggioRichiesta)-sizeof(long),0);
                if (ret < 0)
                {
                    perror("[Client] Errore invio messaggio");
                    exit(0);
                }

                printf("Invio lettera a Babbo Natale chiedendo %d regali\n", m.val);

                /*
                    TODO: attesa del messaggio di risposta
                */

                ret = msgrcv(coda_risposte,(void *)&m1,sizeof(MessaggioRisposta)-sizeof(long),0,0);
                if (ret < 0)
                {
                    perror("[Client] Errore ricezione messaggio risposta");
                    exit(0);
                }
                printf("[Bambino %d] Lettera %d: conferma ricevuta\n", i + 1, j + 1);
            }
            exit(0);
        }
    }

    /*
        TODO: Attesa della terminazione dei processi figli
    */

   for (int i = 0; i < NUM_BAMBINI; i++) wait(NULL);

    return 0;
}
