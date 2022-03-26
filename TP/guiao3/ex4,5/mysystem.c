#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */

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

int mysystem(char *command) {

    int size = command_size(command);
    char **exec_args = malloc(sizeof(char *) * (size + 1));

    int command_len = strlen(command);
    char *temp = malloc(sizeof(char) * command_len);
    strncpy(temp, command, command_len);

    process_command(temp, exec_args, size);

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
        int exec_res = execvp(exec_args[0], exec_args);
        if (exec_res < 0) {
            perror("mysystem: execvp failed in child process!");
            _exit(127);
        }
    }

    free(exec_args);

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
    char test[] = "ls -a -l ../../guiao1/ex3,4,5";
    char test2[] = "pwd";

    int res = mysystem(test);
    printf("filho saiu com %d\n", res);
}*/