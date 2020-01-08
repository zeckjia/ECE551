#include "outname.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * computeOutputFileName(const char * inputName) {
  //WRITE ME
  char * ptr = strchr(inputName, '\0');
  if (ptr == NULL) {
    fprintf(stderr, "'inputName' illegal!\n");
    exit(EXIT_FAILURE);
  }
  int len_input = ptr - inputName;
  char * nameTail = ".counts";
  int len_tail = 8;  // '.counts' plus '\0' are 8 bytes.
  char * outputName = malloc((len_input + len_tail) * sizeof(outputName));
  for (int i = 0; i < len_input; i++) {
    outputName[i] = inputName[i];
  }
  for (int i = 0; i < len_tail; i++) {
    outputName[i + len_input] = nameTail[i];
  }
  return outputName;
}
