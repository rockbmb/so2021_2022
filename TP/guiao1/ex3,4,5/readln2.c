#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "readln2.h"

MYFILE * myopen(int myfd, size_t bufsiz) {
    MYFILE * mf = malloc(sizeof(MYFILE));
    if (mf == NULL) {
        perror("myopen: Memory allocation for MYFILE failed! ");
        return NULL;
    }

    mf->ptr = malloc(bufsiz * sizeof(char));
    if (mf->ptr == NULL) {
        perror("myopen: Memory allocation for MYFILE buffer failed! ");
        free(mf->ptr);
        free(mf);
        return NULL;
    }

    mf->fd = myfd;
    mf->start = 0;
    mf->currCap = 0;
    mf->maxCap = bufsiz;

    return mf;
}

void myclose(MYFILE * mf) {
    free(mf->ptr);
    free(mf);
}

ssize_t readln2(MYFILE * mf, char *line, size_t size) {
    size_t index = 0;
    int read_res = 0;
    bool end_of_line = false;

    while ((index < size) && !end_of_line) {

        if (mf->currCap == 0) {
            mf->start = 0;
            read_res = read(mf->fd, mf->ptr, mf->maxCap);
            if (read_res < 0) {
                perror("Erro a ler do descritor fornecido");
                return read_res;
            }
            // If there's nothing to read from STDIN, it means we've reached
            // the end of the line.
            if (read_res == 0) {
                end_of_line = true;
            }
            mf->currCap = read_res;
        }

        int i = mf->start;
        while((i < (mf->start + mf->currCap)) && (index < size)) {
            line[index] = mf->ptr[i];
            mf->start++;
            mf->currCap--;
            index++;
            i++;
            // If the last character written to `line` is \n or \0, we've
            // reached the end of the line, so both this inner loop and
            // the outer one can stop executing.
            if ((mf->ptr[i - 1] == '\n') || (mf->ptr[i - 1] == 0)) {
                end_of_line = true;
                break;
            }
        }
    }

    return index;
}