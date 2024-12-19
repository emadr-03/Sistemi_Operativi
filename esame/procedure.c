#include "header.h"
#include "monitor_signal_continue.h"

void inizializza_monitor(MonitorLettScritt *ls) {
	/*
        TODO: completare
    */
    init_monitor(&(ls->m),2);
    ls->numlettori = 0;
    ls->totale_regali = 0;
    ls->numscrittori = 0;
}

void distruggi_monitor(MonitorLettScritt *ls) {
	/*
        TODO: completare
    */
   remove_monitor(&(ls->m));
}

void inizio_lettura(MonitorLettScritt *ls) {
	/*
        TODO: completare
    */

    enter_monitor(&(ls->m));

    while (ls->numscrittori > 0) wait_condition(&(ls->m),VARCOND_LETTORI);

    ls->numlettori++;

    leave_monitor(&(ls->m));
}

void fine_lettura(MonitorLettScritt *ls) {
	/*
        TODO: completare
    */

    enter_monitor(&(ls->m));

    ls->numlettori--;

    if (ls->numlettori == 0) signal_condition(&(ls->m),VARCOND_SCRITTORI);

    leave_monitor(&(ls->m));
}

void inizio_scrittura(MonitorLettScritt *ls) {
	/*
        TODO: completare
    */

    enter_monitor(&(ls->m));

    while (ls->numlettori > 0 || ls->numscrittori > 0) wait_condition(&(ls->m),VARCOND_SCRITTORI);

    ls->numscrittori++;

    leave_monitor(&(ls->m));
}

void fine_scrittura(MonitorLettScritt *ls) {
	/*
        TODO: completare
    */
    enter_monitor(&(ls->m));

    ls->numscrittori--;

    if (queue_condition(&(ls->m),VARCOND_SCRITTORI)) signal_condition(&(ls->m),VARCOND_SCRITTORI);
    else signal_all(&(ls->m),VARCOND_LETTORI);

    leave_monitor(&(ls->m));
}

void Lettore(int codaresp, MonitorLettScritt *ls) {

	/*
        TODO: completare
    */
    int ret;
    MessaggioRisposta m;

    inizio_lettura(ls);

    printf("[Lettore] Il totale regali richiesti finora è: %d\n", ls->totale_regali);

    fine_lettura(ls);

    /*
        TODO: invio messaggio conferma di ricezione 
    */
    m.type = 1;
    ret = msgsnd(codaresp,(void *)&m,sizeof(MessaggioRisposta)-sizeof(long),0);
    if (ret < 0)
    {
        perror("[Lettore] Errore invio messaggio");
        exit(0);
    }

    printf("[Lettore] Ho inviato la conferma di ricezione\n");
}

void Scrittore(int codareq, MonitorLettScritt *ls) {
    /*
        TODO: attesa messaggio
    */
    int ret;
    MessaggioRichiesta m;

    ret = msgrcv(codareq,(void *)&m,sizeof(MessaggioRichiesta)-sizeof(long),0,0);
    if (ret < 0)
    {
        perror("[Scrittore] Errore ricezione messaggio");
        exit(0);
    }

	/*
        TODO: completare scrittura
    */

    inizio_scrittura(ls);

    ls->totale_regali += m.val;

    printf("[Scrittore %d] Adesso il totale dei regali richiesti è: %d\n",getpid(), ls->totale_regali);

    fine_scrittura(ls);
}
