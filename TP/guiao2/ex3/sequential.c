#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */

#include <stdio.h>

int main(int argc, char * argv[]) {
    int i;

    for (i = 1; i <= 10; i++) {
        pid_t pid = fork();

        pid_t parent = getppid();
        pid_t self = getpid();

        if (pid) {
            int status;
            pid_t res = waitpid(pid, &status, WUNTRACED);
            if (WIFEXITED(status)) {
                printf("Eu sou o pai %d, e o meu filho %d terminou com código %d.\n", self, pid, WEXITSTATUS(status));
            } else {
                printf("Eu sou o pai %d, e o meu filho %d terminou mal! %d.\n", self, pid, res);
            }
        } else {
            printf("Meu pid: %d; pid do meu pai: %d.\n", self, parent);
            _exit(i);
        }
    }

    return 0;
}