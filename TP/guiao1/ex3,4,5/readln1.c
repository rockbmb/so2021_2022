#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */

#include <stdio.h> /* perror, printf */
#include <string.h>
#include <stdbool.h>

#include "readln1.h"

/*
linha maior que size, caracter não é newline, necessário chamar função de novo.
*/
ssize_t readln1(int fd, char *line, size_t size) {
    size_t index = 0;
    char c;
    int read_res = 0;
    bool end_of_line = false;

    read_res = read(fd, &c, 1);
    while (read_res && (index < size) && !end_of_line) {
        if (read_res < 0) {
            perror("Erro a ler do descritor fornecido");
            return read_res;
        }
        line[index] = c;
        index++;
        if (c == '\n') {
            end_of_line = true;
        } else {
            read_res = read(fd, &c, 1);
        }
    }

    return index;
}