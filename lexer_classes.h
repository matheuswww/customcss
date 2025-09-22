#pragma once
#include <stdint.h>
#include "vector.h"

#define CLASSES_MAX 5000
#define SEARCH_CLASS_MAX 6
#define CLASS_NAME_MAX 100
#define CUSTOM_VALUE_MAX 100

typedef struct {
  char class_name[CLASS_NAME_MAX];
  char custom_val[CUSTOM_VALUE_MAX];
} Class;

typedef enum {
  STATE_CLASS,
  STATE_VAL,
  STATE_IGNORE_CLASS_VAL
} LexerClass_State;

Vector* lex_classes(char[]);