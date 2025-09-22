#include "parser.h"
#include "lexer_classes.h"
#include "lexer_customcss.h"
#include "vector.h"
#include "stdio.h"
#include "uthash.h"
#include "stdbool.h"
#include "file.h"

void insertVal(char dst[CSS_MAX], char src[CUSTOM_VALUE_MAX]);
char *target = "value";

void parse(Vector *customcss, Vector *classes, char *cssPath) {
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
    HASH_FIND_STR(hash, c->class_name, found);
    if (found) {
      int written = 0;
      if (c->custom_val[0] == '\0') {
        written = snprintf(css_buffer + total_css_bytes, BUFFER_CSS_MAX - total_css_bytes, ".%s %s\n", found->class_name, found->css);
      } else {
        int totalCustomCSSBytes = strlen(found->css);
        int totalCssVal = strlen(c->custom_val);
        // + 2 for []
        if ((totalCustomCSSBytes - (strlen(target) + 2)) + totalCssVal >= BUFFER_CSS_MAX - 1) {
          fprintf(stderr,
          "Error: CSS buffer memory limit exceeded (attempted %ld bytes, max %d bytes)\n",
          (totalCustomCSSBytes - (strlen(target) + 2)) + totalCssVal,
          BUFFER_CSS_MAX - 1);
          exit(EXIT_FAILURE);
        }
        insertVal(found->css, c->custom_val);
        written = snprintf(css_buffer + total_css_bytes, BUFFER_CSS_MAX - total_css_bytes, ".%s-\\[%s\\] %s\n", found->class_name, c->custom_val, found->css);
      }
      if (written < 0) {
        perror("snprintf failed!");
        exit(EXIT_FAILURE);
        return;
      }
      total_css_bytes += written;
      if (total_css_bytes >= BUFFER_CSS_MAX - 1) {
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

void insertVal(char dst[CSS_MAX], char src[CUSTOM_VALUE_MAX]) {
  bool stop = false;
  char searchTarget[SEARCH_KEY_MAX] = {0};
  char bytes_after_val[CLASS_NAME_MAX] = {0};
  int searchTargetIndex = 0;
  int strIndex = 0;
  int start_val_index = 0;
  LexerVal_State state = STATE_IGNORE;

  while (dst[strIndex] != '\0') {
    switch (state) {
      case STATE_SEARCH_KEY:
      if (
        dst[strIndex] == 'v' || dst[strIndex] == 'a' ||
        dst[strIndex] == 'l' || dst[strIndex] == 'u' ||
        dst[strIndex] == 'e'
      ) {
        searchTarget[searchTargetIndex] = dst[strIndex];
        searchTargetIndex++;
        if (searchTargetIndex == SEARCH_KEY_MAX - 1) {
          searchTarget[searchTargetIndex] = '\0';
          if (strcmp(target, searchTarget) == 0) {
            state = STATE_SAVE_BYTES_AFTER_KEY;
          } else {
            state = STATE_IGNORE;
          }
        }
      } else {
        state = STATE_IGNORE;
      }
      break;

      case STATE_SAVE_BYTES_AFTER_KEY:
        if (dst[strIndex] == ']') {
          strIndex++;
          int bytesIndex = 0;
          while (dst[strIndex] != '\0') {
            bytes_after_val[bytesIndex] = dst[strIndex];
            bytesIndex++;
            strIndex++;
          }
          strIndex = start_val_index;
          bytes_after_val[bytesIndex] = '\0';
          state = STATE_INSERT_VALUE;
        } else {
          state = STATE_IGNORE;
        }
        break;

      case STATE_INSERT_VALUE:
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
        // + 2 for []
        if (valIndex - 1 < strlen(target) + 2) {
          for (int i = ((strlen(target) + 2) - (valIndex - 1)); i; i--) {
            dst[strIndex] = 0;
            strIndex++;
          }
        }
        bytes_after_val[bytesIndex] = '\0';
        stop = true;
        break;

      case STATE_IGNORE:
        if (dst[strIndex] == '[') {
          start_val_index = strIndex - 1;
          state = STATE_SEARCH_KEY;
        } else if(dst[strIndex+1] == '[') {
          memset(searchTarget, 0, 6);
          searchTargetIndex = 0;
        } 
        break;

      default:
        break;
    }

    if (stop) {
      break;
    }
    
    strIndex++;
  }
}