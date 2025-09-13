#include <lexer_class.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

void lexer(char s[]) {
  char currentToken[SEARCH_MAX] = {0};
  class classList[CLASSES_MAX] = {0};
  char target[] = "class";
  int tokenIndex = 0;
  int strIndex = 0;
  int classCount = 0;

  while (s[strIndex] != '\0') {
    if (
        tokenIndex == SEARCH_MAX ||
        s[strIndex] == '<'  || s[strIndex] == '>'  || s[strIndex] == '/'  ||
        s[strIndex] == '='  || s[strIndex] == '"'  || s[strIndex] == '\'' ||
        s[strIndex] == '\n' || s[strIndex] == '\t' || s[strIndex] == '\r' ||
        s[strIndex] == '\t' || s[strIndex] == '\f' || s[strIndex] == ' '
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
        int bufIndex = 0;
        bool readingName = false;
        bool readingValue = false;
        while (s[strIndex] != '\0') {
          if ((bufIndex > NAME_MAX && readingName) || (bufIndex > VALUE_MAX && readingValue)) {
            break;
          }
          strIndex++;
          if(
            ( (s[strIndex] >= 'a' && s[strIndex] <= 'z') ||
              (s[strIndex] >= 'A' && s[strIndex] <= 'Z') ||
              (s[strIndex] >= '0' && s[strIndex] <= '9') ||
              s[strIndex] == '_' || s[strIndex] == '[' || s[strIndex] == ']' ||
              (s[strIndex] == '-' && readingName) )
            ) {
              if (s[strIndex] == '-') {
                nameBuffer[bufIndex] = '\0';
                readingValue = true;
                readingName = false;
                bufIndex = 0;
                continue;
              } else if (!readingValue) {
                if (!readingName) {
                  readingName = true;
                }
                nameBuffer[bufIndex] = s[strIndex];
              } else {
                valueBuffer[bufIndex] = s[strIndex];
              }
            } else {
              if (readingValue && bufIndex > 0) {
                valueBuffer[bufIndex] = '\0';
                readingValue = false;
                class *newClass = (class*)malloc(sizeof(class));
                memmove(newClass->name, nameBuffer, NAME_MAX);
                memmove(newClass->val, valueBuffer, VALUE_MAX);
                classList[classCount] = *newClass;
                classCount++;
                if (classCount > (CLASSES_MAX / sizeof(class))) {
                  break;
                }
              }
              readingValue = false;
              readingName = false;
              bufIndex = 0;
              memset(nameBuffer, 0, sizeof(NAME_MAX));
              if (s[strIndex] == '"' || s[strIndex] == '\'') {
                break;
              }
              continue;
            }
            bufIndex++;
        }
      }
    }
    strIndex++;
    tokenIndex++;
  }

  for (int i = 0; i < classCount; i++) {
    printf("name: %s\n", classList[i].name);
    printf("val: %s\n", classList[i].val);
  } 
}