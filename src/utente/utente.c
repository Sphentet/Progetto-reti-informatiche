#include "../../include/utente/utente.h"

char *lista_utenti=NULL;
pthread_mutex_t mutex_u;
int sock=-1;
uint16_t porta=0;



int hello(int sd, int porta){
    //invio il comando
    char buf=HELLO;
    int ret=send(sd, (void*)&buf, sizeof(buf), 0);
    if(ret<=0){
        if(ret==0) printf("lavagna disconnessa\n");
        else perror("send hello");
        exit(1);
    }

    //invio il numero di porta
    char num_porta[6];
    memset(num_porta, 0, sizeof(num_porta));
    sprintf(num_porta, "%d", porta);
    ret=send(sd, (void*)&num_porta, sizeof(num_porta), 0);
    if(ret<=0){
        if(ret==0) printf("lavagna disconnessa\n");
        else perror("send numero porta");
        exit(1);
    }

    //ricevo il byte di disponibilità del numero di porta
    char libero;
    ret=recv(sd, &libero, 1, MSG_WAITALL);
    if(ret<=0){
        if(ret==0) printf("lavagna disconnessa\n");
        else perror("recv disponibilità numero porta");
        exit(1);
    }
    if(!libero){
        printf("Error: porta già in uso\n");
        return -1;
    }

    return 1;
}



int quit(int sd){
    char buf=QUIT;
    int ret=send(sd, (void*)&buf, sizeof(buf), 0);
    if(ret<=0){
        if(ret==0){
            printf("lavagna disconnessa\n");
            exit(1);
        }
        perror("send quit");
        return -1;
    }

    return 1;
}



int create_card(int sd){

    char buf=CREATE_CARD;
    int ret=send(sd, (void*)&buf, sizeof(buf), 0);
    if(ret<=0){
        if(ret==0){
            printf("lavagna disconnessa\n");
            exit(1);
        }
        perror("send create_card");
        return -1;
    }


    //lettura parametri
    char id[4];
    memset(id, 0, sizeof(id));
    printf("Inserisci un ID: \n");
    scanf("%2s", id);
    while(getchar() != '\n');

    char testo[LUNG_TESTO+1];
    memset(testo, 0, sizeof(testo));
    printf("Inserisci il testo della card: \n");
    if(fgets(testo, LUNG_TESTO + 1, stdin)==NULL){
        printf("Error: lettura testo\n");
        return -1;
    }



    if(strchr(testo, '\n')!=NULL){
        testo[strcspn(testo, "\n")]='\0';
    }else{
        //se non c'è \n la stringa era troppo lunga, svuoto il buffer dai caratteri in eccesso
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }

    


    //formattazione e invio dati
    char dati[LUNG_TESTO+6];
    memset(dati, 0, sizeof(dati));
    dati[0]='\0';
    int n=sizeof(id);
    strncpy(dati, id, n);

    n=sizeof(testo);
    strncpy(dati+4, testo, LUNG_TESTO);

    ret=send(sd, (void*)&dati, sizeof(dati), 0);
    if(ret<=0){
        if(ret==0){
            printf("lavagna disconnessa");
            exit(1);
        }
        perror("send dati card");
        return -1;
    }


    //aspetto conferma che l'id sia libero

    char libero;
    ret=recv(sd, &libero, 1, MSG_WAITALL);
    if(ret<=0){
        if(ret==0){
            printf("lavagna disconnessa\n");
            exit(1);
        }
        perror("recv conferma creazione card");
        _exit(1);
    } 

        
    if(!libero){
        printf("Error: ID già in uso\n");
        return -1;
    }
    
    printf("card creata\n");
    return 1;

}



char* recv_user_list(int sd){
    //ricevo il numero degli utenti
    char num_utenti[6];
    int ret=recv(sd, &num_utenti, sizeof(num_utenti), MSG_WAITALL);
    if(ret<=0){
        if(ret<0) perror("receive lunghezza lista utenti");
        else printf("Error: lavagna disconnessa\n");
        _exit(1);
    }

    //calcolo la lunghezza del buffer da leggere
    int num=(int) atoi(num_utenti);
    int dim=((num-1)*5)+1;
    char *buf=malloc(dim*sizeof(char));

    //ricevo la lista delle porte
    ret=recv(sd, buf, dim, MSG_WAITALL);
    if(ret<=0){
        if(ret<0) perror("receive lista utenti");
        else printf("Error: lavagna disconnessa\n");
        _exit(1);
    }

    return buf;
}



char* request_user_list(int sd){
    //invio il comando
    char buf=REQUEST_USER_LIST;
    int ret=send(sd, (void*)&buf, sizeof(buf), 0);
    if(ret<=0){
        if(ret==0) printf("lavagna disconnessa");
        else perror("send request user list");
        exit(1);
    }

    //ricevo i dati tramite la funzione ausiliaria
    return recv_user_list(sd);

}



void ack_card(int sd){
    //invio il byte di ack
    char byte=1;
    int ret=send(sd, (void*)&byte, sizeof(char), 0);
    if(ret<=0){
        if(ret==0) printf("lavagna disconnessa\n");
        else perror("send ack");
        exit(1);
    }

}



void pong_lavagna(int sd){
    //invio il byte di pong
    char pong=1;
    int ret=send(sd, (void*)&pong, sizeof(pong), 0);
    if(ret<=0){
        if(ret==0) printf("lavagna disconnessa\n");
        else perror("send hello");
        exit(1);
    }

}



void* review_card(void *arg){

    sleep(120);

    int sd=*(int*)arg;
    
    int ptr=0;
    int porta=0;
    int tutti_approvati=1;
    int caratteri_letti=0;

    //richiedo la lista utenti

    int peer=0;
    char *utenti=NULL;
    while(!peer){
        printf("Richiedo la lista utenti\n");
        utenti=request_user_list(sd);
        if(utenti&&strlen(utenti)>=4){//se ci sono altri utenti proseguo, sennò aspetto 5 secondi e richiedo finchè non si collega qualcuno
            peer=1;
        } 
        else{
            printf("Non ci sono altri utenti\n");
            free(utenti);
            sleep(3);
        }
    }
    

    
    //aggiorno la lista globale

    pthread_mutex_lock(&mutex_u);
    lista_utenti=utenti;
    pthread_mutex_unlock(&mutex_u);

    
    //invio le richieste di review
    while(utenti[ptr]!='\0'&&tutti_approvati&&strlen(&utenti[ptr])>=4){
        if (sscanf(&utenti[ptr], "%d%n", &porta, &caratteri_letti)!=1||porta<=0) {
            break; //se non riesce a estrarre un numero valido o il numero di porta è sbagliato
        }
        ptr+=caratteri_letti;


        //apro il socket temporaneo verso il peer
        int sd_peer=socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in peer_review_addr;
        memset(&peer_review_addr, 0, sizeof(peer_review_addr));
        peer_review_addr.sin_family=AF_INET;
        peer_review_addr.sin_port=htons(porta);
        inet_pton(AF_INET, "127.0.0.1", &peer_review_addr.sin_addr);

        //connect per la review
        printf("Richiedo review al peer %d...\n", porta);
        if (connect(sd_peer, (struct sockaddr*)&peer_review_addr, sizeof(peer_review_addr)) < 0) {
            perror("Connect al peer fallita");
            tutti_approvati=0;
            close(sd_peer);
            break;
        }


        //ricevo il byte di review
        char voto=1;
        int ret=recv(sd_peer, &voto, 1, MSG_WAITALL);
        if (ret<= 0||voto!=1) {
            printf("[ERRORE] Review rifiutata o peer disconnesso\n");
            tutti_approvati=0;
        }
        close(sd_peer);

    }
    

    //se non ci sono stati errori e ho letto caratteri
    if(tutti_approvati&&ptr){
        printf("Card completata\n");
        //invio il comando di card done
        char buf=CARD_DONE;
        int ret=send(sd, (void*)&buf, sizeof(buf), 0);
        if(ret<=0){
            if(ret==0){
                printf("lavagna disconnessa\n");
                exit(1);
            }
            perror("send card_done");
            exit(1);
        }
    }
    free(utenti);
    return NULL;
}



void* gestore_input(void *arg){
    int sd=*(int*)arg;
    free(arg);

    pthread_detach(pthread_self());

    char comando[50];
    memset(comando, 0, sizeof(comando));

    while(1){
        //leggo da tastiera il comando
        scanf("%49s", comando);

        if(!strlen(comando)){
            printf("Error: errore lettura comando\n");
            continue;
        }

        if(strcmp(comando, "QUIT")==0){
            quit(sd); 
            pthread_exit(NULL);;
        }
        else{
            if(strcmp(comando, "CREATE_CARD")==0){
                create_card(sd);
            }
            else    printf("Error: comando non supportato\n");
        }
            

    }
    pthread_exit(NULL);

}



void* gestore_richieste(void *arg){
    
    int sd=*(int*)arg;
    free(arg); //libero la memoria allocata nel main

    pthread_detach(pthread_self());

    char buf[3];
    memset(buf, 0, sizeof(buf));
    int ret;


    while(1){
        //recv del comando
        ret=recv(sd, buf, sizeof(buf), MSG_WAITALL);
        if(ret<=0){
            if(ret<0){
                perror("ricezione richiesta lavagna");
                _exit(1);
            } 
            else{
                printf("Disconnessione completata\n");
                _exit(0);
            } 
            
        }
        
        int comando=(int) atoi(buf);
        switch (comando)
        {
            case HANDLE_CARD:{

                //ricevo i dati della card
                char dati[LUNG_TESTO+6];
                ret=recv(sd, dati, sizeof(dati), MSG_WAITALL);
                if(ret<=0){
                    if(ret<0) perror("receive dati card");
                    else printf("Error: lavagna disconnessa\n");
                    _exit(1);
                }


                //invio byte di ack
                ack_card(sd);


                //ricevo la lista delle porte utente
                char *utenti=recv_user_list(sd);

                //aggiorno la lista globale
                pthread_mutex_lock(&mutex_u);
                lista_utenti=utenti;
                pthread_mutex_unlock(&mutex_u);


                //estraggo i dati e converto l'id della card
                char id_tmp[4];
                strncpy(id_tmp, dati, 4);
                id_tmp[3]='\0';
                int id=(uint32_t) atoi(id_tmp);

                char testo[LUNG_TESTO+1];
                strncpy(testo, dati+4, LUNG_TESTO+1);
                

                //stampo la card
                int l=1;
                printf("-----------------------------------\n");
                printf("|                                 |\n");

                //stampa id card
                printf("|             Task %d", id);
                sprintf(id_tmp, "%d", id);
                int len_id=strlen(id_tmp);
                while(SPAZI_DX>=len_id){
                    printf(" ");
                    len_id++;
                }
                printf("|\n");

                
                //stampa testo card
                while(l){
                    printf("|");
                    int len=strlen(testo);
                    int start=LARG_COL*(l-1);
                    if(len>LARG_COL*l){
                        for(int i=start; i<LARG_COL*l; ++i){
                            printf("%c", testo[i]);
                        }
                        l++;
                    }
                    else{
                        int mezzo=(LARG_COL*l-len)/2;
                        for(int i=0; i<mezzo; ++i){
                            printf(" ");
                        }

                        for(int i=start; i<len; ++i){
                            printf("%c", testo[i]);
                        }

                        if((LARG_COL*l-len)%2!=0) mezzo++;
                        for(int i=0; i<mezzo; ++i){
                            printf(" ");
                        }
                        l=0;
                    }
                    printf("|\n");
                }

                printf("|                                 |\n");
                printf("-----------------------------------\n");
                

                //creo il thread dedicato alla richiesta delle review
                int *arg_review=malloc(sizeof(int));
                *arg_review=sock;

                pthread_t review_thread;
                if(pthread_create(&review_thread, NULL, review_card, arg_review) != 0){
                    perror("creazione thread review_card");
                    exit(1);
                }

                break;
            }
            
            case(PING_USER):{
                pong_lavagna(sd);
                break;
            }
            
            default:{
                break;
            }
                
        }




    }
    close(sd);
    pthread_exit(NULL);
}



int main(int argc, char** argv){
    //controlli su argomenti
    if(argc!=2){
        printf("Error: indicare il numero di porta\n");
        exit(1);
    }

    porta=(uint16_t) atoi(argv[1]);

    if(porta<=5678){
        printf("Error: numero di porta minore di 5679\n");
        exit(1);
    }


    //socket per comandi utente
    int sd, ret;
    struct sockaddr_in sv_addr;

    sd=socket(AF_INET, SOCK_STREAM, 0);
    if(sd<0){
        perror("creazione socket per il server");
        _exit(1);
    }

    memset(&sv_addr, 0, sizeof(sv_addr));
    sv_addr.sin_family = AF_INET ;
    sv_addr.sin_port = htons(PORTA_LAVAGNA);

    inet_pton(AF_INET, INDIRIZZO_LAVAGNA, &sv_addr.sin_addr);
    ret = connect(sd, (struct sockaddr*)&sv_addr, sizeof(sv_addr));
    if(ret<0){
        perror("connect");
        _exit(1);
    }


    

    //socket per richieste dalla lavagna
    int listen_sd=socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in listen_addr;
    
    if(listen_sd < 0){ 
        perror("socket richieste lavagna"); 
        _exit(1); 
    }

    memset(&listen_addr, 0, sizeof(listen_addr));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = htons(porta);
    listen_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(listen_sd, (struct sockaddr*)&listen_addr, sizeof(listen_addr)) < 0){
        perror("errore bind socket di ascolto"); 
        _exit(1);
    }
    if(listen(listen_sd, 10) < 0){
        perror("errore listen");
        _exit(1);
    }


    //registrazione
    if(hello(sd, porta)<0) _exit(1);



    //accettazione connessione lavagna
    struct sockaddr_in lavagna_addr;
    socklen_t len=sizeof(lavagna_addr);
    
    int sd_ltou=accept(listen_sd, (struct sockaddr*)&lavagna_addr, &len);
    if(sd_ltou<0){
        perror("accept richieste lavagna");
        _exit(1);
    }




    //thread per comandi
    int *arg_comandi = malloc(sizeof(int));
    *arg_comandi = sd;
    sock=sd;
    pthread_t input;
    pthread_create(&input, NULL, gestore_input, arg_comandi);


    //thread per richieste
    int *arg_richieste = malloc(sizeof(int));
    *arg_richieste = sd_ltou;
    pthread_t richieste_t;
    pthread_create(&richieste_t, NULL, gestore_richieste, arg_richieste);





    

    while(1){
        //accept continua di richieste di card review
        int sd_ptop=accept(listen_sd, (struct sockaddr*)&lavagna_addr, &len);
        if(sd_ptop<0){
            perror("accept richieste peer");
            _exit(1);
        }


        //invio il byte di review positiva, poi ritorno in attesa di connessione
        char buf=1;
        int ret=send(sd_ptop, (void*)&buf, sizeof(buf), 0);
        if(ret<=0){
            if(ret==0){
                printf("peer disconnesso\n");
                continue;
            }
            perror("send byte di review");
            return -1;
        }

    }
    




    pthread_exit(NULL);
    return 0;
}






