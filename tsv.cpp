#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <iostream>
#include <arpa/inet.h>

using namespace std;

#define PORT 2020
#define address "127.0.0.11"

extern int errno;

typedef struct thData{
	int idThread; //id-ul thread-ului
	int cl1; //socket descriptor client1
	int cl2; //socket descriptor client2
}thData;

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void raspunde(void *);

int main ()
{
    pthread_t th[100];

    cout << "inceput server " << endl;

    struct sockaddr_in client;
    struct sockaddr_in client2;
    struct sockaddr_in server;

    bzero (&server, sizeof (server));
    bzero (&client, sizeof (client));
    bzero (&client2, sizeof (client2));

    int sd;		//descriptorul de socket 
    if ((sd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
        {
        perror ("[server]Eroare la socket().\n");
        return errno;
        }

    int on=1;
    setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    
    server.sin_family = AF_INET;
    server.sin_port = htons (PORT);
    //server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_addr.s_addr = inet_addr(address);
    
    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
        {
        perror ("[server]Eroare la bind().\n");
        return errno;
        }

    if (listen (sd, 5) == -1)
        {
        perror ("[server]Eroare la listen().\n");
        return errno;
        }
    int i = 0;
    while (1) 
    {    
        printf ("[server]Asteptam la adresa %s si la portul %d...\n", address, PORT);
        fflush (stdout);
        int cllen = sizeof (client);
        int clsd = accept (sd, (struct sockaddr *) &client, (socklen_t*) &cllen);
        if (clsd < 0)
        {
            perror ("[server]Eroare la accept().\n");
            continue;
        }

        int cllen2 = sizeof (client2);
        int clsd2 = accept (sd, (struct sockaddr *) &client2, (socklen_t*) &cllen2);
        if (clsd2 < 0)
        {
            perror ("[server]Eroare la accept2().\n");
            continue;
        }

        thData * td; //parametru functia executata de thread     
    
        td=(struct thData*)malloc(sizeof(struct thData));
        td->idThread=i++;
        td->cl1=clsd;
        td->cl2=clsd2;

        pthread_create(&th[i], NULL, &treat, td);
    }
}

static void *treat(void * arg)
{		
		struct thData tdL; 
		tdL= *((struct thData*)arg);

		printf ("[thread]- %d - Joc nou creat\n", tdL.idThread);
		fflush (stdout);

		pthread_detach(pthread_self());

		raspunde((struct thData*)arg);

		/* am terminat cu acest client, inchidem conexiunea */
		close ((intptr_t)arg);
		return(NULL);	
  		
}

void afisare(char tabla[6][7])
{
    for (int i=0; i<6; i++)
    {
        printf("|===|===|===|===|===|===|===|\n");
        for (int j=0; j<7; j++)
        {
            printf("| ");
            printf("%c ", tabla [i] [j] );

        }
        printf("|\n" );
    }
    printf("|===|===|===|===|===|===|===|\n");
    printf("  1   2   3   4   5   6   7 \n");
}


void sendMatrix(int cl, char tabla[6][7]) {
    char x;
    for (int i=0; i < 6; i++)
        for (int j = 0; j < 7; j++) {
            x = tabla[i][j];
            send (cl, &x, 1, 0);
        }
}

void getMatrix(int cl, char tabla[6][7]) {
    char x;
    for (int i=0; i < 6; i++)
        for (int j = 0; j < 7; j++) {
            recv(cl, &x, 1, 0);
            tabla[i][j] = x;
        }
}

void sendLib(int cl, int lib[8]) {
    char x;
    for (int i = 0; i < 8; i++) {
        x = char(lib[i]);
        send (cl, &x, 1, 0);
    }
}

void getLib(int cl, int lib[8]) {
    char x;
    for (int i = 0; i < 8; i++) {
        recv(cl, &x, 1, 0);
        lib[i] = int(x);
    }
}

void raspunde(void *arg)
{
	struct thData tdL; 
	tdL= *((struct thData*)arg);
    
    char tabla[6][7];
    int i, j;
    char simbol;
    int scor_x = 0, scor_0 = 0;
    char char_scor_x, char_scor_0;
    char confirmare1[2], confirmare2[2];

    simbol = 'X';
    send (tdL.cl1, &simbol, 1, 0);

    simbol = '0';
    send (tdL.cl2, &simbol, 1, 0);

    int won = 0;
    int run = 1;

    while (run) {
        int lib[8] = {6, 6, 6, 6, 6, 6, 6, 6}; //lib = casute libere pe coloana
        for (i=0; i<6; i++)
            for (j=0; j<7; j++)
                tabla [i] [j] = ' ';
        
        char winner = ' ';
        while (won == 0) {
            sendMatrix(tdL.cl1, tabla);
            sendLib(tdL.cl1, lib);
            getMatrix(tdL.cl1, tabla);
            getLib(tdL.cl1, lib);
            
            send (tdL.cl1, &winner, 1, 0 );
            recv (tdL.cl1, &winner, 1, 0 );

            if (winner == 'X') {
                scor_x ++;
            }

            char_scor_x = char(scor_x);
            char_scor_0 = char(scor_0);

            send (tdL.cl1, &char_scor_x, 1, 0 );
            send (tdL.cl1, &char_scor_0, 1, 0 );
            

            sendMatrix(tdL.cl2, tabla);
            sendLib(tdL.cl2, lib);
            getMatrix(tdL.cl2, tabla);
            getLib(tdL.cl2, lib);

            send (tdL.cl2, &winner, 1, 0 );
            recv (tdL.cl2, &winner, 1, 0 );

            if (winner == '0') {
                scor_0 ++;
            }

            char_scor_x = char(scor_x);
            char_scor_0 = char(scor_0);

            send (tdL.cl2, &char_scor_x, 1, 0 );
            send (tdL.cl2, &char_scor_0, 1, 0 );


            if (winner == '0' || winner == 'X') {
                if (winner == '0') {
                    sendMatrix(tdL.cl1, tabla);
                    sendLib(tdL.cl1, lib);
                    getMatrix(tdL.cl1, tabla);
                    getLib(tdL.cl1, lib);

                    send (tdL.cl1, &winner, 1, 0 );
                    recv (tdL.cl1, &winner, 1, 0 );

                    send (tdL.cl1, &char_scor_x, 1, 0 );
                    send (tdL.cl1, &char_scor_0, 1, 0 );
                }
                
                won = 1;
            }
        }
        recv (tdL.cl1, &confirmare1, 1, 0 );
        recv (tdL.cl2, &confirmare2, 1, 0 );

        send (tdL.cl1, &confirmare2, 1, 0 );
        send (tdL.cl2, &confirmare1, 1, 0 );
        if ((strcmp(confirmare1, "y") == 0 || strcmp(confirmare1, "Y") == 0) && (strcmp(confirmare2, "y") == 0 || strcmp(confirmare2, "Y") == 0)) {
            won = 0;
        } else {
            run = 0;
        }
    }
}