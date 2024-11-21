#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include "procedure.h"


#define NUM_THREADS     10

int main()
{
    pthread_t threads[NUM_THREADS]; //array di threads (ID)

    //opzionale attributo joinable (vedere dalla repo github)

    //inizializzo le risorse
    Buffer *buff = (Buffer *) malloc(sizeof(Buffer)); //metto la struct nell'area heap dei thread per passarla come argomento alle routine

    buff->buffer_libero = 1;
    buff->val = 0;
    
    pthread_mutex_init(&(buff->mutex),NULL); //inizializzo il mutex

    pthread_cond_init(&(buff->ok_consumatore),NULL);
    pthread_cond_init(&(buff->ok_produttore),NULL); //inizializzo le variabili condition

    //creazione thread
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (i%2 == 0)
        {
            pthread_create(&threads[i],NULL,consumatore,(void *)buff);
        }
        else
        {
            pthread_create(&threads[i],NULL,produttore,(void *)buff);
        }
    }
    
    //attendo terminazione dei thread
    for (int i = 0; i < NUM_THREADS; i++) pthread_join(threads[i],NULL);

    //dealloco risorse
    pthread_mutex_destroy(&(buff->mutex));
    pthread_cond_destroy(&(buff->ok_consumatore));
    pthread_cond_destroy(&(buff->ok_produttore));

    //rilascio area di memoria
    free(buff);

    pthread_exit(NULL);
}