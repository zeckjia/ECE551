#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAT_SIZE 10

void read_matrix(char matrix[MAT_SIZE][MAT_SIZE + 2], FILE * f) {
  char * ptr;
  // read characters to matrix (the 1st 10 lines)
  for (int i = 0; i < MAT_SIZE; i++) {
    // If EOF appears, file is too short
    if (fgets(matrix[i], MAT_SIZE + 2, f) == NULL) {
      fprintf(stderr, "Input is too short!\n");
      exit(EXIT_FAILURE);
    }
    else {
      ptr = strchr(matrix[i], '\n');
      // If '\n' cannot be found, line is too long
      if (ptr == NULL) {
        fprintf(stderr, "Input is too long!\n");
        exit(EXIT_FAILURE);
      }
      // If '\n' is not the 11th char, line is too short
      else if ((ptr - matrix[i]) < MAT_SIZE) {
        fprintf(stderr, "Input is too short!\n");
        exit(EXIT_FAILURE);
      }
    }
  }

  // If the 11th line is not EOF, file is too long
  char line[MAT_SIZE + 2];
  if (fgets(line, MAT_SIZE + 2, f) != NULL) {
    fprintf(stderr, "Input is too long!\n");
    exit(EXIT_FAILURE);
  }
}

void rotate_matrix(char matrix[MAT_SIZE][MAT_SIZE + 2]) {
  for (int j = 0; j < MAT_SIZE; j++) {
    for (int i = MAT_SIZE - 1; i >= 0; i--) {
      fprintf(stdout, "%c", matrix[i][j]);
    }
    fprintf(stdout, "\n");
  }
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: rotateMatrix inputFileName\n");
    return EXIT_FAILURE;
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open file!\n");
    return EXIT_FAILURE;
  }
  // read matrix
  char matrix[MAT_SIZE][MAT_SIZE + 2];
  read_matrix(matrix, f);

  // rotate matrix
  rotate_matrix(matrix);

  if (fclose(f) != 0) {
    perror("Failed to close the input file\n!");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
