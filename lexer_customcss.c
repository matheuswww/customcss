#include "lexer_customcss.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void lexer_customcss(char s[]) {
  css *cssList = (css*)malloc(sizeof(css) * CSS_CLASS_MAX);
  char nameBuffer[NAME_MAX] = {0};
  char cssBuffer[CSS_MAX] = {0};
  int cssIndex = 0;
  int nameIndex = 0;
  int cssListIndex = 0;
  int strIndex = 0;
  bool ignoreCurrentString = false;
  bool openCurlyBracket = false;
  bool closeCurlyBracket = false;
  bool colon = false;

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
      memmove(cssList[cssListIndex].class_name, nameBuffer, NAME_MAX);
      memmove(cssList[cssListIndex].css, cssBuffer, CSS_MAX);
      cssListIndex++;
    }

    if (ignoreCurrentString || closeCurlyBracket) {
      colon = false;
      openCurlyBracket = false;
      ignoreCurrentString = false;
      nameIndex = 0;
      cssIndex = 0;
      closeCurlyBracket = false;
      memset(nameBuffer, 0, sizeof(NAME_MAX));
      memset(cssBuffer, 0, sizeof(CSS_CLASS_MAX));
    }

    strIndex++;
  }


  for (int i = 0; i < cssListIndex; i++) {
    printf("%s\n", cssList[i].class_name);
    printf("%s\n", cssList[i].css);
  }
}