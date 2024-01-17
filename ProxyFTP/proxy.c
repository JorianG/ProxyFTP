#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/socket.h>
#include  <netdb.h>
#include  <string.h>
#include  <unistd.h>
#include  <stdbool.h>
#include "./simpleSocketAPI.h"


#define SERVADDR "127.0.0.1"            // Définition de l'adresse IP d'écoute
#define SERVPORT "0"                    // Définition du port d'écoute, si 0 port choisi dynamiquement
#define LISTENLEN 1                     // Taille de la file des demandes de connexion
#define MAXBUFFERLEN 2048               // Taille du tampon pour les échanges de données
#define MAXHOSTLEN 64                   // Taille d'un nom de machine
#define MAXPORTLEN 64                   // Taille d'un numéro de port

void traitement(int descSockRDV , int descSockCOM );    // entete de la procedure de traitement de connection

void main(){
    pid_t idProc;                       // Permet de récupérer la valeur du fork()
    int ecode;                          // Code retour des fonctions
    char serverAddr[MAXHOSTLEN];        // Adresse du serveur
    char serverPort[MAXPORTLEN];        // Port du server

    int descSockRDV;                    // Descripteur de socket de rendez-vous
    int descSockCOM;                    // Descripteur de socket de communication

  
    
    struct addrinfo hints;              // Contrôle la fonction getaddrinfo
    struct addrinfo *res;               // Contient le résultat de la fonction getaddrinfo
              
    struct sockaddr_storage infoClient;     // Informations sur la connexion de RDV
    struct sockaddr_storage infoSockRDV;    // Informations sur le client connecté
    
    socklen_t len;                      // Variable utilisée pour stocker les longueurs des structures de socket
				                     
        
    
    // Initialisation de la socket de RDV IPv4/TCP
    descSockRDV = socket(AF_INET, SOCK_STREAM, 0);
    if (descSockRDV == -1) {
         perror("Erreur création socket RDV\n");
         exit(2);
    }

    // Publication de la socket au niveau du système
    // Assignation d'une adresse IP et un numéro de port
    // Mise à zéro de hints
    memset(&hints, 0, sizeof(hints));

    // Initialisation de hints
    hints.ai_flags = AI_PASSIVE;      // mode serveur, nous allons utiliser la fonction bind
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_family = AF_INET;        // seules les adresses IPv4 seront présentées par 
				                      // la fonction getaddrinfo

     // Récupération des informations du serveur
     ecode = getaddrinfo(SERVADDR, SERVPORT, &hints, &res);
     if (ecode) {
         fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(ecode));
         exit(1);
     }
     
     // Publication de la socket
     ecode = bind(descSockRDV, res->ai_addr, res->ai_addrlen);
     if (ecode == -1) {
         perror("Erreur liaison de la socket de RDV");
         exit(3);
     }
     // Nous n'avons plus besoin de cette liste chainée addrinfo
     freeaddrinfo(res);

     // Récuppération du nom de la machine et du numéro de port pour affichage à l'écran
     len=sizeof(struct sockaddr_storage);
     ecode = getsockname(descSockRDV, (struct sockaddr *) &infoSockRDV, &len);
     if (ecode == -1)
     {
         perror("SERVEUR: getsockname");
         exit(4);
     }
     ecode = getnameinfo((struct sockaddr*)&infoSockRDV, sizeof(infoSockRDV), serverAddr,MAXHOSTLEN, 
                         serverPort, MAXPORTLEN, NI_NUMERICHOST | NI_NUMERICSERV);
     if (ecode != 0) {
             fprintf(stderr, "error in getnameinfo: %s\n", gai_strerror(ecode));
             exit(4);
     }
     printf("L'adresse d'ecoute est: %s\n", serverAddr);
     printf("Le port d'ecoute est: %s\n", serverPort);

     // Definition de la taille du tampon contenant les demandes de connexion
     ecode = listen(descSockRDV, LISTENLEN);
     if (ecode == -1) {
         perror("Erreur initialisation buffer d'écoute");
         exit(5);
     }

	len = sizeof(struct sockaddr_storage);
     // Attente connexion du client
     // Lorsque demande de connexion, creation d'une socket de communication avec le client
     

     // boucle infinie qui permet d'attendre les connexion , si il y a une connexion , on créé un nouveau processus
     //qui permet de faire le traitement du client
    while(1){
        descSockCOM = accept(descSockRDV, (struct sockaddr *) &infoClient, &len);
        if (descSockCOM == -1){
            perror("Erreur accept\n");
            exit(6);
        }
        idProc = fork();
        switch(idProc){
            case -1 : perror("erreur fork"); exit(1);
            case 0 : printf("---------------Nouvelle connection---------------\n"); traitement(descSockRDV , descSockCOM );exit(0);
        }
    }
}

//procédure de traitement de connexion du client 
void traitement(int descSockRDV , int descSockCOM ){

    int ecode ;             // Code retour des fonctions
    int sockServeur;        // descripteur de socket de serveur
    int sockClientData;     // descripteur de socket clientData
    int sockServeurData;    // descripteur de socket serveurData

    char buffer[MAXBUFFERLEN];  //récupére la communication entre le client et le serveur
    
    // Echange de données avec le client connecté

    
    //message de connection
    strcpy(buffer, "220 Welcome to the FTP server\n");
    ecode = write(descSockCOM, buffer, strlen(buffer));
    if (ecode == -1 ) {perror("erreur de d'écriture"); exit(1);}

     // Lecture de l'identifiant du  client (anonymous@ftp.fau.de)
    ecode = read(descSockCOM, buffer , MAXBUFFERLEN -1);
    if (ecode == -1 ) {perror ("Problème de lecture \n") ; exit(3);}
    buffer[ecode] = '\0';

    //id = anonymous , nomServer = ftp.fau.de
    char id[50],nomServeur[50]; // récupération du nom de serveur et de l'identifiant utilisateur

    //permet d'atribuer le id et nom serveur a l'utilisateur qui vient de se connecter 
    sscanf(buffer, "%[^@]@%s" , id , nomServeur);
    //Affiche dans la console serveur le login et le serveur utiliser par le client 
    printf("%s ; serveur =%s\n" , id , nomServeur );

    //Connection entre le serveur proxy et le serveur ftp 
    ecode = connect2Server(nomServeur , "21" , &sockServeur);

    ecode=read(sockServeur,buffer , MAXBUFFERLEN-1);
    if (ecode == -1 ) {perror("erreur de lecture"); exit(1);}
    buffer[ecode] = '\0';

    strncat( id , "\r\n" , sizeof(id) -1 );

    //envoie user au serveur 
    ecode = write(sockServeur , id , strlen(id));
    if (ecode == -1 ) {perror("erreur de d'écriture"); exit(1);}

    //reception réponse serveur message : 331
    ecode = read(sockServeur , buffer , MAXBUFFERLEN-1);
    if (ecode == -1 ) {perror("erreur de lecture"); exit(1);}
    buffer[ecode] = '\0';
    printf("\n");
    printf("LOGIN : %s\n" , buffer);

    ecode = write(descSockCOM , buffer , strlen(buffer));
    if (ecode == -1 ) {perror("erreur de d'écriture"); exit(1);}


    // demander le mdp au client  message 230
    ecode = read(descSockCOM , buffer , MAXBUFFERLEN-1);
    if (ecode == -1 ) {perror("erreur de lecture"); exit(1);}
    buffer[ecode] = '\0';
    printf("PASSWORD : %s\n" , buffer);

    ecode = write(sockServeur , buffer , strlen(buffer));
    if (ecode == -1 ) {perror("erreur de d'écriture"); exit(1);}

    ecode = read(sockServeur , buffer , MAXBUFFERLEN-1);
    if (ecode == -1 ) {perror("erreur de lecture"); exit(1);}
    buffer[ecode] = '\0';
    printf("ACCESS : %s\n" , buffer);
    write(descSockCOM , buffer , strlen(buffer));
    

    //lecture de sys du client message : 215
    ecode = read(descSockCOM, buffer , MAXBUFFERLEN-1);
    if (ecode == -1 ) {perror("erreur de lecture"); exit(1);}
    buffer[ecode] = '\0';

    ecode = write(sockServeur, buffer,strlen(buffer));
    if (ecode == -1){perror("erreur d'écriture "); exit(1);}

    ecode=read(sockServeur, buffer , MAXBUFFERLEN-1);
    if (ecode == -1 ) {perror("erreur de lecture"); exit(1);}
    buffer[ecode] = '\0';
    printf("SYSTEM : %s\n" , buffer);

    ecode = write(descSockCOM, buffer,strlen(buffer));
    if (ecode == -1 ) {perror("erreur de d'écriture"); exit(1);}


    // récupére l'adresse et port du client 
    ecode = read(descSockCOM, buffer , MAXBUFFERLEN-1);
    if (ecode == -1 ) {perror("erreur de lecture"); exit(1);}
    buffer[ecode] = '\0';
    printf("CLIENT : %s" ,buffer);
   
    
   //crée le serveur de data client 
    int n1, n2, n3, n4, n5, n6;
    char adresseClient[25];
    char portClient[10];

    sscanf(buffer, "PORT %d,%d,%d,%d,%d,%d" , &n1, &n2, &n3, &n4, &n5, &n6);
    sprintf( adresseClient, "%d.%d.%d.%d" , n1 , n2 , n3 , n4);
    sprintf(portClient , "%d", n5 * 256 + n6);

    ecode = connect2Server(adresseClient ,portClient , &sockClientData );

    //passage en mode passive
    
    ecode = write(sockServeur, "PASV\r\n",strlen("PASV\r\n"));
    if (ecode == -1 ) {perror("erreur de d'écriture"); exit(1);}

    ecode = read(sockServeur, buffer , MAXBUFFERLEN-1);
    if (ecode == -1 ) {perror("erreur de lecture"); exit(1);}
    buffer[ecode] = '\0';
    printf("\n");
    printf("SERVER : %s", buffer);

    //crée le serveur data cote serveur message : 227
    char adresseServeur [25];
    char portServeur [10];
    printf("%s",buffer);
    sscanf(buffer, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)" , &n1, &n2, &n3, &n4, &n5, &n6);
    sprintf( adresseServeur, "%d.%d.%d.%d" , n1 , n2 , n3 , n4);
    sprintf(portServeur , "%d", n5 * 256 + n6);

    ecode = connect2Server(adresseServeur ,portServeur , &sockServeurData );
   
    ecode = write(descSockCOM , "220\n" ,strlen("220\n"));
    if (ecode == -1 ) {perror("erreur de d'écriture"); exit(1);}

    //Lecture de la commande ls du client  message : LIST
    ecode = read(descSockCOM, buffer , MAXBUFFERLEN-1);
    if (ecode == -1 ) {perror("erreur de lecture"); exit(1);}
    buffer[ecode] = '\0';
    printf("\n");
    printf("CLIENT COMMAND : %s\n" ,buffer);

    ecode = write(sockServeur, "LIST\r\n", strlen("LIST\r\n"));
    if (ecode == -1 ) {perror("erreur de d'écriture"); exit(1);}


    // 150 opening ascii 
    ecode = read(sockServeur, buffer , MAXBUFFERLEN-1);
    if (ecode == -1 ) {perror("erreur de lecture"); exit(1);}
    buffer[ecode] = '\0';
    printf("SERVER : %s\n" ,buffer);


    ecode = write(descSockCOM , buffer ,strlen(buffer));
    if (ecode == -1 ) {perror("erreur de d'écriture"); exit(1);}
    bzero(buffer , MAXBUFFERLEN -1 );

    // permet d'afficher tous le résultat de la commande ls 

    printf("----------------------------LS------------------------------\n");
        do {                                                                    
            ecode = read(sockServeurData, buffer , MAXBUFFERLEN-1);
            if (ecode == -1 ) {perror("erreur de de lecture"); exit(1);}               
            if (ecode <= 0 ) { break;}                                          
            printf("%s",buffer);
            ecode = write(sockClientData , buffer , strlen(buffer));
            if (ecode == -1 ) {perror("erreur de d'écriture"); exit(1);}
            bzero(buffer , MAXBUFFERLEN -1 );
            
        } while (ecode > 0 );

    close(sockServeurData);
    close(sockClientData);

    
    printf("-------------------------------------------------------------\n");

    bzero(buffer , MAXBUFFERLEN -1 );

    //récupération du nombre de fichier tranféré message : 226 
    ecode = read(sockServeur, buffer , MAXBUFFERLEN-1);
    if (ecode == -1 ) {perror("erreur de lecture"); exit(1);}
    buffer[ecode] = '\0';
    printf("LS COMMAND : %s\n" , buffer);

    ecode = write(descSockCOM , buffer , MAXBUFFERLEN-1);
    if (ecode == -1 ) {perror("erreur de d'écriture"); exit(1);}

    // fermeture du client 

    ecode = read(descSockCOM, buffer , MAXBUFFERLEN-1);
    if (ecode == -1 ) {perror("erreur de lecture"); exit(1);}
    buffer[ecode] = '\0';
    printf("CLIENT : %s\n" , buffer);

    ecode = write(sockServeur , buffer , MAXBUFFERLEN-1);
    if (ecode == -1 ) {perror("erreur de d'écriture"); exit(1);}

    // envoie du message de fermeture : 221
    ecode = read( sockServeur, buffer , MAXBUFFERLEN-1);
    if (ecode == -1 ) {perror("erreur de lecture"); exit(1);}
    printf("221 connexion terminée !\n");
    
    ecode = write(descSockCOM , buffer , MAXBUFFERLEN-1);
    

    //Fermeture de la connexion
    close(descSockCOM);
    close(descSockRDV);
}