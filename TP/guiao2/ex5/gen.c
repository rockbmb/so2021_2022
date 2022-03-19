#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <unistd.h>

#include "gen.h"

#define _GNU_SOURCE

int *gen_matrix(int lines, int cols, int max) {
    int *random = mmap(0, lines * cols * sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    if (random == (void *) -1) {
        perror("genc: mmap failed!");
        return NULL;
    }

    int lin, col;

    srand(time(NULL));
    for(lin = 0; lin < lines; lin++) {
        for(col = 0; col < cols; col++) {
            random[lin * cols + col] = rand() % max;
        }
    }

    return random;
}

int **gen_mat(int lines, int cols, int max) {
    int **random = malloc(lines * sizeof(int *));
    if (random == NULL) {
        perror("genc: malloc failed!");
        return NULL;
    }

    for (int i = 0; i < lines; i++) {
        random[i] = (int *) malloc(cols * sizeof(int));
    }

    int lin, col;

    srand(time(NULL));
    for(lin = 0; lin < lines; lin++) {
        for(col = 0; col < cols; col++) {
            random[lin][col] = rand() % max;
        }
    }

    return random;
}