#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>

int main (int argc, char *argv[]) {
    /* On WSL, mkfifo cannot be used on the Windows filesystem: it must be created
    on the Ubuntu filesystem.
    https://github.com/Microsoft/WSL/issues/3962#issuecomment-480078059
    */
    const char* fifo = "fifo";
    unlink(fifo);
    int res = mkfifo(fifo, 0660);
    if (res < 0) {
        perror("main: problem with mkfifo!");
        return 1;
    }
    return 0;
}