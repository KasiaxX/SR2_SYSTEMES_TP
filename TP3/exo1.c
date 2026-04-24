// #define _POSIX_C_SOURCE 200809L
// #define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>


int randomSleep(){
    srand(getpid());
    int dormir = rand()%101; //modulo pour que le nombre est entre 0 et 100
    if(usleep(dormir) == -1){
        perror("usleep");
        exit(3);
    }
    return dormir;
}

int main(int argc, char * argv[]){

    if( argc != 3){
        fprintf(stderr, "Usage : %s <nb_processus> <nb_fois>", argv[0]);
        exit(1);
    }

    int processus = atoi(argv[1]);
    int fois = atoi(argv[2]);

    pid_t pid;
    int status;

    for(int i=0; i<processus; i++){
        switch (pid=fork()){
        case -1:
            perror("fork");
            exit(2);
        case 0:
            for(int j=0; j<fois; j++){
                printf("Activite rang %d : identifiant = %d (delai = %d)\n", i, getpid(), randomSleep());
            }
            WEXITSTATUS(status);
        
        default:
            wait(&status);
            printf("Valeur retournee par le fils %d = %d\n", getpid(), i);
            exit(0);
        }
    } 

    return 0;
}
