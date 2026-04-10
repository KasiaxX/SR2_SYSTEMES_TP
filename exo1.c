#define _POSIX_C_SOURCE 200809L //il faut le mettre pour la struct signaction 
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>


void traiterSignal(int signal){
    printf("Signal [%d] reçu \n", signal);
}

int main(int argc, char * argv[]){

    if(argc != 1){
        fprintf(stderr, "ERROR : trop d'argments");
        exit(1);
    }

    pid_t pid = getpid();
    time_t t;

    struct sigaction act;

    sigemptyset(&act.sa_mask);  //pour vider le masque ; au cas ou
    act.sa_handler = traiterSignal;
    act.sa_flags = 0;

    if(sigaction(SIGUSR1, &act, NULL) == -1){ //associe une signal a une action
        perror("Echec signaction");
        exit(2);
    }
    

    while(1){
        printf("Je suis le processus %d %s", pid, (time(&t), ctime(&t)));
        sleep(1);
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