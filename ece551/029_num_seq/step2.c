// This file is for Step 2.
// You should do
//  Step 2 (A): write seq2
//  Step 2 (B): write main to test seq2
//  Step 2 (C): write sumSeq2
//  Step 2 (D): add test cases to main to test sumSeq2
//
// Be sure to #include any header files you need!

#include <stdio.h>
#include <stdlib.h>

int seq2(int x) {
  if (x % 4 == 2 || x % 4 == -2) {
    // check if output will be underflow or overflow
    if (x > 1073741823 || x < -1073741823) {
      printf("Output is overflow or underflow!\n");
      exit(EXIT_FAILURE);
    }
    else {
      return x * 2;
    }
  }
  else {
    // check if output is overflow
    if (x < -46341 || x > 46339) {
      printf("Output is overflow!\n");
      exit(EXIT_FAILURE);
    }
    else {
      return x * (x + 3) + 1;
    }
  }
}

int sumSeq2(int low, int high) {
  int sum = 0;
  for (int i = low; i < high; i++) {
    sum += seq2(i);
  }
  return sum;
}

int main(void) {
  // test seq2
  printf("seq2(%d) = %d\n", 0, seq2(0));
  printf("seq2(%d) = %d\n", 1, seq2(1));
  printf("seq2(%d) = %d\n", 2, seq2(2));
  printf("seq2(%d) = %d\n", 3, seq2(3));
  printf("seq2(%d) = %d\n", 4, seq2(4));
  printf("seq2(%d) = %d\n", 20, seq2(20));
  printf("seq2(%d) = %d\n", 34, seq2(34));
  printf("seq2(%d) = %d\n", 1024, seq2(1024));
  printf("seq2(%d) = %d\n", -1, seq2(-1));
  printf("seq2(%d) = %d\n", -2, seq2(-2));
  printf("seq2(%d) = %d\n", -3, seq2(-3));
  printf("seq2(%d) = %d\n", -4, seq2(-4));
  printf("seq2(%d) = %d\n", -28, seq2(-28));
  printf("seq2(%d) = %d\n", -50, seq2(-50));
  printf("seq2(%d) = %d\n", -2047, seq2(-2047));
  printf("seq2(%d) = %d\n", 1073741822, seq2(1073741822));
  printf("seq2(%d) = %d\n", -1073741822, seq2(-1073741822));
  printf("seq2(%d) = %d\n", 46339, seq2(46339));
  printf("seq2(%d) = %d\n", -46341, seq2(-46341));
  printf("seq2(%d) = %d\n", 46342, seq2(46342));

  // test sumSeq2
  printf("sumSeq2(%d, %d) = %d\n", 0, 2, sumSeq2(0, 2));
  printf("sumSeq2(%d, %d) = %d\n", 3, 6, sumSeq2(3, 6));
  printf("sumSeq2(%d, %d) = %d\n", -10, -3, sumSeq2(-10, -3));
  printf("sumSeq2(%d, %d) = %d\n", -4, 4, sumSeq2(-4, 4));
  printf("sumSeq2(%d, %d) = %d\n", 0, 0, sumSeq2(0, 0));
  printf("sumSeq2(%d, %d) = %d\n", 9, 7, sumSeq2(9, 7));

  return EXIT_SUCCESS;
}
