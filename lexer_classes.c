#include <lexer_classes.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "vector.h"

void saveClass(Vector* vector, char classNameBuffer[CLASS_NAME_MAX], char customValueBuffer[CUSTOM_VALUE_MAX]);
void lex(char* s, int* strIndex, Vector* vector);
bool getFirstCharClass(char* s, int* strIndex);

Vector* lex_classes(char s[]) {
  char searchTarget[SEARCH_CLASS_MAX] = {0};
  char *target = "class";
  int searchTargetIndex = 0;
  int strIndex = 0;
  Vector* vector = vector_init(sizeof(Class));
  while (s[strIndex] != '\0') {
    char c = s[strIndex];
    
    if (
      searchTargetIndex == SEARCH_CLASS_MAX ||
      (c != 'c' && c != 'l' && c != 'a' && c != 's')
    ) {
      if (
        (s[strIndex+1] == target[0])
      ) {
        searchTargetIndex = 0;
        memset(searchTarget, 0, SEARCH_CLASS_MAX);
      }
      strIndex++;
      continue;
    }
    searchTarget[searchTargetIndex] = c;
    searchTarget[searchTargetIndex+1] = '\0';

    if (strcmp(searchTarget, target) == 0) {
      if (getFirstCharClass(s, &strIndex)) {
        lex(s, &strIndex, vector);
      }
    }
    strIndex++;
    searchTargetIndex++;
  }

  return vector;
}

void lex(char* s, int* strIndex, Vector* vector) {
  char classNameBuffer[CLASS_NAME_MAX];
  char customValueBuffer[CUSTOM_VALUE_MAX];
  int bufIndex = 0;
  LexerClass_State state = STATE_CLASS;
  
  classNameBuffer[0] = '\0';
  customValueBuffer[0] = '\0';

  while (s[*strIndex] != '\0') {
    (*strIndex)++;                     
    char c = s[*strIndex];

    switch (state) {
      case STATE_CLASS:
        if(
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c == '[' && *strIndex > 0 && s[*strIndex-1] == '-') ||
            (c >= '0' && c <= '9') ||
            (c == '_') || (c == '-' && s[*strIndex+1] != '-')
            ) {
              if (c == '[') {
                classNameBuffer[bufIndex] = '\0';
                bufIndex = 0;
                state = STATE_VAL;
              } else if (s[*strIndex+1] != '[') {
                if (bufIndex >= CLASS_NAME_MAX - 1) {
                  printf("limit memory exceed\n");
                  exit(EXIT_FAILURE);
                }
                classNameBuffer[bufIndex++] = c;
              }
            } else {
              if (classNameBuffer[0] != '\0') {
                saveClass(vector, classNameBuffer, customValueBuffer);
                bufIndex = 0;
              }
              state = STATE_IGNORE_CLASS_VAL;
            }
          break;
    
      case STATE_VAL:
        if(
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c == ']') ||
            (c >= '0' && c <= '9') ||
            (c == '_') || (c == '-')
          ) {
            if (c == ']') {
              customValueBuffer[bufIndex] = '\0';
              saveClass(vector, classNameBuffer, customValueBuffer);
              bufIndex = 0;
              state = STATE_CLASS;
              continue;
            } else if (bufIndex <= CLASS_NAME_MAX - 1) {
              if (bufIndex >= CLASS_NAME_MAX - 1) {
                fprintf(stderr,
                "Error: class name buffer memory limit exceeded max %d\n", CLASS_NAME_MAX - 1);
                exit(EXIT_FAILURE);
              }
              customValueBuffer[bufIndex++] = c;
            }
            break;
          } else {
            state = STATE_IGNORE_CLASS_VAL;
          }
        break;
      default:
        break;
      }

      if (state == STATE_IGNORE_CLASS_VAL) {
        if (
          !((s[*strIndex+1] >= 'a' && s[*strIndex+1] <= 'z') ||
          (s[*strIndex+1] >= 'A' && s[*strIndex+1] <= 'Z') ||
          (s[*strIndex+1] == '[') || (s[*strIndex+1] == ']') ||
          (s[*strIndex+1] >= '0' && s[*strIndex+1] <= '9') ||
          (s[*strIndex+1] == '_') || (s[*strIndex+1] == '-'))
        ) {                       
          bufIndex = 0;
          memset(classNameBuffer, 0, CLASS_NAME_MAX);
          memset(customValueBuffer, 0, CUSTOM_VALUE_MAX);
          customValueBuffer[0] = '\0';
          classNameBuffer[0] = '\0';
        }
        state = STATE_CLASS;
      }

      if (c == '"' || c == '\'') {
        break;
      }
    }
}

bool getFirstCharClass(char* s, int* strIndex) {
  bool found = false;
  char c = s[*strIndex];
  while (c != '\0') {
    (*strIndex)++;
    c = s[*strIndex];
    if (
      c != ' ' && c != '"' && c != '\'' &&
      c != '='
    ) {
      break;
    }
    if (!found && (c == '"' || c == '\'')) {
      found = true;
      break;
    }
  }

  return found;
}

void saveClass(Vector* vector, char classNameBuffer[CLASS_NAME_MAX], char customValueBuffer[CUSTOM_VALUE_MAX]) {
  Class *class = malloc(sizeof(Class));
  if (!class) {
    perror("malloc failed!");
    exit(EXIT_FAILURE);
  }

  if (vector->len > CLASSES_MAX) {
    fprintf(stderr,
    "Error: Classes vector memory limit exceeded "
    "(%d elements, max allowed %d)\n",vector->len, CLASSES_MAX);
    exit(EXIT_FAILURE);
  }
  strcpy(class->class_name, classNameBuffer);
  strcpy(class->custom_val, customValueBuffer);
  vector_push(vector, class);
  memset(classNameBuffer, 0, CLASS_NAME_MAX);
  memset(customValueBuffer, 0, CUSTOM_VALUE_MAX);
  customValueBuffer[0] = '\0';
  classNameBuffer[0] = '\0';
}