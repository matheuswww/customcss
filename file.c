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
      perror("Error trying to open file");
      exit(EXIT_FAILURE);
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *content = malloc(length + 1);
  if (!content) {
      perror("malloc failed");
      fclose(file);
      exit(EXIT_FAILURE);
  }

  size_t bytesRead = fread(content, 1, length, file);
  if (bytesRead != length) {
    fprintf(stderr, "Error: failed to read file %s (read %zu of %ld bytes)\n",
            path, bytesRead, length);
    fclose(file);
    exit(EXIT_FAILURE);
  }

  content[length] = '\0';
  fclose(file);
  return content;
}
