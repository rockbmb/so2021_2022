#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */

#include <stdbool.h>
#include <stdio.h>

volatile int time_since_start = 0;

// Used to know if every child has exited; to avoid waiting the whole 10 seconds.
volatile int children_num = 0;

volatile int done_children = 0;
volatile bool children_all_exited = false;
volatile bool done = false;

void sigchld_handler(int signum) {
    pid_t pid;
    int status;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        done_children++;
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) == 0) {
                printf("filho %d encontrou!\n", pid);
                done = true;
            } else {
                printf("filho %d terminou sem encontrar!\n", pid);
            }
        }
    }
}
void alarm_handler(int signum) {
    alarm(1);
    time_since_start++;
}

int main(int argc, char* argv[]) {
    // For every folder in argv, create a child - the first two arguments
    // are the executable own name, and the pattern to grep for,
    int ch = argc - 2;
    children_num = ch;
    pid_t pids[ch];

    for (int i = 0; i < ch; i++) {
        pid_t p = fork();
        if (!p) execlp("grep", "grep", argv[1], "-r", argv[i + 2], (char *) NULL);
        else pids[i] = p;
    }

    signal(SIGCHLD, sigchld_handler);
    signal(SIGALRM, alarm_handler);
    alarm(1);

    while(time_since_start < 10 && !done && !(done_children == children_num)) {
        pause();
    }

    if (done) {
        printf("found!\n");
    } else {
        printf("not found!\n");
    }

    for (int i = 0; i < ch; i++) {
        kill(pids[i], SIGKILL);
    }
}
