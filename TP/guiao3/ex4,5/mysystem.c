#define _DEFAULT_SOURCE

#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mysystem.h"

int command_size(char *command) {
    // Will hold the total number of arguments, including (char *) NULL.
    int count = 0;

    char *rest = NULL;

    char *temp, *token;
    int command_len = strlen(command);
    temp = malloc(sizeof(char) * command_len);
    strncpy(temp, command, command_len);

    for (token = strtok_r(temp, " \n", &rest); token != NULL; token = strtok_r(NULL, " \n", &rest)) {
        count++;
    }

    return count;
}

/*
temp, used in strtok_r, has to be passed in as an argument, or when it's freed,
it'll cause issues accessing elements of args
*/
int process_command(char * command, char **args, int count) {
    char *rest = NULL;
    char *token;

    int i = 0;
    // The delimiter string for strtok_r needs a newline, or execvp will fail without explanation.
    for (args[i] = strtok_r(command, " \n", &rest); args[i] != NULL; args[i] = strtok_r(NULL, " \n", &rest)) {
        i++;
    }
    args[count] = (char *) NULL;

    return 0;
}

/**
 * @brief Dada uma string com redirecionamentos IO, retira os nomes do ficheiros
 * para onde se redirecionam os descritores STD(IN|OUT|ERR).
 * 
 * @param io_redirs String com redirecionamentos no formato descrito em myshell.c
 * e.g. < in.txt > out.txt 2> err.txt
 * @param in_filename Ficheiro de onde se lerá  STDIN para o comando a ser executado. Opcional.
 * @param out_filename Ficheiro para onde se escreverá o STDOUT do comando a ser executado. Opcional.
 * @param err_filename Ficheiro para onde se escreverá o STDERR do comando a ser executado. Opcional.
 * 
 * Os três argumentos finais, que são sentinelas de output, são apontadores para strings, cuja memória
 * será alocada *nesta* função, mas que são declarados em mysystem(), devendo também ser libertados lá.
 */
void process_io_redir(char *io_redirs, char **in_filename, char **out_filename, char **err_filename) {
    int ioredirs_len = strlen(io_redirs);
    char *io_redirs_copy = malloc(sizeof(char) * ioredirs_len);
    strncpy(io_redirs_copy, io_redirs, ioredirs_len);

    int count = 0;
    char *rest = NULL;
    char *temp, *token;

    // The delimiter string for strtok_r needs a newline, or sometimes execvp will fail without explanation.
    for (token = strtok_r(io_redirs_copy, " \n", &rest); token != NULL; token = strtok_r(NULL, " \n", &rest)) {
        count++;
    }

    if (count == 0) {
        free(io_redirs_copy);
        return;
    }

    char *io_redirs_copy2 = malloc(sizeof(char) * ioredirs_len);
    strncpy(io_redirs_copy2, io_redirs, ioredirs_len);

    // String array with every token in the IO redirection part of the command.
    char **io_redirs_toks = malloc(sizeof(char *) * (count + 1));
    io_redirs_toks[count] = NULL;

    int ix = 0;
    rest = NULL;
    for (io_redirs_toks[ix] = strtok_r(io_redirs_copy2, " \n", &rest);
         io_redirs_toks[ix] != NULL;
         io_redirs_toks[ix] = strtok_r(NULL, " \n", &rest)) {
        ix++;
    }

    ix = 0;
    bool in, out, out1, out2, err, err1, err2;

    if (!strncmp(io_redirs_toks[ix], "<", 1)) {
        if (io_redirs_toks[ix + 1] == NULL) {
            perror("I/O redir com < não tem ficheiro associado!");
            return;
        }
        *in_filename = malloc(sizeof(char) * strlen(io_redirs_toks[ix + 1]));
        strcpy(*in_filename, io_redirs_toks[ix + 1]);
        ix += 2;
    }

    // There was only an input redirection, return early.
    if (ix == count) {
        return;
    }

    if (!strncmp(io_redirs_toks[ix], ">", 1)) {
        if (io_redirs_toks[ix + 1] == NULL) {
            perror("I/O redir com > não tem ficheiro associado!");
            return;
        }
        *out_filename = malloc(sizeof(char) * strlen(io_redirs_toks[ix + 1]));
        strcpy(*out_filename, io_redirs_toks[ix + 1]);
        ix += 2;
    }

    /* There was only an output redirection, or an input redir followed by an output redir.
    Return.
    */
    if (ix == count) {
        return;
    }

    if (!strncmp(io_redirs_toks[ix], "2>", 2)) {
        if (io_redirs_toks[ix + 1] == NULL) {
            perror("I/O redir com 2> não tem ficheiro associado!");
            return;
        }
        *err_filename = malloc(sizeof(char) * strlen(io_redirs_toks[ix + 1]));
        strcpy(*err_filename, io_redirs_toks[ix + 1]);
    }

    free(io_redirs_copy);
    free(io_redirs_copy2);
    free(io_redirs_toks);

    return;
}

int mysystem(char *command) {

    /* length of the input string. Needed to use strdup when processing it
    , to avoid modifying it.
    */
    int command_len = strlen(command);

    /* These variables will hold the input/output files to redirect from/to, if
    specified.
    */
    char *input_file, *output_file, *err_file;
    input_file = output_file = err_file = NULL;

    char *temp1 = malloc(sizeof(char) * command_len);
    strncpy(temp1, command, command_len);

    char *command_part = strsep(&temp1, "!");
    char *io_redir_part = strsep(&temp1, "!");
    if (io_redir_part != NULL) {
        process_io_redir(io_redir_part, &input_file, &output_file, &err_file);
    }

    int size = command_size(command_part);
    char **exec_args = malloc(sizeof(char *) * (size + 1));

    process_command(command_part, exec_args, size);

    int i = 0;
    pid_t child = fork();

    if (child < 0) {
        perror("mysystem: failed to create child process!");
        return -1;
    }

/*
* If a shell could not be executed in the child process, then the return value is as
though the child shell terminated by calling _exit(2) with the status 127.
*/
    if (!child) {
        if (input_file != NULL) {
            int fd_in = open(input_file, O_RDONLY);
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }

        if (output_file != NULL) {
            int fd_out = open(output_file, O_CREAT | O_TRUNC | O_WRONLY, 0666);
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }

        if (err_file != NULL) {
            int fd_err = open(err_file, O_CREAT | O_TRUNC | O_WRONLY, 0666);
            dup2(fd_err, STDERR_FILENO);
            close(fd_err);
        }

        int exec_res = execvp(exec_args[0], exec_args);
        if (exec_res < 0) {
            perror("mysystem: execvp failed in child process!");
            _exit(127);
        }
    }

    free(temp1);
    free(exec_args);
    free(input_file);
    free(output_file);
    free(err_file);

/*
* If all system calls succeed, then the return value is the termination status of the
child shell used to execute the command.
*/
    int status;
    waitpid(child, &status, 0);
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

/*
* If a child process could not be created, or its status could not be retrieved, the
return value is -1 and errno is set to indicate the error.
*/
    return -1;
}

/*int main() {
    char test[] = "ls -a -l ../../guiao1/ex3,4,5 ! < test.c > output 2> err.txt";
    char test2[] = "ls -a -l ! < test.c > ot.txt";

    int res = mysystem(test2);
    printf("filho saiu com %d\n", res);
}*/