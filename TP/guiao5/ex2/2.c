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
        char buf[BUFSIZ];
        char text[BUFSIZ];

        int r;
        do {
            r = read(p[READEND], buf, BUFSIZ);
            int snr = snprintf(
                text,
                BUFSIZ,
                "Eu sou o filho %d, o meu pai é %d, e enviou-me o texto %s",
                getpid(),
                getppid(),
                buf);
            if (r != 0) {
                write(STDOUT_FILENO, text, snr);
            }
        } while (r != 0);
        close(p[READEND]);
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
