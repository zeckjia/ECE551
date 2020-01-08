#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//This function is used to figure out the ordering
//of the strings in qsort.  You do not need
//to modify it.
int stringOrder(const void * vp1, const void * vp2) {
  const char * const * p1 = vp1;
  const char * const * p2 = vp2;
  return strcmp(*p1, *p2);
}
//This function will sort and print data (whose length is count).
void sortData(char ** data, size_t count) {
  qsort(data, count, sizeof(char *), stringOrder);
}

//This function is used to read data from file f
char ** readData(char ** lines, size_t * i, FILE * f) {
  size_t sz = 0;
  char * curr = NULL;
  size_t count = 0;

  while (getline(&curr, &sz, f) >= 0) {
    lines = realloc(lines, (count + 1) * sizeof(*lines));
    lines[count] = curr;
    curr = NULL;
    count++;
  }
  free(curr);
  *i = count;
  return lines;
}

//This function will print out the sorted data
void printData(char ** lines, size_t count) {
  for (size_t j = 0; j < count; j++) {
    fprintf(stdout, "%s", lines[j]);
    free(lines[j]);
  }
  free(lines);
}

void read_sort_print(FILE * f) {
  char ** lines = NULL;
  size_t count = 0;

  if (f == NULL) {
    perror("Could not open file!\n");
    exit(EXIT_FAILURE);
  }

  lines = readData(lines, &count, f);
  sortData(lines, count);
  printData(lines, count);

  if (fclose(f) != 0) {
    perror("Fail to close the input file!\n");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char ** argv) {
  //WRITE YOUR CODE HERE!
  FILE * f;

  // No arguments
  if (argc == 1) {
    f = stdin;
    //    printf("Please input the strings you want to sort:\n");
    read_sort_print(f);
  }

  // With arguments
  else {
    for (int i = 1; i < argc; i++) {
      f = fopen(argv[i], "r");
      read_sort_print(f);
    }
  }
  return EXIT_SUCCESS;
}
