#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mysystem.h"

/*

Sobre a sintaxe dos comandos:

* O redirecionamento I/O é feito no *fim* da linha, e segue-se necessariamente
a um (ou mais) caracter(es) '!':
* O redirecionamento I/O é feito necessariamente na order:
  * input, output, e depois descritor de erro.

command -arg1 -arg2 ... -argn (!)* < in_file > out_file 2> err_file

*/

int myshell() {
    char *input = malloc(BUFSIZ * sizeof(char));
    memset(input, 0, BUFSIZ);

    bool background = false;
    pid_t child, wait_pid, zombie_pid;
    int status;

    int write_res;
    int read_res;
    int res;

    do {
start:
        /*
        Important, otherwise commands inserted at one point can affect future ones
        if they are shorter in length e.g.

        cat ! < test.c > out.txt 2> err.txt
        cat out.txt

        Bash will error with "2>: No such file or directory".
        */
        memset(input, 0, BUFSIZ);
        write_res = write(STDOUT_FILENO, "\033[0;31mMyPrompt\033[0m \033[0;36m$\033[0m ", 34);
        if (write_res < 0) {
            perror("myshell: escrita para terminal falhou!");
            return -1;
        }

        read_res = read(STDIN_FILENO, input, BUFSIZ);
        if (read_res < 0) {
            perror("myshell: leitura de input falhou!");
            return -1;
        }
        if (read_res == 0) {
            write(STDOUT_FILENO, "\nBye!\n", 7);
            goto end;
        }

        if (read_res == 1) {
            if (input[0] == '\n' || input[0] == '\0') {
                goto end;
            } else {
                input[1] = '\0';
            }
        }

        if (!strncmp(input, "exit", 4)) {
            write(STDOUT_FILENO, "Exiting!\n", 10);
            while (waitpid(-1, NULL, 0) > 0);
            return 0;
        }

        /*
        Se a linha terminar com &, executar em background.
        Se for este o caso, remover o '&' e substituir por '\0'.
        */
        if(input[read_res - 1] == '&') {
            background = true;
            input[read_res - 1] = '\0';
        }
        if(read_res >= 2) {
            if (input[read_res - 2] == '&') {
                background = true;
                input[read_res - 2] = '\0';
            }
        }

        child = fork();
        if (child < 0) {
            perror("myshell: fork falhou!");
            return -1;
        }

        if (!child) {
            _exit(mysystem(input));
        }

        /*
        Sleep for a moment in case background child process writes to STDOUT and
        mangles prompt.
        */
        if (background) {
            usleep(500 * 1000);
        } else {
            wait_pid = waitpid(child, &status, 0);
            if(WIFEXITED(status)) {
                if (WEXITSTATUS(status) == 0) {}

                if (WEXITSTATUS(status) == 127) {
                    write(STDERR_FILENO, "myshell: command failed!\n", 26);
                }

                // mysystem child exited with -1, _exit collects lowest 8 bits.
                if (WEXITSTATUS(status) == 255) {
                    write(STDERR_FILENO, "myshell: command failed!\n", 26);
                }
                } else {
                    write(STDERR_FILENO, "myshell: child exited abnormally!\n", 35);
                }

            // periodically reap zombie children.
            do {
                zombie_pid = waitpid(-1, &status, WNOHANG);
            } while (zombie_pid > 0);
        }

end:
        background = false;

    } while (read_res > 0);

    free(input);
}

int main(int argc, char *argv[]) {
    myshell();
}