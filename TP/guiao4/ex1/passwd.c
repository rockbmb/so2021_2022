#include<unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include<sys/wait.h>
#include<stdio.h>
#include<fcntl.h>

#include "../../guiao1/ex3,4,5/readln2.h"

int main(int argc, char const *argv[])
{
    int saved_stdout;

    int i_fd = open("/etc/passwd", O_RDONLY);
    int o_fd = open("saida.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666);
    int e_fd = open("erros.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666);

    saved_stdout = dup(STDOUT_FILENO);

    dup2(i_fd, STDIN_FILENO);
    dup2(o_fd, STDOUT_FILENO);
    dup2(e_fd, STDERR_FILENO);

    close(i_fd);
    close(o_fd);
    close(e_fd);

    MYFILE * mf = myopen(STDIN_FILENO, BUFSIZ);
    char line[BUFSIZ];

    int read_res;

    do {
        read_res = readln2(mf, (char*) &line, BUFSIZ);
        write(STDOUT_FILENO, line, read_res);
        write(STDERR_FILENO, line, read_res);
    } while (read_res != 0);

    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    write(STDOUT_FILENO, "terminei!\n", 11);

    myclose(mf);

    return 0;
}