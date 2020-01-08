#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int * array, size_t n);

void runcheck(int * array, size_t n, size_t n_maxSeq) {
  if (maxSeq(array, n) != n_maxSeq) {
    exit(EXIT_FAILURE);
  }
}

int main(void) {
  int array1[] = {-1, 0};
  runcheck(array1, 2, 2);
  int array2[] = {1, 2, 1, 3, 5, 7, 2, 4, 6, 9};
  runcheck(array2, 10, 4);
  int array3[] = {0, 0};
  runcheck(array3, 2, 1);
  int array4[] = {1};
  runcheck(array4, 0, 0);

  return EXIT_SUCCESS;
}
