#include "lexer_class.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUF_MAX 8192

int main() {
    char *tests[] = {
        NULL
    };

    for (int i = 0; tests[i] != NULL; i++) {
        printf("\n========== Teste %d ==========\n", i + 1);
        printf("HTML: %s\n\n", tests[i]);
        lexer_class(tests[i]);
        printf("\n"); // separador bonito
    }

    return 0;
}
