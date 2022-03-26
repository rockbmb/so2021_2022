#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "mysystem.h"

int mysystem(char *command){

/*
*  If command is NULL, then a nonzero value if a shell is available, or 0 if no shell is available.
*/
    if (command == NULL) {
        return 0;
    }

    // Will hold the total number of arguments, including (char *) NULL.
    int count = 0;

    char *rest = NULL;

    char *temp, *temp2, *token;
    int command_len = strlen(command);
    temp = malloc(sizeof(char) * command_len);
    temp2 = malloc(sizeof(char) * command_len);
    strncpy(temp, command, command_len);
    strncpy(temp2, command, command_len);

    for (token = strtok_r(temp, " ", &rest); token != NULL; token = strtok_r(NULL, " ", &rest)) {
        count++;
    }

    char *exec_args[count + 1];

    rest = NULL;
    int i = 0;

    for (exec_args[i] = strtok_r(temp2, " ", &rest); exec_args[i] != NULL; exec_args[i] = strtok_r(NULL, " ", &rest)) {
        i++;
    }
    exec_args[count] = (char *) NULL;

    pid_t child = fork();

    if (child < 0) {
        perror("mysystem: failed to create child process!");
        return -1;
    }

    /* Redirecting IO */
    char *input, *output;
    int ix;
    bool in, out, out1, out2, err, err1, err2;

    // Not worth iterating to count + 1, (char *) NULL does not matter.
    for (ix = 0; ix < count; ix++) {
        in = !strncmp(exec_args[ix], "<", 1);

        out1 = !strncmp(exec_args[ix], ">", 1); 
        out2 = !strncmp(exec_args[ix], ">>", 2);
        out = out1 || out2;

        err1 = !strncmp(exec_args[ix], "2>", 2);
        err2 =  !strncmp(exec_args[ix], "2>>", 3);
        err = err1 || err2;

        if (in || out || err) {
            break;
        }
    }

    if (in) {
        if (exec_args[ix + 1] == NULL) {
            printf("redir < não tem valor!\n");
            return 1;
        }
        input = exec_args[ix + 1];
        ix += 2;
    }

    // Retest
    out1 = !strncmp(exec_args[ix], ">", 1); 
    out2 = !strncmp(exec_args[ix], ">>", 2);
    out = out1 || out2;
    if (out) {
        if (exec_args[ix + 1] == NULL) {
            printf("redir > não tem valor!\n");
            return 1;
        }
        output = exec_args[ix + 1];
        ix += 2;
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

    free(temp);
    free(temp2);

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

    int res = mysystem(test2);
    printf("filho saiu com %d\n", res);
}*/