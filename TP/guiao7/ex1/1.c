#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdio.h>

volatile int seconds = 0;

volatile bool done = false;

volatile int ctrl_c_counter = 0;

void sigint_handler(int signum) {
    ctrl_c_counter++;
    printf("CTRL+C\n");
    printf("segundos: %d\n", seconds);
}

void sigquit_handler(int signum) {
    printf("CTRL+\\\n");
    printf("ctrl+c's: %d\n", ctrl_c_counter);
    done = true;
}

void sigalrm_handler(int signum) {
    seconds++;
    alarm(1);
}

int main(int argc, char *argv[]) {
    if(signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("SIGINT failed!");
    }
    alarm(1);

    if(signal(SIGQUIT, sigquit_handler) == SIG_ERR) {
        perror("SIGQUIT failed!");
    }
    if(signal(SIGALRM, sigalrm_handler) == SIG_ERR) {
        perror("SIGALRM failed!");
    }

    while(!done) {
        pause();
    }

    return 0;
}