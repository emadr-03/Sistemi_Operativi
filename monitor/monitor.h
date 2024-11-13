#ifndef MONITOR_H
#define MONITOR_H

typedef struct
{
    int mutex;
    int semcond;
    int semurg;
    int num_var_cond;
    int shmid;
    int *cond_counts;
    int *urg_counts;
}Monitor;

void initmonitor(Monitor *m,int num_var);
void entermonitor(Monitor *m);
void leavemonitor(Monitor *m);
void removemonitor(Monitor *m);
void Wait_Cond(Monitor *m,int numvar);
void Signal_Cond(Monitor *m,int numvar);

#endif