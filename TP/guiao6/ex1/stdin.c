#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>

#include "../../guiao1/ex3,4,5/readln2.h"

int main (int argc, char *argv[]) {
    const char* fifo = "fifo";

    int fd = open(fifo, O_WRONLY, 0660);
    if (fd < 0) {
        write(STDERR_FILENO, "main: problem opening fifo!", 33);
        return 1;
    }

    int old_stdout = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    close(fd);

    int rd;
    MYFILE *mf = myopen(STDIN_FILENO, BUFSIZ);
    char buf[BUFSIZ];
    do {
        rd = readln2(mf, (char *) &buf, BUFSIZ);
        if (rd < 0) {
            perror("read: failed to read from STDIN");
            return 1;
        }
        int wr = write(STDOUT_FILENO, buf, rd);
        if (wr < 0) {
            perror("write: failed to write to STDOUT");
        }
    } while (rd != 0);

    dup2(old_stdout, STDOUT_FILENO);
    close (old_stdout);

    write(STDOUT_FILENO, "terminei!\n", 11);

    return 0;
}