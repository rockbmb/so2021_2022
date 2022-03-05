#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */
#include <time.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINES 10
#define COLS 25000
#define MAXNUM 1000

// Number to be found
#define NEEDLE 1000

void gen_matrix(int random[LINES][COLS], int max) {
    int lin, col;

    srand(time(NULL));
    for(lin = 0; lin < LINES; lin++) {
        for(col = 0; col < COLS; col++) {
            random[lin][col] = rand() % max;
        }
    }
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

int min(int a, int b) {
    if (a <= b) {
        return a;
    } else {
        return b;
    }
}

int main(int argc, char * argv[]) {
    pid_t self = getpid();

    int random[LINES][COLS];
    gen_matrix(random, MAXNUM);

    random[0][20001] = 1000;
    random[8][9200] = 1000;
    random[4][11200] = 1000;

    /*int lin, col;
    for(lin = 0; lin < LINES; lin++) {
        for(col = 0; col < COLS; col++) {
            printf("%2d  ", random[lin][col]);
        }
        printf("\n");
    }*/

    int matrix_fd = open("matrix.bin", O_CREAT | O_TRUNC | O_WRONLY, 0660);

    int write_res = write(matrix_fd, &random, LINES * COLS * sizeof(int));
    if (write_res < 0) {
        perror("main: escrita de matriz em ficheiro falhou!");
        return 1;
    }
    close(matrix_fd);

    pid_t children[LINES];
    int results[LINES];
    int fds[LINES][2];

    int i;

    for (i = 0; i < LINES; i++) {
        pipe(fds[i]);
        children[i] = fork();
        pid_t parent = getppid();

        if (!children[i]) {
            int child_fd = open("matrix.bin", O_RDONLY);
            lseek(child_fd, i * COLS * sizeof(int), SEEK_SET);

            int buf[BUFSIZ];
            int found = -1;
            bool done = false;

            int left_in_row = COLS;
            int chunk_size = min(left_in_row, BUFSIZ);

            // How many integers from chunks have been read.
            int offset = 0;

            int read_res;

            while (!done) {
                if (left_in_row <= BUFSIZ) {
                    done = true;
                }

                read_res = read(child_fd, &buf, chunk_size * sizeof(int));
                if (read_res < 0) {
                    perror("Problem reading from file!");
                    exit(-1);
                }
                left_in_row -= (read_res / sizeof(int));
                found = search(buf, read_res / sizeof(int), NEEDLE);

                if (found != -1) {
                    done = true;
                    found += offset;
                }

                offset += read_res / sizeof(int);
                chunk_size = min(left_in_row, BUFSIZ);
            }

            close(fds[i][0]);
            write(fds[i][1], &found, sizeof(found));
            close(fds[i][1]);

            close(child_fd);

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
            if (WEXITSTATUS(st)) {
                printf("Eu sou o pai %d, e o meu filho %d terminou mal!\n", self, wpid);
            }
        }
    }

    for (i = 0; i < LINES; i++) {
        if (results[i] == -1) {
            printf("Eu sou o pai %d, e o meu filho %d não encontrou o número %d na linha %d.\n", self, children[i], NEEDLE, i + 1);
        } else {
            printf("Eu sou o pai %d, e o meu filho %d encontrou o número %d na coluna %d da linha %d.\n", self, children[i], NEEDLE, results[i], i + 1);
        }
    }

    return 0;
}