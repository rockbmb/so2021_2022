#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        write(STDOUT_FILENO, "Argumentos insuficientes para main!\n", 37);
        return 1;
    }

    int progs = argc - 1;
    

    char *exec_args[progs][2];

    for (int i = 0; i < progs; i++) {
        // This is probably wrong, shouldn't we use strcpy here?
        exec_args[i][0] = argv[i + 1];
        exec_args[i][1] = (char *) NULL;
    }

    pid_t children[progs];

    for (int i = 0; i < progs; i++) {
        children[i] = fork();
        if (!children[i]) {
            int res = execvp(exec_args[i][0], exec_args[i]);
            if (res < 0) {
                perror("main: execlp falhou!");
                _exit(1);
            }
        }
    }

    int status;
    pid_t wpid;
    pid_t self = getpid();
    while ((wpid = waitpid(-1, &status, 0)) >= 0) {
        if (!WIFEXITED(status)) {
            if (WEXITSTATUS(status)) {
                printf("Eu sou o pai %d, e o meu filho %d terminou mal!\n", self, wpid);
            }
        }
    }

    return 0;
}