#include "parser.h"
#include "lexer_class.h"
#include "lexer_customcss.h"
#include "vector.h"
#include "stdio.h"
#include "uthash.h"
#include "stdbool.h"
#include "file.h"

void parser(Vector *customcss, Vector *classes, char *cssPath) {
  CustomCSS *hash = NULL;
  char *css_buffer = malloc(BUFFER_CSS_MAX);
  int total_css_bytes = 0;
  
  for (int i = 0; i < customcss->len; i++) {
    CustomCSS *c = vector_get(customcss, i);
    HASH_ADD_STR(hash, class_name, c);
  }

  for (int i = 0; i < classes->len; i++) {
    Class *c = vector_get(classes, i);
    CustomCSS *found;
    HASH_FIND_STR(hash, c->name, found);
    if (found) {
      int written = 0;
      if (c->val[0] == '\0') {
        written = snprintf(css_buffer + total_css_bytes, BUFFER_CSS_MAX - total_css_bytes, ".%s %s\n", found->class_name, found->css);
      } else {
        replaceVal(found->css, c->val);
        written = snprintf(css_buffer + total_css_bytes, BUFFER_CSS_MAX - total_css_bytes, ".%s-\\[%s\\] %s\n", found->class_name, c->val, found->css);
      }
      if (written < 0) {
        perror("snprintf failed!");
        exit(EXIT_FAILURE);
        return;
      }
      total_css_bytes += written;
      if (total_css_bytes >= BUFFER_CSS_MAX) {
        write_file(cssPath, css_buffer, total_css_bytes);
        total_css_bytes = 0;
      }
    }
  }
  if (total_css_bytes > 0) {
    write_file(cssPath, css_buffer, total_css_bytes);
    total_css_bytes = 0;
  }

  free(css_buffer);
}

void replaceVal(char dst[CSS_MAX], char src[VALUE_MAX]) {
  char key[] = "[value]";
  int strIndex = 0;
  int start_val_index = 0;
  bool openBracket = false;
  bool closeBracket = false;
  char bytes_after_val[NAME_MAX] = {0};

  while (dst[strIndex] != '\0') {
    if (closeBracket) {
      int valIndex = 0;
      while (src[valIndex] != '\0') {
        dst[strIndex] = src[valIndex];
        valIndex++;
        strIndex++;
      }
      int bytesIndex = 0;
      while (bytes_after_val[bytesIndex] != '\0') {
        dst[strIndex] = bytes_after_val[bytesIndex];
        bytesIndex++;
        strIndex++;
      }
      if (valIndex - 1 < strlen(key)) {
        for (int i = (strlen(key) - (valIndex - 1)); i; i--) {
          dst[strIndex] = 0;
          strIndex++;
        }
      }
      bytes_after_val[bytesIndex] = '\0';
      break;
    }

    if (dst[strIndex] == '[') {
      openBracket = true;
      start_val_index = strIndex - 1;
    }

    if (dst[strIndex] == ']' && openBracket) {
      strIndex++;
      closeBracket = true;
      int bytesIndex = 0;
      while (dst[strIndex] != '\0') {
        bytes_after_val[bytesIndex] = dst[strIndex];
        bytesIndex++;
        strIndex++;
      }
      strIndex = start_val_index;
      bytes_after_val[bytesIndex] = '\0';
    }
    
    strIndex++;
  }
}