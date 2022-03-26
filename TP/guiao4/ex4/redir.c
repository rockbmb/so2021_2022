#include <sys/types.h>
#include <unistd.h> /* chamadas#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Argumentos insuficientes!\n");
        return 1;
    }

    int ix = 1;
    char *input, *output;
    input = NULL;
    output = NULL;
    bool in, out;
    in = !strncmp(argv[ix], "-i", 2);
    out = !strncmp(argv[ix], "-o", 2);
    if (in || out) {
        if (in) {
            if (argv[ix + 1] == NULL) {
                printf("-i não tem valor!\n");
                return 1;
            }
            input = argv[ix + 1];
            ix += 2;
        }

        // Retest
        out = !strncmp(argv[ix], "-o", 2);
        if (out) {
            if (argv[ix + 1] == NULL) {
                printf("-o não tem valor!\n");
                return 1;
            }
            output = argv[ix + 1];
            ix += 2;
        }
    }

    //printf("in: %s out: %s ix: %d\n", input, output, ix);

    int fd_in, fd_out;

    if (input == NULL) {
        fd_in = STDIN_FILENO;
    } else {
        fd_in = open(input, O_RDONLY);
        if (fd_in < 0) {
            perror("open de input file falhou");
            return 1;
        }
    }

    if (output == NULL) {
        fd_out = STDIN_FILENO;
    } else {
        fd_out = open(output, O_CREAT | O_TRUNC | O_WRONLY, 0666);
        if (fd_out < 0) {
            perror("open de output file falhou");
            return 1;
        }
    }

    int saved_stdin, saved_stdout;

    saved_stdout = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);

    dup2(fd_in, STDIN_FILENO);
    dup2(fd_out, STDOUT_FILENO);

    close(fd_in);
    close(fd_out);

    pid_t child = fork();
    if (!child) {
        int res = execvp(argv[ix], &argv[ix]);
        if (res < 0) {
            perror("execvp failed");
            return 1;
        }
    }

    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
    close(saved_stdin);

    int st;
    waitpid(child, &st, 0);
    if (WIFEXITED(st)) {
        printf("child exited with %d.\n", WEXITSTATUS(st));
    }

    return 0;
}