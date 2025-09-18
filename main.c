#include "lexer_class.h"
#include "lexer_customcss.h"
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
    char customcss[] = "key: {\nbackground-color: white;\n}\nkey_2: {\ncolor:black;\n}";
    Vector* c_1 = lexer_customcss(customcss);
    char classes[] = "<body><p class='test'>test</p></body>";
    Vector* c_2 = lexer_class(classes);
    
    vector_free(c_1);
    vector_free(c_2);
    return 0;
}
