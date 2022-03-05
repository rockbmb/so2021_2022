#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */
#include <time.h>

#include <stdio.h>
#include <stdlib.h>

#define LINES 10
#define COLS 200
#define MAXNUM 1000

// Number to be found
#define NEEDLE 1000

int **gen_matrix(int lines, int cols, int max) {
    int **random = malloc(lines * sizeof(int *));

    int lin, col;

    for (lin = 0; lin < lines; lin++) {
        random[lin] = malloc(cols * sizeof(int));
    }

    srand(time(NULL));
    for(lin = 0; lin < lines; lin++) {
        for(col = 0; col < cols; col++) {
            random[lin][col] = rand() % max;
        }
    }

    return random;
}

void free_matrix(int **mat, int lines) {
    int lin;

    for (lin = 0; lin < lines; lin++) {
        free(mat[lin]);
    }

    free(mat);
}

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

    int **random = gen_matrix(LINES, COLS, MAXNUM);

    random[2][7] = 1000;
    random[8][76] = 1000;

    int lin, col;
    for(lin = 0; lin < LINES; lin++) {
        for(col = 0; col < COLS; col++) {
            printf("%2d  ", random[lin][col]);
        }
        printf("\n");
    }

    pid_t children[LINES];
    int results[LINES];
    int fds[LINES][2];

    int i;

    for (i = 0; i < LINES; i++) {
        pipe(fds[i]);
        children[i] = fork();
        pid_t parent = getppid();

        if (!children[i]) {
            close(fds[i][0]);
            int res = search(random[i], COLS, NEEDLE);
            write(fds[i][1], &res, sizeof(res));
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
        if (!WIFEXITED(st)) {
            printf("Eu sou o pai %d, e o meu filho %d terminou mal!\n", self, wpid);
        }
    }

    for (i = 0; i < LINES; i++) {
        if (results[i] == -1) {
            printf("Eu sou o pai %d, e o meu filho %d não encontrou o número %d na linha %d.\n", self, children[i], NEEDLE, i + 1);
        } else {
            printf("Eu sou o pai %d, e o meu filho %d encontrou o número %d na coluna %d da linha %d.\n", self, children[i], NEEDLE, results[i], i + 1);
        }
    }

    free_matrix(random, LINES);

    return 0;
}