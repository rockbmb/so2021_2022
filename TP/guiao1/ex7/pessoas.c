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
        off_t off = lseek(fd_bin, 0, SEEK_END);
        strncpy(p->nome, argv[2], NAME_MAX_LEN);
        p->idade = atoi(argv[3]);

        write(fd_bin, p, sizeof(struct pessoa));
        char buf[20];
        // Registos começam em 1.
        int written = snprintf(buf, 20, "registo %lu\n", 1 + off/(sizeof(struct pessoa)));
        write(STDOUT_FILENO, buf, written);
    }

    if (!strcmp(argv[1], "-u")) {
        // Registos começam em 1, necessário decrementar.
        int num = atoi(argv[2]) - 1;
        int new_age = atoi(argv[3]);

        lseek(fd_bin, num * (sizeof(struct pessoa)), SEEK_SET);

        int r = read(fd_bin, p, sizeof(struct pessoa));
        if (r < 0) {
            write(STDERR_FILENO, "Erro a ler de ficheiro binário!\n", 34);
            return 0;
        }

        printf("Idade: %d.\n", p->idade);
        p->idade = new_age;
        lseek(fd_bin, -1 * (sizeof(struct pessoa)), SEEK_CUR);
        write(fd_bin, p, sizeof(struct pessoa));
        printf("Nova Idade: %d\n.", p->idade);

    }

    close(fd_bin);

    return 0;
}