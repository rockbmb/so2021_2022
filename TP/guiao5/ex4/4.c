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

    char *programs[2][3] = {
        {"ls", "/etc", (char *) NULL},
        {"wc", "-l", (char *) NULL}
    };

    int progs = 2;
    int i = 0;
    for (i = 0; i < progs; i++) {
        pid_t child = fork();
        if (!child) {
            if (i == 0) {
                close(p[READEND]);
                dup2(p[WRITEEND], STDOUT_FILENO);
                //close(p[WRITEEND]);
            }
            if (i == (progs - 1)) {
                close(p[WRITEEND]);
                dup2(p[READEND], STDIN_FILENO);
                //close(p[READEND]);
            }
            execvp(programs[i][0], programs[i]);
        }
    }

    return 0;
}
