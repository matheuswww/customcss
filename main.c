#include "lexer_class.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUF_MAX 8192

int main() {  
  char html[] = "<div class='mp-[12px] ml-2'></div>";
  char *a = (char *)malloc(BUF_MAX);

  memmove(a, html, strlen(html));
  
  lexer(a);
}
