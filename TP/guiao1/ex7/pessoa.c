#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pessoa.h"

off_t insere_pessoa(int fd_pessoas, char *nome, int idade) {
    Pessoa p = malloc(sizeof(struct pessoa));
    strncpy(p->nome, nome, NAME_MAX_LEN);
    p->idade = idade;

    off_t off = lseek(fd_pessoas, 0, SEEK_END);

    write(fd_pessoas, p, sizeof(struct pessoa));
    // Registos começam em 1.
    return (1 + off/(sizeof(struct pessoa)));
}

int atualiza_idade_1(int fd_pessoas, char *nome, int nova_idade) {
    Pessoa p = malloc(sizeof(struct pessoa));
    // if p == NULL ...

    lseek(fd_pessoas, 0, SEEK_SET);
    int old_idade = -1;
    int r;
    do {
        r = read(fd_pessoas, p, sizeof(struct pessoa));
        if (!strncmp(p->nome, nome, strlen(nome))) {
            old_idade = p->idade;
            lseek(fd_pessoas, -1 * (sizeof(struct pessoa)), SEEK_CUR);
            p->idade = nova_idade;
            write(fd_pessoas, p, sizeof(struct pessoa));
            break;
        }
    } while (r > 0);

    return old_idade;
}

int atualiza_idade_2(int fd_pessoas, int posicao, int nova_idade) {
    Pessoa p = malloc(sizeof(struct pessoa));
    // Registos começam em 1, necessário decrementar.
    int num = posicao - 1;

    lseek(fd_pessoas, num * (sizeof(struct pessoa)), SEEK_SET);

    int r = read(fd_pessoas, p, sizeof(struct pessoa));
    if (r < 0) {
        return -1;
    }
    int old_idade = p->idade;
    p->idade = nova_idade;
    lseek(fd_pessoas, -1 * (sizeof(struct pessoa)), SEEK_CUR);
    write(fd_pessoas, p, sizeof(struct pessoa));

    return old_idade;
}