#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */

#include <stdio.h>

#define READEND 0
#define WRITEEND 1

int main(int argc, char const *argv[])
{
    char line[] = "eu sou o pai, e enviei isto ao meu filho.";

    int p[2];
    int res = pipe(p);
    if (res < 0) {
        perror("main: pipe creation failed");
    }

    pid_t child = fork();
    if (!child) {
        close(p[WRITEEND]);
        char buf[BUFSIZ];
        read(p[READEND], buf, BUFSIZ);
        printf("Eu sou o filho, e li: %s\n", buf);
        close(p[READEND]);
    } else {
        close(p[READEND]);
        write(p[WRITEEND], line, 42);
        close(p[WRITEEND]);
    }

    return 0;
}
