#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */

#include <stdio.h>
#include <stdlib.h>

#define READEND 0
#define WRITEEND 1

int main(int argc, char const *argv[]) {
    int program_num = 4;
    int pipe_num = program_num - 1;
    int pips[pipe_num][2];

    for (int p = 0; p < pipe_num; p++) {
        int res = pipe(pips[p]);
        if (res < 0) {
            perror("main: pipe creation failed");
            return 1;
        }
    }

    char *programs[4][5] = {
        {"grep", "-v", "^#", "5.c", (char *) NULL},
        {"cut", "-f7", "-d:", (char *) NULL},
        {"uniq", (char *) NULL},
        {"wc", "-l", (char *) NULL}
    };

    for (int i = 0; i < program_num; i++) {
        pid_t child = fork();

        if (!child) {
            int dup_res;
            if (i == 0) {
                close(pips[i][READEND]);
                dup_res = dup2(pips[i][WRITEEND], STDOUT_FILENO);
                if (dup_res < 0) {
                    perror("dup2 failed");
                    _exit(1);
                }
            }
            if ((i > 0) && (i < (program_num - 1))) {
                close(pips[i - 1][WRITEEND]);
                dup_res = dup2(pips[i - 1][READEND], STDIN_FILENO);
                if (dup_res < 0) {
                    perror("dup2 failed");
                    _exit(1);
                }

                close(pips[i][READEND]);
                dup_res = dup2(pips[i][WRITEEND], STDOUT_FILENO);
                if (dup_res < 0) {
                    perror("dup2 failed");
                    _exit(1);
                }

            }
            if (i == (program_num - 1)) {
                close(pips[i - 1][WRITEEND]);
                dup_res = dup2(pips[i - 1][READEND], STDIN_FILENO);
                if (dup_res < 0) {
                    perror("dup2 failed");
                    _exit(1);
                }
            }

            execvp(programs[i][0], programs[i]);
        }
    }

    return 0;
}
