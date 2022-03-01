#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */

#include <stdio.h>

int main(int argc, char * argv[]) {
    int i;

    pid_t children[10];
    int statuses[10];

    for (i = 0; i <= 9; i++) {
        children[i] = fork();

        pid_t parent = getppid();
        pid_t self = getpid();

        if (!children[i]) {
            sleep(3);
            printf("Meu pid: %d; pid do meu pai: %d.\n", self, parent);
            _exit(i + 1);
        }
    }

    for (i = 0; i <= 9; i++) {
        pid_t self = getpid();

        pid_t res = waitpid(children[i], &statuses[i], WUNTRACED);
        if (WIFEXITED(statuses[i])) {
            printf("Eu sou o pai %d, e o meu filho %d terminou com código %d.\n", self, children[i], WEXITSTATUS(statuses[i]));
        } else {
            printf("Eu sou o pai %d, e o meu filho %d terminou mal! %d.\n", self, children[i], res);
        }
    }

    int st;
    pid_t wpid;
    // Espera pelo término de quaisquer outros processos-filho.
    while ((wpid = waitpid(-1, &st, 0)) > 0) {}

    return 0;
}