#define _DEFAULT_SOURCE

#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STATELEN 256

int main(int argc, char *argv[]) {
    int res;

    int seed = getpid() * time(NULL);
    char randomStateBuffer[STATELEN];
    struct random_data rand_data;
    memset(&rand_data, 0, sizeof(struct random_data));
    initstate_r(seed, (char *)&randomStateBuffer, STATELEN, &rand_data);
    srandom_r(seed, &rand_data);

    /*srandom is not reentrant; calling it with just time(NULL) at the same time in
    different processes will cause results to not be random at all.
    srandom(getpid() * time(NULL));
    res = random() % 3;
    */
    random_r(&rand_data, &res);
    res = res % 5;
    //printf("%s: %d\n", argv[0], res);
    return res;
}