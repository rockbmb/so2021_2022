#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */

#include <stdio.h>

#define READEND 0
#define WRITEEND 1

int main(int argc, char const *argv[]) {
    int p[2];
    int res = pipe(p);
    if (res < 0) {
        perror("main: pipe creation failed");
    }

    pid_t child = fork();
    if (!child) {
        close(p[WRITEEND]);
        dup2(p[READEND], STDIN_FILENO);
        execlp("wc", "wc", (char *) NULL);
    } else {
        close(p[READEND]);

        char buf[BUFSIZ];
        int r;
        do {
            r = read(STDIN_FILENO, buf, BUFSIZ);
            write(p[WRITEEND], buf, r);
        } while (r != 0);
        close(p[WRITEEND]);
    }

    return 0;
}
