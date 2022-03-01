#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */

#include <stdio.h>

int main(int argc, char * argv[]) {
    pid_t pid = fork();

    pid_t parent = getppid();
    pid_t self = getpid();

    printf("Meu pid: %d; pid do meu pai: %d.\n", self, parent);
    if (pid) {
        int status;
        waitpid(pid, &status, WUNTRACED);
        printf("Eu sou o pai %d, e o meu filho tem pid %d.\n", self, pid);
    }

    return 0;
}