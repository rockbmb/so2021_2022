#define NAME_MAX_LEN 40

#define _POSIX_C_SOURCE 200809L

typedef struct pessoa {
    char nome[NAME_MAX_LEN];
    int idade;
} * Pessoa;

off_t insere_pessoa(int fd_pessoas, char *nome, int idade);
int atualiza_idade_1(int fd_pessoas, char *nome, int nova_idade);
int atualiza_idade_2(int fd_pessoas, int posicao, int nova_idade);