#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */

/*
linha maior que size, não é newline, chama de novo.
*/

/*
int readc(int fd, char *c);
*/
ssize_t readln(int fd, char *line, size_t size) {
    
}