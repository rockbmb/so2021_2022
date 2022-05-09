#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */

#include <stdio.h>
#include <stdlib.h>

#define READEND 0
#define WRITEEND 1

int main(int argc, char const *argv[]) {
    int program_num = 8;
    int pipe_num = program_num - 1;
    int pips[pipe_num][2];

    char *programs[8][5] = {
        {"grep", "-v", "^#", "/etc/passwd", (char *) NULL},
        {"nl", (char *) NULL},
        {"tac", (char *) NULL},
        {"head", "-n", "10", (char *) NULL},
        {"tail", "-n", "10", (char *) NULL},
        {"cut", "-f7", "-d:", (char *) NULL},
        {"uniq", (char *) NULL},
        {"wc", "-l", (char *) NULL}
    };

    int dup_res;
    int pipe_res;

    for (int cs = 0; cs < program_num; cs++) {
        if (cs == 0) {
            pipe_res = pipe(pips[cs]);
            if (pipe_res < 0) {
                perror("main: pipe creation failed");
                return 1;
            }

            pid_t child = fork();
            switch(child) {
                case -1:
                    perror("fork failed");
                    return -1;

                case 0:
                    close(pips[cs][READEND]);
                    dup_res = dup2(pips[cs][WRITEEND], STDOUT_FILENO);
                    close(pips[cs][WRITEEND]);

                    if (dup_res < 0) {
                        perror("dup2 failed");
                        _exit(1);
                    }
                    execvp(programs[cs][0], programs[cs]);

                default:
                    close(pips[cs][WRITEEND]);
            }
        }

        if ((cs > 0) && (cs < (program_num - 1))) {
            pipe_res = pipe(pips[cs]);
            if (pipe_res < 0) {
                perror("main: pipe creation failed");
                return 1;
            }

            pid_t child = fork();
            switch(child) {
                case -1:
                    perror("fork failed");
                    return -1;
                case 0:
                    close(pips[cs][READEND]);

                    dup_res = dup2(pips[cs][WRITEEND], STDOUT_FILENO);
                    if (dup_res < 0) {
                        perror("dup2 failed");
                        _exit(1);
                    }
                    close(pips[cs][WRITEEND]);

                    dup_res = dup2(pips[cs - 1][READEND], STDIN_FILENO);
                    if (dup_res < 0) {
                        perror("dup2 failed");
                        _exit(1);
                    }
                    close(pips[cs - 1][READEND]);

                    execvp(programs[cs][0], programs[cs]);
                
                default:
                    close(pips[cs][WRITEEND]);
                    close(pips[cs - 1][READEND]);
            }
        }

        if (cs == (program_num - 1)) {
            pid_t child = fork();
            switch (child) {
                case -1:
                    perror("fork failed");
                    return -1;
                case 0:
                    dup_res = dup2(pips[cs - 1][READEND], STDIN_FILENO);
                    if (dup_res < 0) {
                        perror("dup2 failed");
                        _exit(1);
                    }
                    close(pips[cs - 1][READEND]);

                    execvp(programs[cs][0], programs[cs]);

                default:
                    close(pips[cs - 1][READEND]);
            }
        }
    }

    int status;
    while (waitpid(-1, &status, 0) > 0) {
        printf("test\n");
    };


    return 0;
}
