#include <stdio.h>
#include <stdlib.h>

unsigned power(unsigned x, unsigned y);

void run_check(unsigned x, unsigned y, unsigned expected_ans) {
  if (power(x, y) != expected_ans) {
    exit(EXIT_FAILURE);
  }
}

int main(void) {
  run_check(2, 2, 4);
  run_check(100, 2, 10000);
  run_check(0, 0, 1);
  run_check(-1, 1, -1);

  return EXIT_SUCCESS;
}
