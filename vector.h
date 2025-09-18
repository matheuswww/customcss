#pragma once
#define INIT 10;

typedef struct {
  int el_size;
  int capacity;
  int len;
  void *data;
} Vector;

Vector* vector_init(int);
void vector_push(Vector*, void*);
void *vector_get(Vector*, int);
void vector_free(Vector*);
void vector_clean(Vector*);