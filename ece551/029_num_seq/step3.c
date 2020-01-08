// This file is for Step 3.
// You should do
//  Step 3 (A): write seq3
//  Step 3 (B): write main to test seq3
//  Step 3 (C): write countEvenInSeq3Range
//  Step 3 (D): add test cases to main to test countEvenInSeq3Range
//
// Be sure to #include any header files you need!

#include <stdio.h>
#include <stdlib.h>

int seq3(int x, int y) {
  return x * y - 3 * x + 2 * y;
}

int countEvenInSeq3Range(int xLow, int xHi, int yLow, int yHi) {
  int count = 0;
  for (int i = xLow; i < xHi; i++) {
    for (int j = yLow; j < yHi; j++) {
      if (seq3(i, j) % 2 == 0) {
        count += 1;
      }
    }
  }
  return count;
}

int main(void) {
  // test seq3
  printf("seq3(%d, %d) = %d\n", 0, 0, seq3(0, 0));
  printf("seq3(%d, %d) = %d\n", -5, -5, seq3(-5, -5));
  printf("seq3(%d, %d) = %d\n", 1, 3, seq3(1, 3));
  printf("seq3(%d, %d) = %d\n", 4, -2, seq3(4, -2));
  printf("seq3(%d, %d) = %d\n", -2, -3, seq3(-2, -3));
  printf("seq3(%d, %d) = %d\n", 100, 100, seq3(100, 100));
  printf("seq3(%d, %d) = %d\n", -20, 0, seq3(-20, 0));
  printf("seq3(%d, %d) = %d\n", 1024, 2048, seq3(1024, 2048));

  // test countEvenInSeq3Range
  printf("countEvenInSeq3Range(%d, %d, %d, %d) = %d\n",
         0,
         2,
         0,
         3,
         countEvenInSeq3Range(0, 2, 0, 3));
  printf("countEvenInSeq3Range(%d, %d, %d, %d) = %d\n",
         -5,
         6,
         -5,
         6,
         countEvenInSeq3Range(-5, 4, -5, 6));
  printf("countEvenInSeq3Range(%d, %d, %d, %d) = %d\n",
         0,
         0,
         0,
         3,
         countEvenInSeq3Range(0, 0, 0, 3));
  printf("countEvenInSeq3Range(%d, %d, %d, %d) = %d\n",
         0,
         -3,
         0,
         10,
         countEvenInSeq3Range(0, -3, 0, 10));
  printf("countEvenInSeq3Range(%d, %d, %d, %d) = %d\n",
         0,
         2,
         4,
         -10,
         countEvenInSeq3Range(0, 2, 4, -10));
  printf("countEvenInSeq3Range(%d, %d, %d, %d) = %d\n",
         -100,
         100,
         -100,
         100,
         countEvenInSeq3Range(-100, 100, -100, 100));
  printf("countEvenInSeq3Range(%d, %d, %d, %d) = %d\n",
         2048,
         1024,
         0,
         2000,
         countEvenInSeq3Range(2048, 1024, 0, 2000));
  printf("countEvenInSeq3Range(%d, %d, %d, %d) = %d\n",
         200,
         -198,
         300,
         -10,
         countEvenInSeq3Range(200, -198, 300, -10));

  return EXIT_SUCCESS;
}
