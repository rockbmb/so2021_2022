#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */

#include <stdio.h> /* perror, printf */
#include <stdlib.h> /* atoi */

int main (int argc, char * argv[]) {

    if (argc < 3) {
        write(STDERR_FILENO, "Número de argumentos insuficiente!\n", 37);
        return 1;
    }

    int fd_in = open(argv[1], O_RDONLY);
    if (fd_in < 0) {
        perror("Erro a abrir ficheiro de entrada: ");
        return 1;
    }

    int fd_out = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0660);
    if (fd_out < 0) {
        perror("Erro a abrir ficheiro de saída: ");
        return 1;
    }

    int buf_len = atoi(argv[3]);
    if (buf_len <= 0) {
        write(STDERR_FILENO, "Comprimento de buffer inválido!\n", 34);
        return 1;
    }

    char buf[buf_len];

    int n = 0;
    while(n = read(fd_in, buf, buf_len)) {
        if (n < 0) {
            perror("Erro a ler de ficheiro de entrada: ");
            return 1;
        }

        int wr_res = write(fd_out, buf, n);
        if (wr_res < 0) {
            perror("Erro a escrever em ficheiro de saída: ");
            return 1;
        }

        if (n < buf_len) {
            break;
        }
    }

    close(fd_in);
    close(fd_out);

    return 0;
}