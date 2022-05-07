#include <unistd.h>
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <sys/types.h>
#include <sys/stat.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "fifo.h"

int main (int argc, char *argv[]) {
    /* On WSL, mkfifo cannot be used on the Windows filesystem: it must be created
    on the Ubuntu filesystem.
    https://github.com/Microsoft/WSL/issues/3962#issuecomment-480078059
    */
    unlink(fifo);
    int res = mkfifo(fifo, 0660);
    if (res < 0) {
        perror("main: problem with mkfifo!");
        return 1;
    }

    pid_t pid = fork();

    if (!pid) {
        bool done = false;
        int log_fd = open("logs.txt", O_CREAT | O_TRUNC | O_WRONLY, 0660);
        int fd;

        char buf[BUFSIZ];
        int rd;
        /**
         * TODO:
         * Usar open(fifo, WRONLY) no servidor para evitar race conditions com vários clientes.
         */
        while(!done) {
            fd = open(fifo, O_RDONLY, 0660);
            rd = read(fd, (char *) &buf, BUFSIZ);
            if (rd < 0) {
                perror("read: failed to read from STDIN");
                return 1;
            }
            /*int wr = write(STDOUT_FILENO, buf, rd);
            if (wr < 0) {
                perror("write: failed to write to STDOUT");
                return 1;
            }*/
            if (!strncmp("exit_server", buf, rd)) {
                done = true;
            };
            if (rd == BUFSIZ) {
                buf[rd - 1] = '\n';
            } else {
                buf[rd] = '\n';
                rd++;
            }
            int wr = write(log_fd, buf, rd);
            if (wr < 0) {
                perror("write: failed to write to log file!");
                return 1;
            }

            close(fd);
        };

        close(log_fd);
        _exit(0);
    }

    return 0;
}