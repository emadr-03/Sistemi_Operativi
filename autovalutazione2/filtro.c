#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "header.h"

void filtro(int ds_queue_gen_filter, int ds_queue_filter_checksum){

        int ret, i;
        message mess;

        for (i=0; i<NUM_MESSAGES; i++){

                ret = msgrcv(ds_queue_gen_filter,&mess,sizeof(message)-sizeof(long),0,0);

                if(ret<0) {
                        perror("msgrcv on ds_queue_gen_filter FALLITA!");
                        exit(-1);
                }

                printf("[filtro] Ricevuto #%d messaggio dal generatore...\n", i);
                //ricerca del carattere 'x'
                printf("[filtro] Ricerca carattere 'x' sulla stringa: %s...\n", mess.stringa);
            
                /* TODO: effettuare la ricerca del caratter 'x' e inviare il messaggio al processo checksum nel caso di carattere non trovato */
                
                if (strchr(mess.stringa,'x') == NULL) ret = msgsnd(ds_queue_filter_checksum,&mess,sizeof(message)-sizeof(long),0);
                else printf("[Filtro] Carattere 'x' trovato\n");
        }
    
        exit(0);
}

