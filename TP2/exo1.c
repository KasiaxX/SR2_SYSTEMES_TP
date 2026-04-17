#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>

int cpt = 0;
int tube[2];
int secondes;
int nb_fois;

void traiterSignal(){
    if(cpt < nb_fois){
        cpt++;
        char buffer[] = "Je suis ton Pere !!!\n";
        write(tube[1], buffer, sizeof(buffer));
        alarm(secondes); // relance l’alarme
    }
}

int main(int argc, char* argv[]){

    if(argc != 3){
        fprintf(stderr, "Usage : %s <secondes> <nb_fois>", argv[0]);
        exit(1);
    }

    secondes = atoi(argv[1]);
    nb_fois = atoi(argv[2]);

    if(pipe(tube) == -1){
        perror("pipe");
        exit(3);
    }

    struct sigaction act;

    sigemptyset(&act.sa_mask);  //pour vider le masque ; au cas ou
    act.sa_handler = traiterSignal;
    act.sa_flags = 0;

    if(sigaction(SIGALRM, &act, NULL) == -1){ //associe une signal a une action
        perror("Echec sigaction");
        exit(2);
    }

    pid_t pid;    

    switch (pid = fork())
    {
    case -1:
        perror("fork");
        exit(4);  
    case 0: 
        close(tube[1]); //fermer ecriture

        char buffer[255];
        time_t t;

        for(int i = 0; i < nb_fois; i++){
            read(tube[0], buffer, sizeof(buffer));
            printf("Fils - Recu (%d) : %s à %s",
                   i+1, buffer, (time(&t), ctime(&t)));
        }

        close(tube[0]);
        
    default:
        close(tube[0]); // fermer en lecture
        alarm(secondes);  //declencher alarm

        while(cpt < nb_fois){  //tant que il y a pas alarm on ecrit
            printf("Pere - Je travaille entre les envois\n");
        }

        close(tube[1]);
    }
    
    return 0;

    /* 
    *  1 terminal:
    *  gcc -Wall exo1.c -o exo1
    * ./exo1
    * 
    * 2 terminal:
    * kill -USR1 11000
    */

}