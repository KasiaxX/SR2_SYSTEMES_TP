#define _POSIX_C_SOURCE 200809L //il faut le mettre pour la struct signaction 
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

int recu = 1;

void traiterSignal(int signal){
    printf("Signal [%d] reçu \n", signal);
    recu = 0;
}

void fils(){

    pid_t pid_a_moi = getpid();
    struct sigaction act;

    sigemptyset(&act.sa_mask);  //pour vider le masque ; au cas ou

    //se proteger contre SIGINT
    act.sa_handler = traiterSignal; //si il recoit un signal par sigaction il fait traiterSignal
    act.sa_flags = 0;

    if(sigaction(SIGINT, &act, NULL) == -1){ //associe une signal a une action
        perror("Echec signaction");
        exit(2);
    }

    while(recu){
        printf("[Fils] (%d) : Je suis protégé \n", pid_a_moi );
        sleep(1);

    }

}

int main(int argc, char * argv[]){

    if(argc != 1){
        fprintf(stderr, "ERROR : trop d'argments");
        exit(1);
    }

    pid_t pid;
    pid_t pidPere = getpid();
    //time_t t;

    printf("[Pére] (%d) : Je me démarre ... \n", pidPere);

    switch (pid = fork())
    {
    case -1:
        perror("Echec fork");
        exit(2);
    case 0: 
        fils();
    
    default:
        wait(NULL);
        printf("[Pére] (%d) : ok je me fini \n", pidPere);
        exit(0);
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
