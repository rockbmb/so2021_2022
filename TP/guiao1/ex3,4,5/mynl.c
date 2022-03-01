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

// Size of the buffer that will hold bytes from the byte descriptor.
#define BUFSIZ 8192

// Maximum amount of bytes to read from each line, in each call to readln2.
#define LINESIZ 4096

int main(int argc, char * argv[]) {
    int count = 1;
    char * line = malloc(LINESIZ * sizeof(char));
    memset(line, 0, LINESIZ * sizeof(char));

#ifdef SLOW
#else
    MYFILE * mf = myopen(STDIN_FILENO, BUFSIZ);
#endif

    ssize_t res = 0;
    read_line:
#ifdef SLOW
    res = readln1(STDIN_FILENO, line, LINESIZ);
    while (res) {
#else
    res = readln2(mf, line, LINESIZ);
    while (res) {
#endif
        if (res < 0) {
            perror("Erro a ler de STDIN");
            return 1;
        }

        if (res == 1) {
            if (line[0] == '\n') {
                write(STDOUT_FILENO, "\n", 1);
                goto read_line;
            }
        }

        char buf[8];
        int written = snprintf(buf, 8, "%6d\t", count);
        // Not necessary to write to STDOUT the '\0' that snprintf
        // inserts at the end of buf, so only write 7 bytes.
        write(STDOUT_FILENO, buf, 7);

        // Write first chunk of line to STDOUT, after writing line number.
        write(STDOUT_FILENO, line, res);

        if ((res == LINESIZ) && (line[res - 1] != '\n')) {
            bool more = true;
            while (more) {
                memset(line, 0, LINESIZ * sizeof(char));
#ifdef SLOW
                res = readln1(STDIN_FILENO, line, LINESIZ);
#else
                res = readln2(mf, line, LINESIZ);
#endif
                if (res < 0) {
                    perror("Erro a ler de STDIN");
                    return 1;
                }
                write(STDOUT_FILENO, line, res);

                // If this is true, it means the line has been read in its,
                // entirety, and written to STDOUT.
                if ((res < LINESIZ) || (line[res - 1] == '\n')) {
                    more = false;
                }
            }
        }

        /* Necessário porque se a linha lida terminar sem newline, quando
        for escrita em STDOUT vai aparecer em cima do "prompt".
        */
        if (line[res - 1] != '\n') {
            write(STDOUT_FILENO, "\n", 1);
        }
        count++;
        memset(line, 0, LINESIZ * sizeof(char));

#ifdef SLOW
        res = readln1(STDIN_FILENO, line, LINESIZ);
#else
        res = readln2(mf, line, LINESIZ);
#endif
    }

#ifdef SLOW
#else
    myclose(mf);
#endif

    free(line);
    return 0;
}/*12345667890*/