#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int * array, size_t n) {
  size_t n_maxSeq;
  size_t n_temp;
  int * p;

  if (n == 0 || n == 1) {
    n_maxSeq = n;
  }
  else {
    n_maxSeq = 1;
    n_temp = 1;
    p = array + 1;
    while (p <= array + n - 1) {
      if (*p > *(p - 1)) {
        n_temp++;
      }
      else {
        n_temp = 1;
      }
      if (n_temp > n_maxSeq) {
        n_maxSeq = n_temp;
      }
      p++;
    }
  }
  return n_maxSeq;
}
