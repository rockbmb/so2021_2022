#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    pid_t pid = fork();
    if (!pid) {
        int res = execlp("ls", "-l", "..", (char *) NULL);
        if (res < 0) {
            perror("main: execlp falhou!");
            _exit(1);
        }
    }

    int status;
    pid_t self = getpid();
    waitpid(-1, &status, 0);
    if (WIFEXITED(status)) {
        if (!WEXITSTATUS(status)) {
            printf("Eu sou o pai %d e o meu filho %d saiu bem com %d.\n", self, pid, status);
        } else {
        printf("Eu sou o pai %d e o meu filho %d saiu mal com %d.\n", self, pid, status);
        }
    }

    return 0;
}