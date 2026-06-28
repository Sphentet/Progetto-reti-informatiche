#ifndef _UTENTE_H_
#define _UTENTE_H_

#include "../definizioni.h"



int hello(int, int);//effettua la registrazione con la lavagna

int quit(int);//effettua la disconnessione dalla lavagna

int create_card(int);//legge da tastiera i dati della card, li formatta e li invia alla lavagna

char* recv_user_list(int);//fa la recv del numero utenti e della lista delle porte utente

char* request_user_list(int);//invia il comando REQUEST_USER_LIST alla lavagna e poi usa recv_user_list per ricevere i dati

void ack_card(int);//invia alla lavagna un byte di conferma della corretta ricezione della card assegnata

void pong_lavagna(int);//invia alla lavagna un byte come pong

void* review_card(void*);//routine per i thread che gestiscono l'invio delle richieste di review al completamento della card

void* gestore_input(void*);//routine per i thread che gestiscono gli input da tastiera

void* gestore_richieste(void*);//routine per i thread che ascoltano le richieste della lavagna



#endif