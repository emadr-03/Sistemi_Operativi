#include "header.h"



void inizializza(monitor* m){

	m->molo=0;
	m->id_nave=0;

    /* TODO: Inizializzare le variabili dell'algoritmo, il mutex, e le variabili condition */
    pthread_mutex_init(&(m->mutex),NULL);
    pthread_cond_init(&(m->cond_lettori),NULL);
    pthread_cond_init(&(m->cond_scrittori),NULL);
    m->num_scrittori_in_attesa = 0;
    m->numlettori = 0;
    m->numscrittori = 0;
}

void rimuovi (monitor* m){
    /* TODO: Disattivare mutex e variabili condition */
    pthread_mutex_destroy(&(m->mutex));
    pthread_cond_destroy(&(m->cond_lettori));
    pthread_cond_destroy(&(m->cond_scrittori));
}


//SCRITTURA. AGGIORNAMENTO DELLA POSIZIONE DEL TRENO
void scrivi_molo(monitor* m, int molo){

    /* TODO: Implementare qui lo schema dei lettori-scrittori con starvation di entrambi.
     * nella parte della SCRITTURA
     */

    pthread_mutex_lock(&(m->mutex));

    while (m->numscrittori > 0 || m->numlettori > 0)
    {
        m->num_scrittori_in_attesa++;
        pthread_cond_wait(&(m->cond_scrittori),&(m->mutex));
        m->num_scrittori_in_attesa--;
    }

    m->numscrittori++;

    pthread_mutex_unlock(&(m->mutex));

    m->molo = molo;

    pthread_mutex_lock(&(m->mutex));

    m->numscrittori--;

    if (m->num_scrittori_in_attesa > 0) pthread_cond_signal(&(m->cond_scrittori));
    else pthread_cond_broadcast(&(m->cond_lettori));

    pthread_mutex_unlock(&(m->mutex));
}


//LETTURA. RESTITUISCE LA POSIZIONE DEL TRENO
int leggi_molo(monitor* m){

    /* TODO: Implementare qui lo schema dei lettori-scrittori con starvation di entrambi.
     * nella parte della LETTURA
     */
    int pos;

    pthread_mutex_lock(&(m->mutex));

    while (m->numscrittori > 0) pthread_cond_wait(&(m->cond_lettori),&(m->mutex));

    m->numlettori++;

    pthread_mutex_unlock(&(m->mutex));

    pos = m->molo;

    pthread_mutex_lock(&(m->mutex));

    m->numlettori--;

    if (m->numlettori == 0) pthread_cond_signal(&(m->cond_scrittori));

    pthread_mutex_unlock(&(m->mutex));

    return pos;
}

