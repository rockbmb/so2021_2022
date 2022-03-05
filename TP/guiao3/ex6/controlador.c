#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */

#include <stdio.h>

#define NUM 3

int runner(char *prog) {
    int num_runs = 0;
    int prev_run_result = 1;
    int local_status = 0;
    pid_t runner;
    do {
        runner = fork();
        if (!runner) {
            execl(prog, prog, (char *) NULL);
        }
        num_runs++;
        waitpid(runner, &local_status, 0);
        if WIFEXITED(local_status) {
            prev_run_result = WEXITSTATUS(local_status);
        }
    } while (prev_run_result != 0);

    return num_runs;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("main: controlador com argumentos insuficientes");
        return 1;
    }

    int num = argc - 1;

    pid_t children[num];
    int statuses[num];
    int results[num];
    char *progs[num];
    for (int i = 1; i <= num; i++) {
        progs[i - 1] = argv[i];
    }

    for (int i = 0; i < num; i++) {
        children[i] = fork();
        if (children[i] < 0) {
            perror("main: fork failed!");
            return 1;
        }
        if (!children[i]) {
            _exit(runner(progs[i]));
        }
    }

    for (int i = 0; i < num; i++) {
        waitpid(children[i], &statuses[i], 0);
        if (WIFEXITED(statuses[i])) {
            results[i] = WEXITSTATUS(statuses[i]);
        }
    }

    for (int i = 0; i < num; i++) {
        char buf[20];
        int chars = snprintf(buf, 20, "%s: %d\n", progs[i], results[i]);
        write(STDOUT_FILENO, buf, chars);
    }

}