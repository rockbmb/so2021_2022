#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */

#include <stdio.h>

int main(int argc, char *argv[]) {
    int res = execlp("ls", "-l", "..", (char *) NULL);
    if (res < 0) {
        perror("main: execlp falhou!");
        return -1;
    }
}