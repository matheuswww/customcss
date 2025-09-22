#include "lexer_customcss.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

Vector* lex_customcss(char *s) {
  Vector* vector = vector_init(sizeof(CustomCSS));
  char nameBuffer[CLASS_NAME_MAX] = {0};
  char cssBuffer[CSS_MAX] = {0};
  int cssIndex = 0;
  int nameIndex = 0;
  int strIndex = 0;
  LexerCustomCSS_State state = STATE_NAME;

  nameBuffer[nameIndex] = '\0';
  cssBuffer[cssIndex] = '\0';
  
  while (s[strIndex] != '\0') {
    char c = s[strIndex];
    
    switch (state) {
      case STATE_NAME:
        if (
          (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') || c == '_' || c == '-' || c == ':'
        ) {
          if (c == ':') {
            state = STATE_EXPECT_CSS;
          } else {
            if (nameIndex >= CLASS_NAME_MAX - 1) {
              fprintf(stderr,
              "Error: class name buffer memory limit exceeded max %d\n", CLASS_NAME_MAX - 1);
              exit(EXIT_FAILURE);
            }
            nameBuffer[nameIndex++] = c;
          }
        } else {
          state = STATE_IGNORE_CSS_VAL;
        }
        break;

      case STATE_EXPECT_CSS:
        if (c == '{') {
          state = STATE_CSS;
          cssBuffer[cssIndex++] = c;
        } else if (
          c != '\n' && c != '\r' && 
          c != '\t' && c != '\f' && 
          c != ' '
        ) {
          state = STATE_IGNORE_CSS_VAL;
        }
        break;

      case STATE_CSS:
        if (c == '}') {
          if (vector->len > CLASSES_MAX) {
            fprintf(stderr,
            "Error: CustomCSS vector memory limit exceeded "
            "(%d elements, max allowed %d)\n",
            vector->len, CLASSES_MAX);
            exit(EXIT_FAILURE);
          }

          cssBuffer[cssIndex++] = c;
          cssBuffer[cssIndex] = '\0';
          nameBuffer[nameIndex] = '\0';
          
          CustomCSS* customCSS = malloc(sizeof(CustomCSS));
          if (!customCSS) {
            perror("malloc failed!");
            exit(EXIT_FAILURE);
          }

          strncpy(customCSS->class_name, nameBuffer, CLASS_NAME_MAX);
          strncpy(customCSS->css, cssBuffer, CSS_MAX);

          vector_push(vector, customCSS);

          memset(nameBuffer, 0, CLASS_NAME_MAX);
          memset(cssBuffer, 0, sizeof(cssBuffer));
          nameIndex = 0;
          cssIndex = 0;

          state = STATE_NAME;
        } else {
           if (cssIndex >= CSS_MAX - 1) {
            fprintf(stderr,
            "Error: css buffer memory limit exceeded max %d\n", CLASS_NAME_MAX - 1);
            exit(EXIT_FAILURE);
           } 
          cssBuffer[cssIndex++] = c;
        }
        break;

      default:
        break;
    }
    if (
        state == STATE_IGNORE_CSS_VAL && (
          (s[strIndex+1] >= 'a' && s[strIndex+1] <= 'z') || (s[strIndex+1] >= 'A' && s[strIndex+1] <= 'Z') ||
          (s[strIndex+1] >= '0' && s[strIndex+1] <= '9') || s[strIndex+1] == '_' || s[strIndex+1] == '-' || s[strIndex+1] == ':'
        )
      ) {
      memset(nameBuffer, 0, sizeof(nameBuffer));
      memset(cssBuffer, 0, sizeof(cssBuffer));
      nameIndex = 0;
      cssIndex = 0;
      state = STATE_NAME;
    }
    strIndex++;
  }

  return vector;
}