#include "election.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_UINT32 4294967295

/* This function is used for copying a string.
 * It takes three parameters:
 *   p_head (points at the first char of the original string)
 *   p_tail (points at the char after the end of the original string)
 *   str (where the string will be copied to) */
void copyStr(const char * p_head, const char * p_tail, char * str) {
  // check string length
  if ((p_tail - p_head) >= MAX_STATE_NAME_LENGTH) {
    fprintf(stderr, "'state name' is too long!\n");
    exit(EXIT_FAILURE);
  }
  // copy string
  while (p_head != p_tail) {
    *str = *p_head;
    p_head++;
    str++;
  }
  *str = '\0';
}

/* This function is used for converting a digit string to an unsigned int.
 * It takes three parameters:
 *   p_head (points at the first digit)
 *   p_tail (points at the char after the last digit)
 *   bit_info (bit number of the desired output integer, 32 or 64) */
uint64_t str2uint(const char * p_head, const char * p_tail, int bit_info) {
  uint64_t num = 0;
  while (p_head != p_tail) {
    if ((*p_head) > '9' || (*p_head) < '0') {
      fprintf(stderr,
              "Improper format of input string: 'population' and 'electoral votes' can "
              "only contain numbers!\n");
      exit(EXIT_FAILURE);
    }
    num = num * 10 + *p_head - '0';
    p_head++;
  }
  // check overflow for 32 bits unsigned int
  if (bit_info == 32 && num > MAX_UINT32) {
    fprintf(stderr, "'electoral votes' is overflow!\n");
    exit(EXIT_FAILURE);
  }
  return num;
}

/* This function is used for calculating ratio of the votes that Candidate A received.
 * It takes two parameters:
 *   votes (number of votes A received in this state)
 *   population (population of this state) */
float calRatio(uint64_t votes, uint64_t population) {
  // demominator cannot be zero
  if (population == 0) {
    fprintf(stderr,
            "Could not calculate the ratio of the votes that Candidate A received since "
            "population is 0!\n");
  }

  float ratio = votes * 1.0 / population;  // change int to float by * 1.0
  if (ratio > 1) {
    fprintf(stderr, "Candidate A could not receive more than %d%% of the votes!\n", 100);
    exit(EXIT_FAILURE);
  }
  return ratio;
}

/* This function is used for getting the name, population, and electoral 
 * votes of a state from the input string.
 * It takes one paramerer
 *   line (whose format should be "name:population:electoral votes") */
state_t parseLine(const char * line) {
  if (line == NULL) {
    fprintf(stderr, "Input string is NULL!\n");
    exit(EXIT_FAILURE);
  }

  const char * p_start = line;
  const char * p1 = strchr(line, ':');   // the fisrt ":"
  const char * p2 = strrchr(line, ':');  // the last ":"
  const char * p_end = strchr(line, '\0');

  // no ':' in the string || only one ":" in the string || nothing between two ":"s || nothing after the last ":"
  if (p1 == NULL || p1 == p2 || p1 + 1 == p2 || p2 + 1 == p_end) {
    fprintf(stderr, "Improper format of input string: name:population:electoral votes\n");
    exit(EXIT_FAILURE);
  }

  state_t state;
  copyStr(p_start, p1, state.name);                    // get the name
  state.population = str2uint(p1 + 1, p2, 64);         // get the population
  state.electoralVotes = str2uint(p2 + 1, p_end, 32);  // get the electoral votes

  return state;
}

/* This function is used for counting the total electoral votes that Candidate A received.
 * It takes three parameters:
 *   stateData (states information getting from parseLine)
 *   voteCounts (vote counts for "Candidate A" in the election)
 *   nStates (number of elements in the each array above) */
unsigned int countElectoralVotes(state_t * stateData,
                                 uint64_t * voteCounts,
                                 size_t nStates) {
  unsigned int count = 0;

  for (size_t i = 0; i < nStates; i++) {
    float ratio = calRatio(*(voteCounts + i), (stateData + i)->population);

    if (ratio > 0.5) {
      count = count + (stateData + i)->electoralVotes;
    }
  }

  return count;
}

/* This function is used for printing out the states which require a recount.
 * Parameters are the same as countElectoralvotes' */
void printRecounts(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  for (size_t i = 0; i < nStates; i++) {
    float ratio = calRatio(*(voteCounts + i), (stateData + i)->population);

    if (ratio <= 0.505 && ratio >= 0.495) {
      printf("%s requires a recount (Candidate A has %.2f%% of the vote)\n",
             (stateData + i)->name,
             ratio * 100);
    }
  }
}

/* This function is used for finding out in which state Candidate A won the 
 * largest percentage of the vote.
 * Parameters are the same as countElectoralvotes' */
void printLargestWin(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  float ratio[nStates];
  for (size_t i = 0; i < nStates; i++) {
    ratio[i] = calRatio(*(voteCounts + i), (stateData + i)->population);
  }

  float * pRatio = ratio;
  state_t * pState = stateData;

  // find the largest value in ratio[]
  for (size_t i = 1; i < nStates; i++) {
    if (ratio[i] > *pRatio) {
      pRatio = ratio + i;
      pState = stateData + i;
    }
  }

  printf("Candidate A won %s with %.2f%% of the vote\n", pState->name, *pRatio * 100);
}
