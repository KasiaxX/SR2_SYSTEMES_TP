#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

// Variables globales sans volatile
int nb_lus_fils1 = 0;
int nb_lus_fils2 = 0;
int nb_param = 0;

void traiterSIGUSR1(int signal) {
    nb_lus_fils1 += nb_param;
    printf("Pere (%d) Fils 1 : nombre de caracteres = %d\n", getpid(), nb_lus_fils1);
}

void traiterSIGUSR2(int signal) {
    nb_lus_fils2 += nb_param;
    printf("Pere (%d) Fils 2 : nombre de caracteres = %d\n", getpid(), nb_lus_fils2);
}

void code_fils(int num_fils, int sig_cible, int nb, int nbmax) {
    int total = 0;
    int c;
    pid_t ppid = getppid();

    while (total < nbmax) {
        c = getchar();
        if (c == EOF) break;
        if (c != '\n') { // On ignore la touche Entrée dans le comptage
            total++;
            if (total % nb == 0) {
                printf("Fils %d (%d) : %d caracteres de plus => %d\n", num_fils, getpid(), nb, total);
                kill(ppid, sig_cible);
            }
        }
    }
    printf("Fils %d (%d) : Termine\n", num_fils, getpid());
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage : %s <NB> <NBMAX>\n", argv[0]);
        exit(1);
    }

    nb_param = atoi(argv[1]);
    int nbmax = atoi(argv[2]);

    struct sigaction act1, act2;
    
    // Config USR1
    sigemptyset(&act1.sa_mask);
    act1.sa_handler = traiterSIGUSR1;
    act1.sa_flags = 0;
    sigaction(SIGUSR1, &act1, NULL);

    // Config USR2
    sigemptyset(&act2.sa_mask);
    act2.sa_handler = traiterSIGUSR2;
    act2.sa_flags = 0;
    sigaction(SIGUSR2, &act2, NULL);

    // Création des fils
    if (fork() == 0) code_fils(1, SIGUSR1, nb_param, nbmax);
    if (fork() == 0) code_fils(2, SIGUSR2, nb_param, nbmax);

    // Attente de la fin des fils
    wait(NULL);
    wait(NULL);

    printf("Pere (%d) Je me termine en dernier\n", getpid());
    return 0;
}