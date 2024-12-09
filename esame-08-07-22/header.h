#define NUM_UPDATES 4
#define NUM_READS 5
#define NUM_READERS 3
#define MSG_RICHIESTA 1
#define MSG_RISPOSTA 2

typedef struct {
	long type;
	int pid;
	int value;
} req; 

typedef struct {
	long type;
	int sender;
	int value;
} res; 


