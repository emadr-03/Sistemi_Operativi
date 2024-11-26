#define NUM_UPDATES 10
#define NUM_READS 20
#define NUM_READERS 3
#define MSG_REQ 1
#define MSG_RES 2


typedef struct {
	//TODO: completare con la struttura del messaggio di richiesta
	long type;
	int mittente;
	int valore;
} req; 

typedef struct {
	//TODO: completare con la struttura del messaggio di risposta
	long type;
	int destinatario;
	int valore;
} res; 


