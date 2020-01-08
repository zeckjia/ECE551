#include <cstdio>
#include <cstdlib>

#include "function.h"

int binarySearchForZero(Function<int, int> * f, int low, int high) {
  if (low >= high || low == high - 1) {
    return low;
  }

  int curr = (high + low) / 2;
  int fvalue = f->invoke(curr);
  if (fvalue < 0) {
    return binarySearchForZero(f, curr, high);
  }
  else if (fvalue == 0) {
    return curr;
  }
  else {
    return binarySearchForZero(f, low, curr);
  }
}
