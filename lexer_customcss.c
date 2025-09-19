#include "lexer_customcss.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

Vector* lexer_customcss(char s[]) {
  Vector* vector = vector_init(sizeof(CustomCSS));
  char nameBuffer[NAME_MAX] = {0};
  char cssBuffer[CSS_MAX] = {0};
  int cssIndex = 0;
  int nameIndex = 0;
  int strIndex = 0;
  bool ignoreCurrentString = false;
  bool openCurlyBracket = false;
  bool closeCurlyBracket = false;
  bool colon = false;
  nameBuffer[nameIndex] = '\0';
  cssBuffer[cssIndex] = '\0';

  while (s[strIndex] != '\0') {
    if (openCurlyBracket) {
      cssBuffer[cssIndex] = s[strIndex];
      cssIndex++;
    }
    if(
      (!openCurlyBracket || s[strIndex] == '}') &&
      (
        (s[strIndex] >= 'a' && s[strIndex] <= 'z') ||
        (s[strIndex] >= 'A' && s[strIndex] <= 'Z') ||
        (s[strIndex] >= '0' && s[strIndex] <= '9') ||
        (s[strIndex] == '_') || (s[strIndex] == '-') ||
        (s[strIndex] == '{' || s[strIndex] == '}') ||
        (s[strIndex] == ' ') ||
        (s[strIndex] == ':')
      )
    ) {
      switch (s[strIndex])
      {
        case '{':
          if (!colon) {
            ignoreCurrentString = true;
            break;
          }
          nameBuffer[nameIndex] = '\0'; 
          openCurlyBracket = true;
          continue;
        case '}':
          if (!openCurlyBracket) {
            ignoreCurrentString = true;
          }
          cssBuffer[cssIndex] = '\0';
          closeCurlyBracket = true;
          break;
        case ':':
          if (colon) {
            ignoreCurrentString = true; 
            break;
          }
          colon = true;
          break;
        case ' ':
          break;
        default:          
          nameBuffer[nameIndex] = s[strIndex];
          nameIndex++;
          break;
      }  
    }
    
    if (closeCurlyBracket) {
      CustomCSS* customCSS = malloc(sizeof(CustomCSS));
      if (!customCSS) {
        perror("malloc failed!");
        exit(EXIT_FAILURE);
      }
      strcpy(customCSS->class_name, nameBuffer);
      strcpy(customCSS->css, cssBuffer);
      vector_push(vector, customCSS);
    }

    if (ignoreCurrentString || closeCurlyBracket) {
      colon = false;
      openCurlyBracket = false;
      ignoreCurrentString = false;
      nameIndex = 0;
      cssIndex = 0;
      closeCurlyBracket = false;
      if (nameBuffer[nameIndex] != '\0') {
        memset(nameBuffer, 0, sizeof(NAME_MAX));
      }
      if (cssBuffer[cssIndex] != '\0') {
        memset(cssBuffer, 0, sizeof(CSS_CLASS_MAX));
      }
    }

    strIndex++;
  }

  return vector;
}