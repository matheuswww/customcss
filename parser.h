#pragma once

#include "lexer_classes.h"
#include "lexer_customcss.h"
#include "vector.h"
#include "uthash.h"

#define BUFFER_CSS_MAX 8192
#define SEARCH_KEY_MAX 6

typedef enum {
  STATE_SEARCH_KEY,
  STATE_SAVE_BYTES_AFTER_KEY,
  STATE_INSERT_VALUE,
  STATE_IGNORE
} LexerVal_State;

void parse(Vector *customcss, Vector *classes, char *cssPath);