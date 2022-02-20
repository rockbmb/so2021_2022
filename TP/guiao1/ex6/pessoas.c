#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */

#include <stdio.h> /* perror, printf */
#include <stdlib.h>
#include <string.h>

#define _POSIX_C_SOURCE 200809L

#define NAME_MAX_LEN 40

typedef struct pessoa {
    char nome[NAME_MAX_LEN];
    int idade;
} * Pessoa;

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
        return 1;
    }

    if (!strcmp(argv[1], "-i")) {
        lseek(fd_bin, 0, SEEK_END);
        strncpy(p->nome, argv[2], NAME_MAX_LEN);
        p->idade = atoi(argv[3]);

        write(fd_bin, p, sizeof(struct pessoa));
    }

    if (!strcmp(argv[1], "-g")) {
        lseek(fd_bin, 0, SEEK_SET);
        int found = 0;
        while (!found) {
            int r = read(fd_bin, p, sizeof(struct pessoa));
            if (r == 0) {
                write(STDOUT_FILENO, "Não encontrado!\n", 18);
                break;
            }
            if (strcmp(p->nome, argv[2])) {
                write(STDOUT_FILENO, "Encontrado: ", 13);
                write(STDOUT_FILENO, p->nome, NAME_MAX_LEN);
                write(STDOUT_FILENO, "\n", 1);
                found = 1;
            }
        }
    }

    close(fd_bin);

    return 0;
}