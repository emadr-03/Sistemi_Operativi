#define NUM_UPDATES 3
#define MAX_VALUES 4
#define NUM_CONS 12
#define MSG_RICHIESTA 1
#define MSG_RISPOSTA 2

typedef struct {
	long type;
} req; 

typedef struct {
	long type;
	int value;
} res; 


