#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int * arrayMax(int * array, int n) {
  if (n == 0) {
    return NULL;
  }

  int * p = array;
  for (int i = 1; i < n; i++) {
    if (*(array + i) > *p) {
      p = (array + i);
    }
  }
  return p;
}

int max_count(int * arrayMax, int * array, int n) {
  if (n == 0) {
    return 0;
  }

  int num = 1;
  int * temp = array;
  for (int i = 0; i < n; i++) {
    if (temp != arrayMax && *temp == *arrayMax) {
      num++;
    }
    temp++;
  }
  return num;
}

void freq_count(FILE * f, int * letters) {
  int c;
  while ((c = fgetc(f)) != EOF) {
    if (isalpha(c)) {
      c = tolower(c);
      c -= 'a';
      *(letters + c) += 1;
    }
  }
}

void breaker(FILE * f) {
  int key;
  int letters[26] = {0};
  freq_count(f, letters);
  int * p_max = arrayMax(letters, 26);
  int max_num = max_count(p_max, letters, 26);
  if (max_num == 1) {
    key = (p_max - letters) - ('e' - 'a');
    if (key < 0) {
      key += 26;
    }
    fprintf(stdout, "%d\n", key);
  }
  else {
    fprintf(stderr, "Failed to get the key for the input file\n");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: breaker inputFileName\n");
    return EXIT_FAILURE;
  }

  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open file\n");
    return EXIT_FAILURE;
  }

  breaker(f);

  if (fclose(f) != 0) {
    perror("Failed to close the input file\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
