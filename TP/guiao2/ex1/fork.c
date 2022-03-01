#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */

#include <stdio.h>

int main(int argc, char * argv[]) {
    pid_t parent = getppid();
    pid_t self = getpid();
    printf("Eu sou o main com PID %d, e o meu pai tem PID %d.\n", self, parent);

    return 0;
}