#include "vector.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

Vector* vector_init(int el_sise) {
  Vector* v = malloc(sizeof(Vector));
  v->len = 0;
  v->capacity = INIT;
  v->el_size = el_sise;
  v->data = malloc(v->capacity*v->el_size);
  if (!v->data) {
    perror("malloc failed!");
    exit(EXIT_FAILURE);
  }
  return v;
}

void vector_push(Vector* v, void *d) {
  if (v->len >= v->capacity) {
    v->capacity*=2;
    void *tmp = realloc(v->data, v->capacity*v->el_size);
    if (!tmp) {
      perror("malloc failed!");
      exit(EXIT_FAILURE);
    }
    v->data = tmp;
  }
  memcpy((char*)v->data+v->len*v->el_size, d, v->el_size);
  v->len++;
}

void* vector_get(Vector* v, int i) {
  if (v->len == 0 || i >= v->len) {
    perror("invalid vector index");
    exit(EXIT_FAILURE);
  }
  return v->data+v->el_size*i;
}

void vector_clean(Vector* v) {
  free(v->data);
  v->len = 0;
  v->capacity = INIT;
  v->data = malloc(v->capacity * v->el_size);
  if (!v->data) {
    perror("malloc failed!");
    exit(EXIT_FAILURE);
  }
}

void vector_free(Vector* v) {
  free(v->data);
  free(v);
}