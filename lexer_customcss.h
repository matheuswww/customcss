#pragma once
#include "lexer_classes.h"
#include "vector.h"
#include "uthash.h"

#define CSS_MAX 2048

typedef struct {
  char class_name[CLASS_NAME_MAX];
  char css[CSS_MAX];
  UT_hash_handle hh;
} CustomCSS;

typedef enum {
  STATE_NAME,
  STATE_EXPECT_CSS,
  STATE_CSS,
  STATE_IGNORE_CSS_VAL,
} LexerCustomCSS_State;

Vector* lex_customcss(char s[]);