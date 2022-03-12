#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */

#include <stdio.h> /* perror, printf */
#include <stdlib.h>
#include <string.h>

#include "pessoa.h"

int main(int argc, char * argv[]) {
    if (argc < 4) {
        write(STDERR_FILENO, "Número de argumentos insuficiente!\n", 37);
        return 1;
    }

    int fd_bin = open("pessoas.bin", O_CREAT | O_RDWR, 0660);
    if (fd_bin < 0) {
        perror("Erro a abrir ficheiro de entrada: ");
        return 1;
    }

    Pessoa p = malloc (sizeof(struct pessoa));
    if (p == NULL) {
        write(STDERR_FILENO, "malloc falhou!", 15);
        close(fd_bin);
        return 1;
    }

    if (!strcmp(argv[1], "-i")) {
        off_t record_number = insere_pessoa(fd_bin, argv[2], atoi(argv[3]));

        char buf[20];
        off_t offset = (record_number - 1) * (sizeof(struct pessoa));
        int written = snprintf(buf, 20, "registo %lu\n", record_number);
        write(STDOUT_FILENO, buf, written);
    }

    if (!strcmp(argv[1], "-u_old")) {
        int new = atoi(argv[3]);
        int old = atualiza_idade_1(fd_bin, argv[2], new);
        if (old == -1) {
            write(STDOUT_FILENO, "Não encontrado!\n", 18);
            return 1;
        } else {
            write(STDOUT_FILENO, "Encontrado registo. \n", 22);
            printf("Idade anterior: %d\n", old);
            printf("Nova Idade: %d\n", new);
        }
    }

    if (!strcmp(argv[1], "-u_new")) {
        // Registos começam em 1, necessário decrementar.
        int new = atoi(argv[3]);
        int old = atualiza_idade_2(fd_bin, atoi(argv[2]), new);
        
        if (old < 0) {
            write(STDERR_FILENO, "Erro a ler de ficheiro binário!\n", 34);
            return 1;
        }
        printf("Idade anterior: %d\n", old);
        printf("Nova Idade: %d\n", new);
    }

    close(fd_bin);

    return 0;
}