#include <string.h>

#include "function.hpp"

#define MAX_NAME_LEN 6  // max length of command name string

// help function to parse command, including deleting '\n' and comment, and get name and expression of command
char * parseCmmd(char * temp, char * name) {
  skipSpace(&temp);
  // delete \n
  char * pEnter = strchr(temp, '\n');
  if (pEnter != NULL) {
    *pEnter = '\0';
  }
  // delete comment
  char * pPond = strchr(temp, '#');
  if (pPond != NULL) {
    *pPond = '\0';
  }
  if (*temp == '\0') {
    throw std::exception();
  }
  // get name and expression of command
  char * expr = strchr(temp, ' ');
  if (expr == NULL) {
    std::cerr << "Improper input format!\n";
    throw std::exception();
  }
  if (expr - temp > MAX_NAME_LEN) {
    std::cerr << "No valid command name found!\n";
    throw std::exception();
  }
  strncpy(name, temp, expr - temp);

  return expr;
}

int main(void) {
  funSet fSet;

  char * line = NULL;
  size_t sz = 0;
  ssize_t len = 0;
  while ((len = getline(&line, &sz, stdin)) != -1) {
    char * temp = line;
    char name[MAX_NAME_LEN] = {'\0'};

    try {
      char * expr = parseCmmd(temp, name);  // get command name and expression
      if (!strcmp(name, "define")) {
        fSet.define(&expr);
      }
      else if (!strcmp(name, "test")) {
        fSet.test(&expr);
      }
      else if (!strcmp(name, "numint")) {
        fSet.numint(&expr);
      }
      else if (!strcmp(name, "mcint")) {
        fSet.mcint(&expr);
      }
      else if (!strcmp(name, "max")) {
        fSet.MaxMin(&expr, '+');
      }
      else if (!strcmp(name, "min")) {
        fSet.MaxMin(&expr, '-');
      }
      else {
        std::cerr << "No valid command name found!\n";
        continue;
      }
    }
    catch (...) {
      continue;
    }
  }
  free(line);

  return EXIT_SUCCESS;
}
