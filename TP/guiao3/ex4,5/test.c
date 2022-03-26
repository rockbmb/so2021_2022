#define _DEFAULT_SOURCE

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */

int main(int argc, char const *argv[]) {

    char *end, *r, *tok;
    r = end = strdup("cat | nl | head -n 10 | tail -n 5 | tac ? < test.c > out");
    assert(end != NULL);

    char *rest = NULL;
    char *temp, *temp2, *token, *token_inner;

    int count = 0;

    while ((temp = strsep(&end, "?")) != NULL) {
        char *rest = NULL;
        if (count == 0) {
            for (token = strtok_r(temp, "|", &rest); token != NULL; token = strtok_r(NULL, "|", &rest)) {
                int i = 0;
                temp2 = token;
                char *rest_inner = NULL;
                for (token_inner = strtok_r(temp2, " ", &rest_inner); token_inner != NULL; token_inner = strtok_r(NULL, " ", &rest_inner)) {
                    printf("i: %d;  str %s\n", i, token_inner);
                    i++;
                }
            }
        }

        if (count == 1) {
            for (token = strtok_r(temp, " ", &rest); token != NULL; token = strtok_r(NULL, " ", &rest)) {
                printf("%s\n", token);
            }
        }
        
        count++;
    }

    return 0;
}
