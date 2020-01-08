int max(int num1, int num2) {
  if (num1 > num2) {
    return num1;
  }
  else {
    return num2;
  }
}

// check if num is between num1 and num2
int between(int num, int num1, int num2) {
  if (num >= num1 && num < num2) {
    return 1;
  }
  else {
    return 0;
  }
}

// check if num is equal to either num1 or num2
int either(int num, int num1, int num2) {
  if (num == num1 || num == num2) {
    return 1;
  }
  else {
    return 0;
  }
}

// check if num1 is less than num2
int less_than(int num1, int num2) {
  if (num1 < num2) {
    return 1;
  }
  else {
    return 0;
  }
}

void squares(int size1, int x_offset, int y_offset, int size2) {
  //compute the max of size1 and (x_offset + size2).  Call this w
  int w = max(size1, x_offset + size2);
  //compute the max of size1 and (y_offset + size2).  Call this h
  int h = max(size1, y_offset + size2);
  //count from 0 to h. Call the number you count with y
  for (int y = 0; y < h; y++) {
    //count from 0 to w. Call the number you count with x
    for (int x = 0; x < w; x++) {
      //check if  EITHER
      //    ((x is between x_offset  and x_offset +size2) AND
      //     y is equal to either y_offset OR y_offset + size2 - 1 )
      //  OR
      //    ((y is between y_offset and y_offset + size2) AND
      //     x is equal to either x_offset OR x_offset + size2 -1)
      // if so, print a *
      if ((between(x, x_offset, x_offset + size2) &&
           either(y, y_offset, y_offset + size2 - 1)) ||
          (between(y, y_offset, y_offset + size2) &&
           either(x, x_offset, x_offset + size2 - 1))) {
        printf("*");
      }
      //if not,
      else {
        // check if EITHER
        //    x is less than size1 AND (y is either 0 or size1-1)
        // OR
        //    y is less than size1 AND (x is either 0 or size1-1)
        //if so, print a #
        if ((less_than(x, size1) && either(y, 0, size1 - 1)) ||
            (less_than(y, size1) && either(x, 0, size1 - 1))) {
          printf("#");
        }
        //else print a space
        else {
          printf(" ");
        }
      }
      //when you finish counting x from 0 to w,
      //print a newline
    }
    printf("\n");
  }
}
