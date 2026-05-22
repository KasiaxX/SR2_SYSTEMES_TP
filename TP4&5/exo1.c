#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

int nb_threads;
int nb_messages;
int nb_lignes;

void * fct(){
    printf(" Afficheur ");

}

void affichage(void){
    
}


int main(int argc, char* argv){

    if(argc != 4){
        fprintf(stderr, "Usage : <nb_threads> <nb_messages> <nb_lignes>");
        exit(1);
    }

    nb_threads = atoi(argv[1]);
    nb_messages = atoi(argv[2]);
    nb_lignes = atoi(argv[3]);


    pid_t pid;
    pthread_t threads[20];
    
    for(int i = 0; i<nb_threads; i++){
        if(pthread_create(&threads[i], NULL, fct, (void*)&i) != 0){
            perror("thread_create");
            exit(2);
        }
    }

    for(int i = 0; i<nb_threads; i++){
        
        if(pthread_join(threads[i], (void**)) != 0){
            perror("thread_join");
            exit(3);

        }
    }

    


}