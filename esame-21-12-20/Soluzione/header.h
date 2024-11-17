#ifndef HEADER_H
#define HEADER_H

#define MSG_TYPE_READ 1
#define MSG_TYPE_WRITE 2

#define MAX_PRODOTTI 3

#define PATH_REQ "."
#define PATH_RESP "."
#define ID_REQ 'a'
#define ID_RESP 'b'

typedef struct {
    long tipo_richiesta;
    long mittente;
    int id_prodotto;
    int value;
} Msg_Req;

typedef struct {
    long pid;
    int id_prodotto;
    int scorta;
} Msg_Resp;

#endif
