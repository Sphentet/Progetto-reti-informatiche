#ifndef _LAVAGNA_H_
#define _LAVAGNA_H_

#include "../definizioni.h"



void inizializza_lavagna();//inizializzazione della lavagna

void distruggi_lavagna();//dealloca la memoria allocata per la lavagna

void show_lavagna();//stampa la lavagna con eventuali card

uint16_t rimuovi_utente(int);//rimuovo l'utente dalla lista della lavagna e ritorna il suo numero di porta

void check_card_utente_libero(int);//controlla se c'è una card disponibile e un utente a cui assegnarla, se si chiama handle_card

int move_card(int, int, int, int);//sposta la card dalla colonna from alla colonna to

int send_user_list(int);//invia all'utente il numero di utenti connessi euna stringa contenente tutti i numeri di porta degli utenti connessi

int handle_card(int, uint16_t);//assegna ad un utente una card in to do

void* ping_user(void*);//routine per il thread secondario che fa sleep(90) e poi PING_USER

void* gestore_connessione_utente(void*);//routine per i thread che ascoltano le richieste degli utenti

void* gestore_input_lavagna(void*);//routine per il thread che gestisce gli input da tastiera



#endif