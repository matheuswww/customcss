#include "lexer_class.h"
#include "lexer_customcss.h"
#include "parser.h"
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "file.h"

int main(int argc, char* argv[]) {
  if (argc != 4) {
    perror("invalid args");
    exit(EXIT_FAILURE);
  }

  char *htmlContent = read_file(argv[1]);
  if (!htmlContent) {
    perror("invalid args");
    exit(EXIT_FAILURE);
  };

  char *customcssContent = read_file(argv[2]);
  if (!customcssContent) {
    perror("invalid args");
    exit(EXIT_FAILURE);
  };

  char classes_str[] = "";
  Vector* customcss = lexer_customcss(customcssContent);
  Vector* classes = lexer_class(htmlContent);

  parser(customcss, classes, argv[3]);

  vector_free(customcss);
  vector_free(classes);
}