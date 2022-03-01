#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */
#include <sys/mman.h>

#include <stdio.h>

#include "gen.h"

#define LINES 5
#define COLS 20
#define MAXNUM 1000

// Number to be found
#define NEEDLE 1000

int search(int * haystack, int len, int needle) {
    int k;
    int res = -1;

    for (k = 0; k < len; k++) {
        if (haystack[k] == needle) {
            res = k;
            break;
        }
    }

    return res;
}

int main(int argc, char * argv[]) {
    pid_t self = getpid();

    int *random = gen_matrix(LINES, COLS, MAXNUM);

    random[2 * COLS + 7] = 1000;

    int lin, col;
    srand(time(NULL));
    for(lin = 0; lin < LINES; lin++) {
        for(col = 0; col < COLS; col++) {
            printf("%2d  ", random[lin * COLS + col]);
        }
        printf("\n");
    }

    pid_t children[LINES];
    int statuses[LINES];
    int results[LINES];
    int fds[LINES][2];

    int i;

    for (i = 0; i < LINES; i++) {
        pipe(fds[i]);
        children[i] = fork();
        pid_t parent = getppid();
        int child_res = 0;

        if (!children[i]) {
            close(fds[i][0]);
            pid_t child = getpid();
            int res = search(&random[i * COLS], COLS, NEEDLE);
            printf("Eu sou o filho %d, o meu pai é %d, e encontrei o número na coluna %d.\n", child, self, res);
            write(fds[i][1], &child_res, sizeof(child_res));
            close(fds[i][1]);
            _exit(0);
        } else {
            close(fds[i][1]);
            read(fds[i][0], &results[i], sizeof(int));
            close(fds[i][0]);
        }
    }

    int st;
    pid_t wpid;
    // Espera pelo término de quaisquer outros processos-filho.
    while ((wpid = waitpid(-1, &st, 0)) >= 0) {
        /*if (WIFEXITED(st)) {
            int child_res = WEXITSTATUS(st);
            if (child_res == -1) {
                printf("Eu sou o pai %d, e o meu filho %d não encontrou o número %d na linha.\n", self, wpid, NEEDLE);
            } else {
                printf("Eu sou o pai %d, e o meu filho %d encontrou o número %d.\n", self, wpid, NEEDLE);
            }
        } else */if (!WIFEXITED(st)) {
            printf("Eu sou o pai %d, e o meu filho %d terminou mal!\n", self, wpid);
        }
    }

    munmap(random, LINES * COLS);

    return 0;
}