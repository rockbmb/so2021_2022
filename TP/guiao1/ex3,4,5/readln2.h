#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */

typedef struct MYFILE
{
    int fd;
    char* ptr;
    size_t start;
    size_t currCap;
    size_t maxCap;
} MYFILE;

MYFILE * myopen(int fd, size_t bufsiz);

ssize_t readln2(MYFILE * mf, char *line, size_t size);

void myclose(MYFILE * mf);