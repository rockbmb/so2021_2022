#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <unistd.h>

#include "gen.h"

#define _GNU_SOURCE

int *gen_matrix(int lines, int cols, int max) {
    int *random = mmap(0, lines * cols, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, 0, 0);

    //int **random = malloc(lines * sizeof(int *));

    int lin, col;

    /*for (lin = 0; lin < lines; lin++) {
        random[lin] = malloc(cols * sizeof(int));
    }*/

    srand(time(NULL));
    for(lin = 0; lin < lines; lin++) {
        for(col = 0; col < cols; col++) {
            random[lin * cols + col] = rand() % max;
        }
    }

    return random;
}