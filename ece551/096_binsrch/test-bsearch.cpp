#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "function.h"

int binarySearchForZero(Function<int, int> * f, int low, int high);

class CountedIntFn : public Function<int, int> {
 protected:
  unsigned remaining;
  Function<int, int> * f;
  const char * mesg;

 public:
  CountedIntFn(unsigned n, Function<int, int> * fn, const char * m) :
      remaining(n),
      f(fn),
      mesg(m) {}
  virtual int invoke(int arg) {
    if (remaining == 0) {
      fprintf(stderr, "Too many function invocations in %s\n", mesg);
      exit(EXIT_FAILURE);
    }
    remaining--;
    return f->invoke(arg);
  }
};

void check(Function<int, int> * f,
           int low,
           int high,
           int expected_ans,
           const char * mesg) {
  int maxInvoke;
  if (high > low) {
    maxInvoke = log2(high - low) + 1;
  }
  else {
    maxInvoke = 1;
  }

  CountedIntFn * count = new CountedIntFn(maxInvoke, f, mesg);
  int ans = binarySearchForZero(count, low, high);
  assert(ans == expected_ans);
}

// functions
class LinearFunction : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return arg; }
};

class SinFunction : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return 10000000 * (sin(arg / 100000.0) - 0.5); }
};

int main(void) {
  LinearFunction * fLinear = new LinearFunction;
  check(fLinear, -100, 100, 0, "Linear Function 1");
  check(fLinear, 100, 101, 100, "Linear Function 2");
  check(fLinear, -100, -1, -2, "Linear Function 3");
  check(fLinear, 91, 91, 91, "Linear Function 4");
  check(fLinear, 0, 0, 0, "Linear Function 5");
  check(fLinear, -56, -56, -56, "Linear Function 6");
  check(fLinear, -1, 1000000000, 0, "Linear Function 7");

  SinFunction * fsin = new SinFunction;
  check(fsin, 0, 150000, 52359, "Sin Function 1");
  check(fsin, 0, 52359, 52358, "Sin Function 2");
  check(fsin, 52378, 160000, 52378, "Sin Function 3");
  check(fsin, 150000, 150000, 150000, "Sin Function 4");
  check(fsin, 0, 0, 0, "Sin Function 5");
  check(fsin, -999, -999, -999, "Sin Function 6");

  return EXIT_SUCCESS;
}
