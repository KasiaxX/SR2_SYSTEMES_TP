#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>


int main(int argc, char * argv[]){

    if (argc != 3){
        fprintf(stderr, "Usage : <prog> <nb secondes> <nb de fois> \n");
        exit(1);
    }

    int nb_secondes = atoi(argv[1]);
    int nb_fois = atoi(argv[2]);

    pid_t pid = getpid();
    time_t t;
    
    for(int i = 0; i<nb_fois; i++){
        printf("Mon numero est %d, il est %s", pid, (time(&t), ctime(&t)));
        sleep(nb_secondes);
    }

    return 0;
 
}