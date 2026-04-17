#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

// Variables globales sans volatile
int nb_lus_fils1 = 0;
int nb_lus_fils2 = 0;
int nb_param = 0;
int delai = 0;
pid_t pid1, pid2;

void traiterSIGINT(int signal) {
    printf("\nPere (%d) : Interruption par SIGINT.\n", getpid());
    kill(pid1, SIGKILL);
    kill(pid2, SIGKILL);
    exit(0);
}

void traiterSIGALRM(int signal) {
    time_t t;
    time(&t);
    
    // Formater l'heure pour enlever le saut de ligne de ctime
    char *time_str = ctime(&t);
    for(int i = 0; time_str[i] != '\0'; i++) {
        if(time_str[i] == '\n') time_str[i] = '\0';
    }

    printf("Pere (%d)\n%s: Fils 1 nombre de caracteres = %d\n", getpid(), time_str, nb_lus_fils1);
    printf("Pere (%d)\n%s: Fils 2 nombre de caracteres = %d\n", getpid(), time_str, nb_lus_fils2);
    
    alarm(delai); // Relancer l'alarme
}

void traiterSIGUSR1(int signal) { nb_lus_fils1 += nb_param; }
void traiterSIGUSR2(int signal) { nb_lus_fils2 += nb_param; }

void code_fils(int num_fils, int sig_cible, int nb, int nbl) {
    int total = 0;
    int c;
    pid_t ppid = getppid();

    while (total < nbl) {
        c = getchar();
        if (c == EOF) break;
        if (c != '\n') {
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
    if (argc != 4) {
        fprintf(stderr, "Usage : %s <NB> <NBL> <DELAI>\n", argv[0]);
        exit(1);
    }

    nb_param = atoi(argv[1]);
    int nbl = atoi(argv[2]);
    delai = atoi(argv[3]);

    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    // Association des signaux
    act.sa_handler = traiterSIGUSR1; sigaction(SIGUSR1, &act, NULL);
    act.sa_handler = traiterSIGUSR2; sigaction(SIGUSR2, &act, NULL);
    act.sa_handler = traiterSIGALRM; sigaction(SIGALRM, &act, NULL);
    act.sa_handler = traiterSIGINT;  sigaction(SIGINT,  &act, NULL);

    pid1 = fork();
    if (pid1 == 0) code_fils(1, SIGUSR1, nb_param, nbl);

    pid2 = fork();
    if (pid2 == 0) code_fils(2, SIGUSR2, nb_param, nbl);

    // Lancer la première alarme
    alarm(delai);

    // Attendre que les deux fils atteignent la limite
    while (nb_lus_fils1 < nbl || nb_lus_fils2 < nbl) {
        pause(); 
    }

    // Nettoyage avant de finir
    alarm(0); // Désactiver l'alarme
    wait(NULL);
    wait(NULL);
    
    printf("Pere (%d) Je me termine (en dernier)\n", getpid());
    return 0;
}