#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */

#include <stdio.h> /* perror, printf */
#include <stdlib.h> /* atoi */

int main (int argc, char * argv[]) {
    if (argc < 2) {
        write(STDERR_FILENO, "Número de argumentos insuficiente!\n", 37);
        return 1;
    }

    int buf_len = atoi(argv[1]);
    if (buf_len <= 0) {
        write(STDERR_FILENO, "Comprimento de buffer inválido!\n", 34);
        return 1;
    }

    char buf[buf_len];

    int n = 0;
    while(n = read(STDOUT_FILENO, buf, buf_len)) {
        if (n < 0) {
            perror("Erro a ler de stdin: ");
            return 1;
        }

        int wr_res = write(STDOUT_FILENO, buf, n);
        if (wr_res < 0) {
            perror("Erro a escrever em stdout: ");
            return 1;
        }
    }

    return 0;
}