#include "kv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

kvarray_t * readKVs(const char * fname) {
  //WRITE ME
  FILE * f = fopen(fname, "r");
  if (f == NULL) {
    fprintf(stderr, "Could not open file!\n");
    exit(EXIT_FAILURE);
  }
  kvarray_t * pairs = malloc(sizeof(*pairs));
  pairs->array = NULL;
  pairs->size = 0;

  char * line = NULL;
  size_t sz = 0;
  char * ptr_equ;
  char * ptr_n;
  while (getline(&line, &sz, f) >= 0) {
    ptr_equ = strchr(line, '=');
    if (ptr_equ == NULL) {
      fprintf(stderr, "Input file should have '=' on each line!\n");
      exit(EXIT_FAILURE);
    }
    ptr_n = strchr(line, '\n');

    kvpair_t * kvpair = malloc(sizeof(*kvpair));
    kvpair->key = NULL;
    kvpair->value = NULL;
    for (int i = 0; i <= ptr_equ - line; i++) {
      kvpair->key = realloc(kvpair->key, (i + 1) * sizeof(*kvpair->key));
      if (i == ptr_equ - line) {
        kvpair->key[i] = '\0';
        break;
      }
      kvpair->key[i] = line[i];
    }
    for (int i = 0; i <= ptr_n - ptr_equ - 1; i++) {
      kvpair->value = realloc(kvpair->value, (i + 1) * sizeof(*kvpair->value));
      if (i == ptr_n - ptr_equ - 1) {
        kvpair->value[i] = '\0';
        break;
      }
      kvpair->value[i] = line[i + ptr_equ - line + 1];
    }

    pairs->array = realloc(pairs->array, (pairs->size + 1) * sizeof(*pairs->array));
    pairs->array[pairs->size] = kvpair;
    pairs->size += 1;
  }
  free(line);

  if (fclose(f) != 0) {
    fprintf(stderr, "Fail to close the file!\n");
    exit(EXIT_FAILURE);
  }

  return pairs;
}

void freeKVs(kvarray_t * pairs) {
  //WRITE ME
  for (int i = 0; i < pairs->size; i++) {
    free(pairs->array[i]->key);
    free(pairs->array[i]->value);
    free(pairs->array[i]);
  }
  free(pairs->array);
  free(pairs);
}

void printKVs(kvarray_t * pairs) {
  //WRITE ME
  for (int i = 0; i < pairs->size; i++) {
    fprintf(stdout,
            "key = '%s' value = '%s'\n",
            pairs->array[i]->key,
            pairs->array[i]->value);
  }
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  //WRITE ME
  for (int i = 0; i < pairs->size; i++) {
    if (strcmp(pairs->array[i]->key, key) == 0) {
      return pairs->array[i]->value;
    }
  }
  return NULL;
}
