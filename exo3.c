#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage : %s <nb_secondes> <nb_fois>\n", argv[0]);
        exit(1);
    }

    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_handler = SIG_IGN; // On ignore SIGINT
    act.sa_flags = 0;

    if (sigaction(SIGINT, &act, NULL) == -1) {
        perror("Echec sigaction");
        exit(2);
    }

    printf("Processus de pid %d : Je suis protege contre SIGINT\n", getpid());
    printf("Et je vais devenir l'executable boucler pour afficher %s fois toutes les %s secondes\n", argv[2], argv[1]);

    // Recouvrement
    execl("./boucler", "boucler", argv[1], argv[2], NULL);

    // Si on arrive ici, c'est que execl a échoué
    perror("Echec execl");
    exit(3);
}