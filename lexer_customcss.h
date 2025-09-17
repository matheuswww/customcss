#pragma once
#include "lexer_class.h"

#define CSS_CLASS_MAX 8196
#define CSS_MAX 2048

void lexer_customcss(char s[]);

typedef struct {
  char class_name[NAME_MAX];
  char css[CSS_MAX];
} css;