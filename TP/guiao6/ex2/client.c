#include <unistd.h>
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <string.h>

#include "fifo.h"

int main (int argc, char *argv[]) {
    if (argc < 2) {
        perror("Número de argumentos insuficiente!");
        return 1;
    }

    /* On WSL, mkfifo cannot be used on the Windows filesystem: it must be created
    on the Ubuntu filesystem.
    https://github.com/Microsoft/WSL/issues/3962#issuecomment-480078059
    */
    int fd = open(fifo, O_WRONLY, 0660);

    write(fd, argv[1], strlen(argv[1]));
    close(fd);
    return 0;
}