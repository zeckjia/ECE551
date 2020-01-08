// This file is for Step 1.
// You should do
//  Step 1 (A): write seq1
//  Step 1 (B): write main to test seq1
//  Step 1 (C): write printSeq1Range
//  Step 1 (D): add test cases to main to test printSeq1Range
//
// Be sure to #include any header files you need!

#include <stdio.h>
#include <stdlib.h>

int seq1(int x) {
  // check if the output will be overflow
  if (x > 536870912) {
    printf("Output is overflow!\n");
    exit(EXIT_FAILURE);
  }
  // check if output will be underflow
  else if (x < -536870911) {
    printf("Output is underflow!\n");
    exit(EXIT_FAILURE);
  }
  else {
    return (4 * x - 3);
  }
}

void printSeq1Range(int low, int high) {
  for (int i = low; i < high; i++) {
    printf("%d", seq1(i));
    if (i != high - 1) {  // if the last output has been reached, do not print ','
      printf(", ");
    }
  }
  printf("\n");
}

int main(void) {
  // test seq1
  printf("seq1(%d) = %d\n", 0, seq1(0));
  printf("seq1(%d) = %d\n", 1, seq1(1));
  printf("seq1(%d) = %d\n", -1, seq1(-1));
  printf("seq1(%d) = %d\n", 7, seq1(7));
  printf("seq1(%d) = %d\n", -10, seq1(-10));
  printf("seq1(%d) = %d\n", 1024, seq1(1024));
  printf("seq1(%d) = %d\n", -2048, seq1(-2048));
  printf("seq1(%d) = %d\n", 'a', seq1('a'));
  printf("seq1(%d) = %d\n", 536870912, seq1(536870912));
  printf("seq1(%d) = %d\n", -536870911, seq1(-536870911));

  // test printSeq1Range
  printf("printSeq1Range(%d, %d)\n", 0, 4);
  printSeq1Range(0, 4);
  printf("printSeq1Range(%d, %d)\n", -10, -7);
  printSeq1Range(-10, -7);
  printf("printSeq1Range(%d, %d)\n", -20, 10);
  printSeq1Range(-20, -10);
  printf("printSeq1Range(%d, %d)\n", 0, 0);
  printSeq1Range(0, 0);
  printf("printSeq1Range(%d, %d)\n", 5, 5);
  printSeq1Range(5, 5);
  printf("printSeq1Range(%d, %d)\n", 10, 4);
  printSeq1Range(10, 4);

  return EXIT_SUCCESS;
}
