#ifndef DEFINIZIONI_H
#define DEFINIZIONI_H

#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORTA_LAVAGNA 5678
#define INDIRIZZO_LAVAGNA "127.0.0.1"
#define PORTA_UTENTE_MIN (PORTA_LAVAGNA+1)
#define COLONNE 3
#define LUNG_TESTO 128


//numeri associati ai comandi utente
#define HELLO 0
#define QUIT 1
#define CREATE_CARD 2
#define PONG_LAVAGNA 3
#define ACK_CARD 4
#define REQUEST_USER_LIST 5
#define REVIEW_CARD 6
#define CARD_DONE 7
//numeri associati ai comandi lavagna
#define PING_USER 8
#define MOVE_CARD 9
#define SHOW_LAVAGNA 10
#define SEND_USER_LIST 11
#define HANDLE_CARD 12

//costanti per stampa lavagna
#define SPAZI_DX 14
#define LARG_COL 33


struct strutt_utente_t {
    uint16_t porta;
    int sd_utol;
    int sd_ltou;
    int id_card;
    struct strutt_utente_t* next_utente;
}typedef strutt_utente_t, utente_t;

struct strutt_card_t {
    uint32_t id;
    int colonna;
    char testo[LUNG_TESTO + 1];
    uint16_t utente;
    time_t ultima_modifica;
    struct strutt_card_t* next_card;
}typedef strutt_card_t, card_t;

struct strutt_lavagna_t {
    int id;
    card_t* colonne[COLONNE];
    utente_t* utenti;
    int num_utenti; 
}typedef strutt_lavagna_t, lavagna_t;



#endif