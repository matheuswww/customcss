#pragma once

#include "lexer_class.h"
#include "lexer_customcss.h"
#include "vector.h"
#include "uthash.h"

#define BUFFER_CSS_MAX 8192

void parser(Vector *customcss, Vector *classes, char *cssPath);
void replaceVal(char dst[CSS_MAX], char src[VALUE_MAX]);