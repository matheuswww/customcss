#pragma once
#include <stdint.h>

#define CLASSES_MAX 4096
#define SEARCH_MAX 6
#define NAME_MAX 100
#define VALUE_MAX 100

typedef struct {
  char name[NAME_MAX];
  char val[VALUE_MAX];
} class;

void lexer_class(char[]);