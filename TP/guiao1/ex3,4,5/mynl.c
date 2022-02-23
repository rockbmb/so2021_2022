#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */

#include <stdio.h> /* perror, printf */
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef SLOW
#include "readln1.h"
#else
#include "readln2.h"
#endif

#define BUFSIZ 8

int main(int argc, char * argv[]) {
    int count = 1;
    char * line = malloc(BUFSIZ * sizeof(char));
    memset(line, BUFSIZ, 0);
#ifdef SLOW
    printf("BOOM!");
#else
    MYFILE * mf = myopen(STDOUT_FILENO, 16);
    printf("BOW!");
#endif

    ssize_t res = 0;
    read_line:
#ifdef SLOW
    while (res = readln1(STDIN_FILENO, line, BUFSIZ)) {
#else
    while (res = readln2(mf, line, BUFSIZ)) {
#endif
        if (res < 0) {
            perror("Erro a ler de STDIN");
            return 1;
        }

        if (res == 1) {
            if (line[res - 1] == '\n') {
                write(STDOUT_FILENO, "\n", 1);
                goto read_line;
            }
        }

        char buf[10];
        int written = snprintf(buf, 10, "%6d  ", count);
        write(STDOUT_FILENO, buf, 10);

        if (line[res - 1] == '\n') {
            write(STDOUT_FILENO, line, res);
        }

        if ((res == BUFSIZ) && (line[res - 1] != '\n')) {
            bool more = true;
            write(STDOUT_FILENO, line, res);
            while (more) {
#ifdef SLOW
                res = readln1(STDIN_FILENO, line, BUFSIZ);
#else
                res = readln2(mf, line, BUFSIZ);
#endif
                //printf("res: %ld\n", res);
                if (res < 0) {
                    perror("Erro a ler de STDIN");
                    return 1;
                }
                write(STDOUT_FILENO, line, res);
                if ((res < BUFSIZ) || (line[res - 1] == '\n')) {
                    more = false;
                }
            }
        }

        count++;
        memset(line, BUFSIZ, 0);
    }

#ifdef SLOW
#else
    myclose(mf);
#endif
    return 0;
}