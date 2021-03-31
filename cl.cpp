#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;

extern int errno;

char tabla[6][7];
int i, j, win;
int lib[8] = {5, 5, 5, 5, 5, 5, 5, 5}; //lib = casute libere pe coloana
char simbol, a_d = 'd' ;
int poz = 0;

void head1()
{
    printf ("\n  ⟱\n");
}
void head2()
{
    printf ("\n      ⟱\n");
}
void head3()
{
    printf ("\n          ⟱\n");
}
void head4()
{
    printf ("\n              ⟱\n");
}
void head5()
{
    printf ("\n                  ⟱\n");
}
void head6()
{
    printf ("\n                      ⟱\n");
}
void head7()
{
    printf ("\n                          ⟱ \n");
}

void afisare()
{
    for (i=0; i<6; i++)
    {
        printf("|===|===|===|===|===|===|===|\n");
        for (j=0; j<7; j++)
        {
            printf("| ");
            printf("%c ", tabla [i] [j] );

        }
        printf("|\n" );
    }
    printf("|===|===|===|===|===|===|===|\n");
    printf("  1   2   3   4   5   6   7 \n");
}

void joc(int sd)
{
    switch (poz) //afisare initiala a indicatorului (sageata in jos)
        {
        case 1:
            head1();
            break;
        case 2:
            head2();
            break;
        case 3:
            head3();
            break;
        case 4:
            head4();
            break;
        case 5:
            head5();
            break;
        case 6:
            head6();
            break;
        case 7:
            head7();
            break;
        default:
            head1();
            break;
        }
    afisare();
    if ((a_d == 'a' || a_d == 'A') && poz > 1)
        poz --;
    if ((a_d == 'd' || a_d == 'D') && poz < 7)
        poz ++;
    while (1) //schimbam pozitia curenta
    {
        if (lib[poz] == 0)
            cout << "Coloana plina" << endl;

        cout << "A = move left, D = move right, S = drop disc: ";
        
        cin >> a_d;
        if ((a_d == 'a' || a_d == 'A') && poz > 1)
            poz --;
        if ((a_d == 'd' || a_d == 'D') && poz < 7)
            poz ++;
        if (a_d == 's' || a_d == 'S')
            if (lib[poz] > 0)
                break;

        system ("clear");
        switch (poz) //afisare indicator
        {
        case 1:
            head1();
            break;
        case 2:
            head2();
            break;
        case 3:
            head3();
            break;
        case 4:
            head4();
            break;
        case 5:
            head5();
            break;
        case 6:
            head6();
            break;
        case 7:
            head7();
            break;
        default:
            break;
        }
        afisare(); //afisare tabla
    }
    switch (poz) //completare matrice pe pozitia selectata
    {
    case 1:
        if (lib[1] >= 0 && tabla [lib[1]-1][poz-1] == ' ') {
            tabla [lib[1]-1][poz-1] = simbol;
            lib[1]--;
            a_d = 'w';
        }

        head1();
        afisare();
        break;
    case 2:
        if (lib[2] >= 0 && tabla [lib[2]-1][poz-1] == ' ') {
            tabla [lib[2]-1][poz-1] = simbol;
            lib[2]--;
            a_d = 'w';
        }

        head2();
        afisare();
        break;
    case 3:
        if (lib[3] >= 0 && tabla [lib[3]-1][poz-1] == ' ') {
            tabla [lib[3]-1][poz-1] = simbol;
            lib[3]--;
            a_d = 'w';
        }
        
        head3();
        afisare();
        break;
    case 4:
        if (lib[4] >= 0 && tabla [lib[4]-1][poz-1] == ' ') {
            tabla [lib[4]-1][poz-1] = simbol;
            lib[4]--;
            a_d = 'w';
        }

        head4();
        afisare();
        break;
    case 5:
        if (lib[5] >= 0 && tabla [lib[5]-1][poz-1] == ' ') {
            tabla [lib[5]-1][poz-1] = simbol;
            lib[5]--;
            a_d = 'w';
        }

        head5();
        afisare();
        break;
    case 6:
        if (lib[6] >= 0 && tabla [lib[6]-1][poz-1] == ' ') {
            tabla [lib[6]-1][poz-1] = simbol;
            lib[6]--;
            a_d = 'w';
        }

        head6();
        afisare();
        break;
    case 7:
        if (lib[7] >= 0 && tabla [lib[7]-1][poz-1] == ' ') {
            tabla [lib[7]-1][poz-1] = simbol;
            lib[7]--;
            a_d = 'w';
        }

        head7();
        afisare();
        break;
    default:
        break;
    }
}

void ifwin()
{
    // diagonala
    for (int linie = 0; linie < 4; linie++)
        for (int coloana = 6; coloana > 2; coloana--)
            if (tabla[linie][coloana] == simbol)
                if (tabla[linie + 1][coloana - 1] == simbol)
                    if (tabla[linie + 2][coloana - 2] == simbol)
                        if (tabla[linie + 3][coloana - 3] == simbol)
                            win = 1;

    // diagonala
    for (int linie = 0; linie < 4; linie++)
        for (int coloana = 6; coloana > 2; coloana--)
            if (tabla[linie][coloana - 3] == simbol)
                if (tabla[linie + 1][coloana - 2] == simbol)
                    if (tabla[linie + 2][coloana - 1] == simbol)
                        if (tabla[linie + 3][coloana] == simbol)
                            win = 1;

    // orizontala
    for (int linie = 0; linie < 7; linie++)
        for (int coloana = 6; coloana > 2; coloana--)
            if (tabla[linie][coloana] == simbol)
                if (tabla[linie][coloana - 1] == simbol)
                    if (tabla[linie][coloana - 2] == simbol)
                        if (tabla[linie][coloana - 3] == simbol)
                            win = 1;

    // verticala
    for (int coloana = 0; coloana < 7; coloana++)
        for (int linie = 6; linie > 2; linie--)
            if (tabla[linie][coloana] == simbol)
                if (tabla[linie - 1][coloana] == simbol)
                    if (tabla[linie - 2][coloana] == simbol)
                        if (tabla[linie - 3][coloana] == simbol)
                            win = 1;
}

void getMatrix(int sd) {
    char x;
    for ( i=0; i < 6; i++)
        for (j = 0; j < 7; j++) {
            recv(sd, &x, 1, 0);
            tabla[i][j] = x;
        }
}

void sendMatrix(int sd) {
    char x;
    for ( i=0; i < 6; i++)
        for (j = 0; j < 7; j++) {
            x = tabla[i][j];
            send (sd, &x, 1, 0);
        }
}

void sendLib(int sd, int lib[8]) {
    char x;
    for (i = 0; i < 8; i++) {
        x = char(lib[i]);
        send (sd, &x, 1, 0);
    }
}

void getLib(int sd, int lib[8]) {
    char x;
    for (i = 0; i < 8; i++) {
        recv(sd, &x, 1, 0);
        lib[i] = int(x);
    }
}


int main (int argc, char *argv[])
{
    if (argc != 3)
    {
        printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }
    struct sockaddr_in server;

    int sd;
    if ((sd = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        perror ("Eroare la socket().\n");
        return errno;
    }

    int opt = 1;
    setsockopt (sd, SOL_SOCKET, SO_REUSEADDR, (void *) &opt, sizeof (opt));

    server.sin_family = AF_INET;
    server.sin_port = htons (atoi (argv[2]));
    server.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
        perror ("[client]Eroare la connect().\n");
        return errno;
    }
    int run = 1;

    recv (sd, &simbol, 1, 0);

    while (run) {
        char winner = ' ';
        char scor_x, scor_0;

        while (win == 0){

            getMatrix(sd); // primeste matricea de la server
            getLib(sd, lib);

            joc(sd); // apel catre functia care permite player-ului sa puna o piesa in tabla
            afisare();
            
            sendMatrix(sd); // trimite matricea cu piesa pusa de player
            sendLib(sd, lib);
            ifwin ();

            recv (sd, &winner, 1, 0);
            if (winner == '0' || winner == 'X') {
                win = 1;
            }
            
            if (win == 1 && winner != '0' && winner != 'X') {
                winner = simbol;
            }
            send (sd, &winner, 1, 0);

            recv (sd, &scor_x, 1, 0);
            recv (sd, &scor_0, 1, 0);

            system("clear");
        }
        if(win == 1) {
            cout << "a castigat " << winner << endl;
            cout << "X:  " << int(scor_x) << "-" << int(scor_0) << "  :0" << endl;
        }
        cout << "Play again ?    Y/N: ";
        char confirmare[2];
        char confirmare2[2];

        
        cin >> confirmare;

        while(strcmp(confirmare, "N") != 0 && strcmp(confirmare, "n") != 0 && strcmp(confirmare, "Y") != 0 && strcmp(confirmare, "y") != 0) {
            cout << "Introduceti Y sau N:";
            cin >> confirmare;
        }
        
        send (sd, &confirmare, 1, 0);
        recv (sd, &confirmare2, 1, 0);
        
        
        if (strcmp(confirmare, "N") == 0 || strcmp(confirmare, "n") == 0 || strcmp(confirmare2, "N") == 0 || strcmp(confirmare2, "n") == 0)
            run = 0;cout<<endl;
        if (strcmp(confirmare, "Y") == 0 || strcmp(confirmare, "y") == 0)
            win = 0; poz = 1;

    }
    close (sd);
}
