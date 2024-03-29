/* Programme serveur : affiche les messages qui proviennent des clients */
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include<unistd.h>
#include <string.h>
void verifier(char *ch2, char *ch1, int *found) {
    *found = 1; 
    for (int i = 0; i < strlen(ch1); i++) {
        int char_found = 0;
        for (int j = 0; j < strlen(ch2); j++) {
            if (ch1[i] == ch2[j]) {
                char_found = 1;
                break;
            }
        }
        if (!char_found) { 
            *found = 0;
            return;
        }
        *found = 1; 
    }
}
int main()
{
int sock_ecoute, sock_dial ; /* sock_ecoute est utilisée par le processus père
pour l'écoute des demandes de connexions, sock_dial est utilisée par un fils pour
communiquer avec un client */
char tampon[256], ch[100] ; /* pour stocker un message reçu */
struct sockaddr_in adr_serv ; /* adresse socket serveur */
int lg_adr_serv; /* longueur de l'adresse de la socket serveur */
char nom_serv[30]; /* nom du serveur */
struct hostent *num_ip_serv; /* pointeur sur une structure donnant le nom,
l'adresse IP, etc, du serveur */
struct sockaddr_in adr_client ; /* adresse socket distante */
int lg_adr_client; /* longueur de l'adresse de la socket client */
/* création de la socket d'écoute */
sock_ecoute = socket(AF_INET, SOCK_STREAM,0) ;
/* la socket est attachée à une adresse IP et un numéro de port
(IPPORT_USERRESERVED, ne nécessite pas un mode privilégié) */
bzero((char *)&adr_serv, sizeof(adr_serv)) ;
adr_serv.sin_port = IPPORT_USERRESERVED ;
adr_serv.sin_addr.s_addr = INADDR_ANY; /*pour accepter une adresse quelconque
(0.0.0.0) */
/* autre solution pour obtenir l'adresse IP de la machine locale : */
gethostname(nom_serv, (size_t) 30);
//printf("nom du serveur %s \n", nom_serv);
if ((num_ip_serv=gethostbyname("127.0.0.1"))==NULL)
{ printf(" échec dans l'obtention de l'adresse IP du serveur \n");
return -1;
}
else
{
bcopy(num_ip_serv->h_addr,&adr_serv.sin_addr,num_ip_serv->h_length);} /* fin
solution */
adr_serv.sin_family =AF_INET ;
bind (sock_ecoute, (struct sockaddr*)&adr_serv, sizeof(adr_serv)) ; /* attachement
de la socket */
/* ouverture du service : création d'une file d'attente pour les demandes de
connexion d'une longueur max. égale à 5*/
listen(sock_ecoute,5) ;
int num_clients=0;
for (;;)/* boucle infinie traitant les différentes des clients */
{
/* attente d'une demande de connexion */
lg_adr_client = sizeof(adr_client) ;
sock_dial=accept(sock_ecoute, (struct sockaddr *)&adr_client, &lg_adr_client) ;
/* création d'un processus fil */
/* attente d'une demande de connexion */

   

if (fork() ==0)
{ /* fils */
close(sock_ecoute) ;
/* réception d’un message sur la socket au moyen de la primitive read. IL existe
une autre primitive de réception « recv », celle-ci utilise un paramètre
supplémentaire « in t flag » (en dernier param.) permettant de consulter des
données sans les prélever (flag=MSG_PEEK), ... */
read(sock_dial,tampon,sizeof(tampon)) ;
printf("Client : %s\n", tampon) ;

char Smot[256]="b,a,m,i,d,t,r";
char Cmot[256]="Salut !";
write (sock_dial,Smot,sizeof(Smot) );
write(sock_dial, Cmot, sizeof(Cmot));

//mot from client
read(sock_dial,tampon,sizeof(tampon)) ;
printf("Client : %s\n", tampon) ;
int found;char message[256];int i=9;
verifier(Smot, tampon, &found);
write(sock_dial, &found, sizeof(found));
    if (found!=0)
	write (sock_dial,"Bonne reponse !",sizeof("Bonne reponse !"));
    else{
         while(1){
            sprintf(message, "Faux,tu as %d essais", i);
            write(sock_dial, message, sizeof(message));
            read(sock_dial,tampon,sizeof(tampon)) ;
            printf("Client : %s\n", tampon) ;
            verifier(Smot, tampon, &found);
            write(sock_dial, &found, sizeof(found));
              i--;
            if (found!=0){
	write (sock_dial,"Bonne reponse !",sizeof("Bonne reponse !"));
      break;}
        if(i<1){
            write (sock_dial,"Mauvaise reponse !\n",sizeof("Mauvaise reponse !\n"));
        break;}
        }
         }
return 0 ;
}
close(sock_dial) ;
} /* for */
}/*main*/