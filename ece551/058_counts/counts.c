#include "counts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

counts_t * createCounts(void) {
  //WRITE ME
  counts_t * counts = malloc(sizeof(*counts));
  counts->countArray = NULL;
  counts->arraySz = 0;
  counts->unknownNum = 0;
  return counts;
}

void addArray(counts_t * c, const char * name) {
  c->countArray = realloc(c->countArray, (c->arraySz + 1) * sizeof(*c->countArray));
  c->countArray[c->arraySz] = malloc(sizeof(*c->countArray[c->arraySz]));

  // compute length of name
  int nameLen = 0;
  while (name[nameLen] != '\0') {
    nameLen++;
  }
  nameLen++;  // '\0' at the end of string

  c->countArray[c->arraySz]->num = 1;
  c->countArray[c->arraySz]->value =
      malloc(nameLen * sizeof(*c->countArray[c->arraySz]->value));
  for (int i = 0; i < nameLen; i++) {
    c->countArray[c->arraySz]->value[i] = name[i];
  }

  c->arraySz++;
}

void addCount(counts_t * c, const char * name) {
  //WRITE ME
  if (name == NULL) {
    c->unknownNum++;
  }
  else if (c->arraySz == 0) {
    addArray(c, name);
  }
  else {
    for (int i = 0; i < c->arraySz; i++) {
      if (strcmp(name, c->countArray[i]->value) == 0) {
        c->countArray[i]->num++;
        break;
      }
      else if (i == c->arraySz - 1) {
        addArray(c, name);
        break;
      }
    }
  }
}

void printCounts(counts_t * c, FILE * outFile) {
  //WRITE ME
  for (int i = 0; i < c->arraySz; i++) {
    fprintf(outFile, "%s: %d\n", c->countArray[i]->value, c->countArray[i]->num);
  }
  if (c->unknownNum != 0) {
    fprintf(outFile, "<unknown> : %d\n", c->unknownNum);
  }
}

void freeCounts(counts_t * c) {
  //WRITE ME
  for (int i = 0; i < c->arraySz; i++) {
    free(c->countArray[i]->value);
    free(c->countArray[i]);
  }
  free(c->countArray);
  free(c);
}
