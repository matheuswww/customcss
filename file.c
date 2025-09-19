#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void write_file(const char *path, const char *w, int bytes) {
  FILE *file = fopen(path, "w");
  if (!file) {
    perror("Error trying openfile");
    exit(EXIT_FAILURE);
    return;
  }
  fwrite(w, sizeof(char), bytes, file);
  fclose(file);
}

char* read_file(const char *path) {
  FILE *file = fopen(path, "r");
  if (!file) {
    perror("Error trying openfile");
    exit(EXIT_FAILURE);
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *content = malloc(length + 1);
  if (!content) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
    fclose(file);
    return NULL;
  }

  fread(content, 1, length, file);
  content[length] = '\0';

  fclose(file);
  return content;
}