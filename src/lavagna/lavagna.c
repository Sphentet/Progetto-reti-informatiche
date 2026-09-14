#include "../../include/lavagna/lavagna.h"

lavagna_t lav;
pthread_mutex_t mutex_lav;



void inizializza_lavagna(){

    lav.id=0;

    lav.colonne[0]=NULL;
    lav.colonne[1]=NULL;
    lav.colonne[2]=NULL;

    for(int i=1; i<=10; ++i){

        //creo la card
        card_t *c=NULL;
        card_t* newc=malloc(sizeof(card_t));
        newc->id=i;
        newc->colonna=0;
        newc->utente=0;
        newc->next_card=NULL;

        char id[9];
        sprintf(id, "testo %d", i);
        strncpy(newc->testo, id, LUNG_TESTO+1);
        newc->testo[LUNG_TESTO] = '\0';

        time_t tempo;
        time(&tempo);
        newc->ultima_modifica=tempo;

        //inserisco la card nella colonna to do
        if(!lav.colonne[0]){
            lav.colonne[0]=newc;
        }
        else{
            c=lav.colonne[0];
            while(c->next_card) c=c->next_card;
            c->next_card=newc;
        } 

    }

    

    lav.utenti=NULL;
    lav.num_utenti=0;

}



void distruggi_lavagna(){
    while(lav.utenti) rimuovi_utente(lav.utenti->sd_utol);

    for(int i=0; i<3; ++i){
        while(lav.colonne[i])   move_card(i, -1, lav.colonne[i]->utente, -1);
    }
}



void show_lavagna(){
    printf("-------------------------------------------------------------------------------------------------------\n");
    printf("|                                             Lavagna - %d                                             |\n", lav.id);
    printf("|-----------------------------------------------------------------------------------------------------|\n");
    printf("|              To Do              |              Doing              |              Done               |\n");
    printf("|-----------------------------------------------------------------------------------------------------|\n");
    printf("|                                                                                                     |\n");


    //stampa card
    

    card_t *c0=lav.colonne[0], *c1=lav.colonne[1], *c2=lav.colonne[2];
    int f0=2, f1=2, f2=2;//flag che indica se stampare l'id o il testo
    int l0=1, l1=1, l2=1;//flag che indica il numero di righe del testo stampate+1

    while(c0||c1||c2){
        
        if(!c0||f0==2){
            printf("|                                 |");
            f0=0;
        } 
        else{
            //stampa id card to do
            if(!f0){
                char buf[4];
                memset(buf, 0, sizeof(buf));
                printf("|             Task %d", c0->id);
                sprintf(buf, "%d", c0->id);
                int len=strlen(buf);
                while(SPAZI_DX>=len){
                    printf(" ");
                    len++;
                }
                printf("|");
                f0++;

            }
            //stampa testo card to do
            else{
                printf("|");
                int len=strlen(c0->testo);
                int start=LARG_COL*(l0-1);
                if(len>LARG_COL*l0){
                    for(int i=start; i<LARG_COL*l0; ++i){
                        printf("%c", c0->testo[i]);
                    }
                    l0++;
                }
                else{
                    int mezzo=(LARG_COL*l0-len)/2;
                    for(int i=0; i<mezzo; ++i){
                        printf(" ");
                    }

                    for(int i=start; i<len; ++i){
                        printf("%c", c0->testo[i]);
                    }

                    if((LARG_COL*l0-len)%2!=0) mezzo++;
                    for(int i=0; i<mezzo; ++i){
                        printf(" ");
                    }
                    c0=c0->next_card;
                    f0++;
                    l0=1;

                }

                printf("|");
                

            }
        }

        if(!c1||f1==2){
            printf("                                 |");
            f1=0;
        } 
        else{
            //stampa id card doing
            if(!f1){
                char buf[4];
                memset(buf, 0, sizeof(buf));
                printf("             Task %d", c1->id);
                sprintf(buf, "%d", c1->id);
                int len=strlen(buf);
                while(SPAZI_DX>=len){
                    printf(" ");
                    len++;
                }
                printf("|");
                f1++;

            }
            //stampa testo card doing
            else{
                
                int len=strlen(c1->testo);
                if(len>LARG_COL*l1){
                    for(int i=LARG_COL*(l1-1); i<LARG_COL*l1; ++i){
                        printf("%c", c1->testo[i]);
                    }
                    l1++;
                }
                else{
                    int mezzo=(LARG_COL*l1-len)/2;
                    for(int i=0; i<mezzo; ++i){
                        printf(" ");
                    }

                    for(int i=LARG_COL*(l1-1); i<len; ++i){
                        printf("%c", c1->testo[i]);
                    }

                    if((LARG_COL*l1-len)%2!=0) mezzo++;
                    for(int i=0; i<mezzo; ++i){
                        printf(" ");
                    }
                    c1=c1->next_card;
                    f1++;
                    l1=1;
                }

                printf("|");
                
            }
        }

        if(!c2||f2==2){
            printf("                                 |\n");
            f2=0;
        } 
        else{
            //stampa id card done
            if(!f2){
                char buf[4];
                memset(buf, 0, sizeof(buf));
                printf("             Task %d", c2->id);
                sprintf(buf, "%d", c2->id);
                int len=strlen(buf);
                while(SPAZI_DX>=len){
                    printf(" ");
                    len++;
                }
                printf("|\n");
                f2++;

            }
            //stampa testo card done
            else{
                int len=strlen(c2->testo);
                if(len>LARG_COL*l2){
                    for(int i=LARG_COL*(l2-1); i<LARG_COL*l2; ++i){
                        printf("%c", c2->testo[i]);
                    }
                    l2++;
                }
                else{
                    int mezzo=(LARG_COL*l2-len)/2;
                    for(int i=0; i<mezzo; ++i){
                        printf(" ");
                    }

                    for(int i=LARG_COL*(l2-1); i<len; ++i){
                        printf("%c", c2->testo[i]);
                    }

                    if((LARG_COL*l2-len)%2!=0) mezzo++;
                    for(int i=0; i<mezzo; ++i){
                        printf(" ");
                    }
                    c2=c2->next_card;
                    f2++;
                    l2=1;
                }

                printf("|\n");
                
            }
        }

    } 
    printf("|                                 |                                 |                                 |\n");
    printf("|                                                                                                     |\n");
    printf("-------------------------------------------------------------------------------------------------------\n");
    fflush(stdout);

    

}   



uint16_t rimuovi_utente(int sd_u){
    
    //cerco l'utente e lo estraggo
    uint16_t num=0;
    utente_t *u1=lav.utenti, *u2=u1;
    if(u1==NULL){
        return 0;
    }
    if(lav.utenti->sd_utol==sd_u){//utente in testa
        lav.utenti=lav.utenti->next_utente;
    }
    else{
        while(u1&&u1->sd_utol!=sd_u){
            u2=u1;
            u1=u1->next_utente;
        }
        if(u1==NULL){
            return 0;
        }
        u2->next_utente=u1->next_utente;

    }
    
    num=u1->porta;

    //chiudo i socket
    if(u1->sd_ltou>=0) close(u1->sd_ltou);
    if(u1->sd_utol>=0) close(u1->sd_utol);

    free(u1);
    lav.num_utenti--;


    printf("l'utente %d si è disconnesso\n", num);


    return num;
}



void check_card_utente_libero(int porta){

    //controllo se c'è una card in to do
    if(lav.colonne[0]){
        //card disponibile, provo ad assegnarla ad un utente se ne trovo uno libero
        utente_t *u=lav.utenti;
        while(u&&(u->id_card!=0||((porta>0)? u->porta!=porta : 0))) u=u->next_utente;
        if(u){
            //invio la lista delle porte utente e chiamo handle_card
            int hand=handle_card(u->sd_ltou, u->porta);
            if(hand==-1){
                rimuovi_utente(u->sd_utol);
                pthread_mutex_unlock(&mutex_lav);
                pthread_exit(NULL);
            }else{
                if(hand==1){
                    move_card(0, 1, u->porta, lav.colonne[0]->id);
                }
                if(send_user_list(u->sd_ltou)==-1){//invio la lista degli utenti
                    rimuovi_utente(u->sd_utol);
                    pthread_mutex_unlock(&mutex_lav);
                    pthread_exit(NULL);
                }
            }
        }
    }

}



int move_card(int from, int to, int utente, int id){
    

    card_t* c1=lav.colonne[from], *c2=c1, *c3=NULL;

    //cerco la card per id nella colonna from e la estraggo
    if(id>=0){
        if(c1!=NULL){
            if(lav.colonne[from]->id==id){//card in testa
                lav.colonne[from]=lav.colonne[from]->next_card;
            }
            else{
                while(c1&&c1->id!=id){
                    c2=c1;
                    c1=c1->next_card;
                }
                //rimuovo la card dalla colonna
                if(c1!=NULL) c2->next_card=c1->next_card;
                else c2->next_card=c1;
                
            }
        }
    }
    else{
        //cerco card assegnata a utente nella colonna from e la estraggo
        if(c1!=NULL){
            if(lav.colonne[from]->utente==utente){//card in testa
                lav.colonne[from]=lav.colonne[from]->next_card;
            }
            else{
                while(c1&&c1->utente!=utente){
                    c2=c1;
                    c1=c1->next_card;
                }
                //rimuovo la card dalla colonna
                if(c1!=NULL) c2->next_card=c1->next_card;
                else c2->next_card=c1;
                
            }
        }
    }
    

    //se to==-1 sto svuotando la colonna
    if(to==-1){
        free(c1);
        return 1;
    }

    //c1 punta alla card o è null
    //se la card c'è sposto nella colonna to
    if(c1!=NULL){
        c3=lav.colonne[to];


        card_t* newc=malloc(sizeof(card_t));
        newc->id=c1->id;
        newc->colonna=to;
        strncpy(newc->testo, c1->testo, LUNG_TESTO);
        newc->testo[LUNG_TESTO] = '\0';
        //cambio il campo utente in base al contesto in cui ho chiamato la funzione
        //(se cerco per id sto assegnando la card)
        if(id>=0) newc->utente=utente;
        else newc->utente=(!to)? 0 : c1->utente;

        //se l'utente ha finito la card aggiorno il suo campo id card
        if(to==2&&id<0){
            utente_t* u=lav.utenti;
            while(u&&u->porta!=utente) u=u->next_utente;
            if(u) u->id_card=0;
        }


        
        time_t tempo_attuale;
        time(&tempo_attuale);
        newc->ultima_modifica=tempo_attuale;
        newc->next_card=NULL;

        if(!c3){
            lav.colonne[to]=newc;
        }
        else{
            while(c3->next_card)   c3=c3->next_card;
            c3->next_card=newc;
        }
        
        free(c1);
    }
    else{
        if(to){//se to=0 stavo cercando card di un utente disconnesso, va bene non trovarla, se è 1 o 2 c'è stato un errore
            return -1;
        } 
    }
    
    show_lavagna();
    return 1;


}



int send_user_list(int sd){
    //max 5 interi per ogni porta, non considero l'utente a cui invio la lista, +1 per il terminatore
    int dim=((lav.num_utenti-1)*5)+1;
    char buf[dim];
    memset(buf, 0, sizeof(buf));
    buf[0] = '\0';

    utente_t *u=lav.utenti;
    char temp[610];
    memset(temp, 0, sizeof(temp));

    //concateno tutte le porte tranne quella dell'utente stesso
    while(u!=NULL){
        if(u->sd_ltou!=sd&&u->sd_utol!=sd){
            sprintf(temp, "%d ", u->porta);
            strcat(buf, temp);
        }
        u=u->next_utente;
    }
    //lascio il mutex prima delle send (l'ho preso prima della chiamata di questa funzione), lo riprendo prima di uscire dalla funzione
    pthread_mutex_unlock(&mutex_lav);

    //invio prima la lunghezza della stringa
    char num_utenti[6];
    memset(num_utenti, 0, sizeof(num_utenti));
    sprintf(num_utenti, "%d", lav.num_utenti);
    int ret=send(sd, num_utenti, sizeof(num_utenti), 0);
    if(ret<=0){
        if(ret<0) perror("send numero utenti");
        pthread_mutex_lock(&mutex_lav);
        return -1;
    }


    //invio la stringa
    int len=sizeof(buf); 
    ret=send(sd, buf, len, 0);
    if(ret<=0){
        if(ret<0) perror("send lista utenti");
        pthread_mutex_lock(&mutex_lav);
        return -1;
    }

    pthread_mutex_lock(&mutex_lav);

    return 1;


}



int handle_card(int sd, uint16_t porta){
    

    //formattazione dati
    char dati[LUNG_TESTO+5];
    memset(dati, 0, sizeof(dati));
    sprintf(dati, "%4d", lav.colonne[0]->id);
    strncpy(dati+4, lav.colonne[0]->testo, LUNG_TESTO);

    //lascio il mutex prima delle send (l'ho preso prima della chiamata di questa funzione), lo riprendo prima di uscire dalla funzione
    pthread_mutex_unlock(&mutex_lav);


    //invio il comando
    char comando[3];
    memset(comando, 0, sizeof(comando));
    sprintf(comando, "%d", HANDLE_CARD);
    int ret=send(sd, (void*)&comando, sizeof(comando), 0);
    if(ret<=0){
        if(ret<0) perror("send handle card");
        pthread_mutex_lock(&mutex_lav);
        return -1;
    }


    //invio i dati
    ret=send(sd, (void*)&dati, sizeof(dati), 0);
    if(ret<=0){
        if(ret<0) perror("send dati card");
        pthread_mutex_lock(&mutex_lav);
        return -1;
    }


    //ricevo il byte ack_card, se è 1 sposto la card in doing
    char ack;
    ret=recv(sd, &ack, 1, MSG_WAITALL);
    if(ret<=0){
        if(ret<0) perror("recv ack card");
        pthread_mutex_lock(&mutex_lav);
        return -1;
    }
    if(ack){
        
        //cerco l'utente e inserisco l'id della card assegnatagli
        utente_t *u=lav.utenti;
        while(u&&u->porta!=porta) u=u->next_utente;
        if(!u){
            printf("Error: utente %d non in lista\n", porta);
            exit(1);
        }
        u->id_card=lav.colonne[0]->id;


        //creazione thread che si occupa di controllare da quanto la card è in doing e se l'utente è ancora connesso
        pthread_t time_thread;
        if(pthread_create(&time_thread, NULL, ping_user, (void*)(long)u->id_card) != 0){
            perror("creazione thread ping_user");
            exit(1);
        } 


        pthread_mutex_lock(&mutex_lav);

        printf("Card assegnata\n");

        return 1;
    }
    pthread_mutex_lock(&mutex_lav);
    return 0;

}



void* ping_user(void *arg){

    int id=(int)(long)arg;


    pthread_mutex_unlock(&mutex_lav);
    
    
    sleep(TIMER_PING);
    
    
    pthread_mutex_lock(&mutex_lav);
    //controllo se la card è nella colonna doing e se sono passati 90 secondi dall'ultima modifica
    //(potrebbe essere stata rimossa e riassegnata)
    time_t tempo_ping;
    time(&tempo_ping);

    //cerco la card
    card_t *c=lav.colonne[1];
    while(c&&c->id!=id) c=c->next_card;
    if(!c){
        //l'utente ha completato la card
        pthread_mutex_unlock(&mutex_lav);
        pthread_exit(NULL);
    }

    
    if(difftime(tempo_ping, c->ultima_modifica)>=TIMER_PING){//controllo che la card non sia tornata in to do e poi dinuovo in doing

        int sd;
        //cerco l'utente
        utente_t *u=lav.utenti;
        while(u&&u->id_card!=id) u=u->next_utente;
        if(!u){
            printf("Error: utente non presente in lista ha una card\n");
            exit(1);
        }

        sd=u->sd_ltou;
        
        pthread_mutex_unlock(&mutex_lav);

        //invio byte di ping
        char ping[3];
        memset(ping, 0, sizeof(ping));
        sprintf(ping, "%d", PING_USER);

        
        int ret=send(sd, (void*)&ping, sizeof(ping), 0);
        if(ret<=0){
            if(ret<0) perror("send ping user");
            pthread_mutex_lock(&mutex_lav);
            int numero=rimuovi_utente(sd);
            if(numero>0){
                move_card(1, 0, numero, -1);
                check_card_utente_libero(0);
            }
            pthread_mutex_unlock(&mutex_lav);
            pthread_exit(NULL);
        }

        printf("ping %d\n", u->porta);

        //ricevo byte di pong
        char pong;
        ret=recv(sd, &pong, 1, MSG_WAITALL);
        if(ret<=0){
            if(ret<0) perror("recv pong");
            pthread_mutex_lock(&mutex_lav);
            int numero=rimuovi_utente(sd);
            if(numero>0){
                move_card(1, 0, numero, -1);
                check_card_utente_libero(0);
            }
            pthread_mutex_unlock(&mutex_lav);
            pthread_exit(NULL);
        }


        printf("pong %d\n", u->porta);


        time_t tempo_limite;
        time(&tempo_limite);
        if(difftime(tempo_limite, tempo_ping)>=30.0){
            printf("Error: utente non ha fatto pong entro 30 s\n");
            pthread_mutex_lock(&mutex_lav);
            int numero=rimuovi_utente(sd);
            if(numero>0){
                move_card(1, 0, numero, -1);
                check_card_utente_libero(0);
            }
            pthread_mutex_unlock(&mutex_lav);
            pthread_exit(NULL);
        }



    }else pthread_mutex_unlock(&mutex_lav);
    
    return NULL;

}



void* gestore_connessione_utente(void* arg){
    int sd_utol = *(int*)arg;
    free(arg);
    pthread_detach(pthread_self());


    
    char buf;
    int ret;
    
    while(1){
        //ricezione comando
        ret=recv(sd_utol, &buf, sizeof(char), MSG_WAITALL);
        if(ret<=0){
            if(ret<0) perror("receive command"); 
            //errore, rimuovo l'utente dalla lista e se non era registrato chiudo qui il socket
            pthread_mutex_lock(&mutex_lav);
            int numero=rimuovi_utente(sd_utol);
            if(numero>0){
                move_card(1, 0, numero, -1);
                check_card_utente_libero(0);
            }
            if(!numero) close(sd_utol);
            pthread_mutex_unlock(&mutex_lav);
            pthread_exit(NULL);
        }

        //processazione comando
        switch(buf){

            case(HELLO):{
                int in_uso=0;
                char num_porta[6];
                ret=recv(sd_utol, &num_porta, sizeof(num_porta), MSG_WAITALL);
                if(ret<=0){
                    if(ret<0) perror("receive numero porta");
                    pthread_mutex_lock(&mutex_lav);
                    //utente disconnesso durante la registrazione, chiudo il socket
                    printf("l'utente si è disconnesso\n");
                    close(sd_utol);
                    pthread_mutex_unlock(&mutex_lav);
                    pthread_exit(NULL);
                }

                pthread_mutex_lock(&mutex_lav);

                //creo il nuovo utente
                utente_t* u1=lav.utenti, *u2=u1;

                utente_t* newu=malloc(sizeof(utente_t));
                newu->sd_utol=sd_utol;
                newu->sd_ltou = -1;
                newu->porta=(uint16_t) atoi(num_porta);
                newu->id_card=0;
                newu->next_utente=NULL;

                //inserisco l'utente
                if(!lav.utenti){
                    lav.utenti=newu;
                }
                else{
                    if(u1->porta>newu->porta){
                        lav.utenti=newu;
                        lav.utenti->next_utente=u1;
                    }
                    else{
                        while(u1&&u1->porta<newu->porta){
                            u2=u1;
                            u1=u1->next_utente;
                        }   
                        if(!u1){
                            u2->next_utente=newu;
                        }    
                        else{
                            if(u1->porta==newu->porta){
                                in_uso=1;
                            }
                            else{
                                newu->next_utente=u1;
                                u2->next_utente=newu;
                            }
                        }
                    }
                }

                pthread_mutex_unlock(&mutex_lav);

                //invio il byte di disponibilità del numero di porta
                char byte=!in_uso;
                ret=send(sd_utol, (void*)&byte, sizeof(char), 0);
                if(ret<=0){
                    if(ret<0) perror("send disponibilità porta");
                    
                    if(byte){
                        pthread_mutex_lock(&mutex_lav);
                        rimuovi_utente(sd_utol);
                        pthread_mutex_unlock(&mutex_lav);
                    }
                    else{
                        free(newu);
                        close(sd_utol);
                    } 
                    
                    pthread_exit(NULL);
                }
                if(in_uso){
                    free(newu);
                    close(sd_utol);
                    pthread_exit(NULL);
                }

                

                //creazione del socket per richieste lavagna
                int sd_ltou=socket(AF_INET, SOCK_STREAM, 0);
                if(sd_ltou<0){
                    perror("creazione socket richiesta lavagna");
                    pthread_mutex_lock(&mutex_lav);
                    rimuovi_utente(sd_utol);
                    pthread_mutex_unlock(&mutex_lav);
                    pthread_exit(NULL);
                }

                struct sockaddr_in client_notif_addr;
                memset(&client_notif_addr, 0, sizeof(client_notif_addr));
                client_notif_addr.sin_family=AF_INET;
                client_notif_addr.sin_port=htons(newu->porta);
                
                
                inet_pton(AF_INET, "127.0.0.1", &client_notif_addr.sin_addr);


                if(connect(sd_ltou, (struct sockaddr*)&client_notif_addr, sizeof(client_notif_addr))<0){
                    perror("connect per richieste lavagna fallita");
                    pthread_mutex_lock(&mutex_lav);
                    rimuovi_utente(sd_utol);
                    pthread_mutex_unlock(&mutex_lav);
                    pthread_exit(NULL);
                }     


                pthread_mutex_lock(&mutex_lav);

                lav.num_utenti++;

                //inserisco il nuovo socket nella struct utente
                newu->sd_ltou=sd_ltou;

                printf("Utente %d connesso\n", newu->porta);

                //rilevato un utente, se c'è una card in to do la assegno
                check_card_utente_libero(newu->porta);
                    
                pthread_mutex_unlock(&mutex_lav);

                break;
            }

            case(QUIT):{
                pthread_mutex_lock(&mutex_lav);

                int numero=rimuovi_utente(sd_utol);
                if(numero>0){
                    move_card(1, 0, numero, -1);
                    check_card_utente_libero(0);
                }
                pthread_mutex_unlock(&mutex_lav);
                pthread_exit(NULL);

                break;
            }

            case(CREATE_CARD):{

                //ricevo i dati della nuova card
                char dati[LUNG_TESTO+6];
                ret=recv(sd_utol, &dati, sizeof(dati), MSG_WAITALL);
                if(ret<=0){
                    if(ret<0) perror("receive dati card");
                    pthread_mutex_lock(&mutex_lav);
                    //errore, rimuovo l'utente dalla lista
                    int numero=rimuovi_utente(sd_utol);
                    if(numero>0){
                        move_card(1, 0, numero, -1);
                        check_card_utente_libero(0);
                    }
                    pthread_mutex_unlock(&mutex_lav);
                    pthread_exit(NULL);
                }

                //estraggo l'id
                char id_tmp[4];
                strncpy(id_tmp, dati, 4);
                id_tmp[3]='\0';
                int id=(uint32_t) atoi(id_tmp);

                pthread_mutex_lock(&mutex_lav);
                

                //controllo se l'id è libero
                card_t *c;
                int trovato=0;
                for(int i=0; i<3&&!trovato; ++i){
                    c=lav.colonne[i];
                    if(c&&c->id==id) trovato=1;
                    while(c&&c->id!=id&&!trovato){
                        c=c->next_card;
                        if(c&&c->id==id){
                            trovato=1;
                        }
                    } 
                }

                pthread_mutex_unlock(&mutex_lav);
                
                //invio byte di conferma/errore
                char byte=!trovato;
                ret=send(sd_utol, (void*)&byte, sizeof(char), 0);
                if(ret<=0){
                    if(ret<0) perror("send conferma create_card");
                    pthread_mutex_lock(&mutex_lav);
                    int numero=rimuovi_utente(sd_utol);
                    if(numero>0){
                        move_card(1, 0, numero, -1);
                        check_card_utente_libero(0);
                    }
                    pthread_mutex_unlock(&mutex_lav);
                    pthread_exit(NULL);
                }

                if(!trovato){

                    pthread_mutex_lock(&mutex_lav);

                    //estraggo il testo della card
                    char testo[LUNG_TESTO+1];
                    memset(testo, 0, sizeof(testo));
                    strncpy(testo, dati+4, LUNG_TESTO+1);


                    //creo la card
                    card_t* newc=malloc(sizeof(card_t));
                    newc->id=id;
                    newc->colonna=0;
                    newc->utente=0;
                    newc->next_card=NULL;

                    strncpy(newc->testo, testo, LUNG_TESTO+1);
                    newc->testo[LUNG_TESTO] = '\0';

                    time_t tempo;
                    time(&tempo);
                    newc->ultima_modifica=tempo;

                    //inserisco la card nella colonna to do
                    if(!lav.colonne[0]){
                        lav.colonne[0]=newc;
                    }
                    else{
                        c=lav.colonne[0];
                        while(c->next_card) c=c->next_card;
                        c->next_card=newc;
                    } 

                    printf("Card creata\n");

                    show_lavagna();

                    //controllo se c'è un utente a cui assegnarla
                    check_card_utente_libero(0);

                    
                    pthread_mutex_unlock(&mutex_lav);
                }
                
                break;
            }
            case(REQUEST_USER_LIST):{
                pthread_mutex_lock(&mutex_lav);
                send_user_list(sd_utol);
                pthread_mutex_unlock(&mutex_lav);
                break;
            }
            
            case(CARD_DONE):{
                printf("Card completata\n");
                pthread_mutex_lock(&mutex_lav);
                //cerco l'utente, sposto la card in done e gliene assegno una nuova
                utente_t *u=lav.utenti;
                while(u&&u->sd_utol!=sd_utol) u=u->next_utente;
                move_card(1, 2, u->porta, -1);
                check_card_utente_libero(u->porta);
                pthread_mutex_unlock(&mutex_lav);
                break;
            }
            
            default:{
                printf("Comando non supportato\n");
                break;
            } 
        }
    }
    return NULL;
}



void* gestore_input_lavagna(void* arg){
    
    pthread_detach(pthread_self());

    char comando[50];
    memset(comando, 0, sizeof(comando));

    while(1){
        //leggo il comando da tastiera e lo processo
        scanf("%49s", comando);

        if(!strlen(comando)){
            printf("Error: errore lettura comando\n");
            _exit(1);
        }

        if(strcmp(comando, "SHOW_LAVAGNA")==0){
            pthread_mutex_lock(&mutex_lav);
            show_lavagna();
            pthread_mutex_unlock(&mutex_lav);
        }
        else{
            if(strcmp(comando, "QUIT")==0){
                pthread_mutex_lock(&mutex_lav);
                distruggi_lavagna();
                exit(0);
            }
            else    printf("Error: comando non supportato\n");
        }    
        
            

    }
    pthread_exit(NULL);

}



int main() {
    int sd, sd_utol;
    struct sockaddr_in my_addr, cl_addr;
    
    inizializza_lavagna();


    //creazione socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        perror("creazione socket ascolto");
        _exit(1);
    }

    //creazione indirizzo
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORTA_LAVAGNA);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    //bind e listen
    if (bind(sd, (struct sockaddr*)&my_addr, sizeof(my_addr)) < 0) {
        perror("bind");
        _exit(1);
    }
    listen(sd, 10);

    

    printf("Lavagna avviata sulla porta %d...\n", PORTA_LAVAGNA);

    show_lavagna();

    //creo il thread per ricezione comandi 
    pthread_t tinput;
    if (pthread_create(&tinput, NULL, gestore_input_lavagna, NULL) != 0) perror("creazione thread input lavagna");

    //ciclo di accettazione infinito
    while (1) {

        socklen_t len=sizeof(cl_addr);
        sd_utol=accept(sd, (struct sockaddr*)&cl_addr, &len);
        if (sd_utol < 0) {
            perror("accept");
            continue;
        }


        //creazione thread di ascolto
        int* thread_sd_utol = malloc(sizeof(int));
        *thread_sd_utol=sd_utol;
        
        pthread_t tid;
        if (pthread_create(&tid, NULL, gestore_connessione_utente, thread_sd_utol) != 0) perror("creazione thread connessioni utente");
    }

    close(sd);
    return 0;
}

