#include <lexer_class.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "vector.h"

Vector* lexer_class(char s[]) {
  Vector* vector = vector_init(sizeof(Class));
  char currentToken[SEARCH_MAX] = {0};
  char target[] = "class";
  int tokenIndex = 0;
  int strIndex = 0;

  while (s[strIndex] != '\0') {
    if (
        tokenIndex == SEARCH_MAX ||
        s[strIndex] == '<'  || s[strIndex] == '>'  || s[strIndex] == '/'  ||
        s[strIndex] == '='  || s[strIndex] == '"'  || s[strIndex] == '\'' ||
        s[strIndex] == '\n' || s[strIndex] == '\t' || s[strIndex] == '\r' ||
        s[strIndex] == '\f' || s[strIndex] == ' '
    ) {
      strIndex++;
      tokenIndex = 0;
      memset(currentToken, 0, sizeof(currentToken));
      continue;
    }
    currentToken[tokenIndex] = s[strIndex];
    currentToken[tokenIndex+1] = '\0';
    if (strcmp(currentToken, target) == 0) {
      tokenIndex = 0;
      bool foundClassAttr = false;
      while (s[strIndex] != '\0') {
        strIndex++;
        if (
          s[strIndex] != ' ' && s[strIndex] != '"' && s[strIndex] != '\'' &&
          s[strIndex] != '='
        ) {
          break;
        }
        if (!foundClassAttr && (s[strIndex] == '"' || s[strIndex] == '\'')) {
          foundClassAttr = true;
        }
        if (foundClassAttr) {
          break;
        }
      }
      if (foundClassAttr) {
        char nameBuffer[NAME_MAX];
        char valueBuffer[VALUE_MAX];
        valueBuffer[0] = '\0';
        nameBuffer[0] = '\0';
        int bufIndex = 0;
        bool readingName = false;
        bool readingVal = false;
        bool openBracket = false;
        bool closeBracket = false;
        bool ignoreCurrentString = false;
        while (s[strIndex] != '\0') {
          if ((bufIndex > NAME_MAX) || (bufIndex > VALUE_MAX)) {
            break;
          }
          strIndex++;

          if(
            !ignoreCurrentString && (
            (s[strIndex] >= 'a' && s[strIndex] <= 'z') ||
            (s[strIndex] >= 'A' && s[strIndex] <= 'Z') ||
            (s[strIndex] == '[') || (s[strIndex] == ']') ||
            (s[strIndex] >= '0' && s[strIndex] <= '9') ||
            (s[strIndex] == '_') || (s[strIndex] == '-') ||
            ( (openBracket && !closeBracket) && 
              (
                s[strIndex] == '\n' || s[strIndex] == '\r' || 
                s[strIndex] == '\t' || s[strIndex] == '\f' || 
                s[strIndex] == ' ' 
              )
            )
            )
            ) {
              if (
                (s[strIndex] == '-' && (readingName && s[strIndex+1] == '-')) || 
                ((s[strIndex] == '[' && ((openBracket || !readingName) || s[strIndex-1] != '-')) ||
                (s[strIndex] == ']' && (!openBracket || !readingVal)))
                ) {
                readingVal = false;
                readingName = false;
                ignoreCurrentString = true;
              } else if (s[strIndex] == '[') {
                nameBuffer[bufIndex-1] = 0;
                nameBuffer[bufIndex] = '\0';
                openBracket = true;
                readingVal = true;
                readingName = false;
                bufIndex = 0;
                continue;
              } else if (s[strIndex] == ']') {
                valueBuffer[bufIndex] = '\0';
                closeBracket = true;
                continue;
              } else {
                if (!readingName && !readingVal) {
                  readingName = true;
                }
                if (readingName) {
                  nameBuffer[bufIndex] = s[strIndex];
                  bufIndex++;
                }
                if(readingVal) {
                  valueBuffer[bufIndex] = s[strIndex];
                  bufIndex++;
                }
                continue;
              }
            }

            if (
              s[strIndex] != '\n' && s[strIndex] != '\t' && s[strIndex] != '\r' &&
              s[strIndex] != '\t' && s[strIndex] != '\f' && s[strIndex] != ' ' &&
              s[strIndex] != '"' && s[strIndex] != '\'' && s[strIndex] != '\0'
            ) {
              ignoreCurrentString = true;
            } else {
              if (ignoreCurrentString) {
                ignoreCurrentString = false;
              }
            }
            if ((readingName || readingVal) && !ignoreCurrentString && (openBracket == closeBracket)) {
              if (readingName) {
                if (bufIndex > 0 && nameBuffer[bufIndex-1] == '-') {
                  nameBuffer[bufIndex-1] = 0;
                  bufIndex-=1;
                }
                nameBuffer[bufIndex] = '\0';
              }
              Class *class = malloc(sizeof(Class));
               if (!class) {
                perror("malloc failed!");
                exit(EXIT_FAILURE);
              }
              strcpy(class->name, nameBuffer);
              strcpy(class->val, valueBuffer);
              vector_push(vector, class);
              if (vector->len > (CLASSES_MAX / sizeof(class))) {
                break;
              }
            }
            bufIndex = 0;
            openBracket = false;
            readingName = false;
            closeBracket = false;
            readingVal = false;
            if (nameBuffer[bufIndex] != '\0') {
              memset(nameBuffer, 0, sizeof(NAME_MAX));
            }
            if (valueBuffer[bufIndex] != '\0') {
              memset(valueBuffer, 0, sizeof(VALUE_MAX));
            }
            if (s[strIndex] == '"' || s[strIndex] == '\'') {
              break;
          }
        }
      }
    }
    strIndex++;
    tokenIndex++;
  }

  return vector;
}