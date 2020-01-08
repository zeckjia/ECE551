#include <cstdio>

#include "point.h"

class Circle {
 private:
  Point center;
  double radius;

 public:
  Circle(Point init_c, double init_r);
  void move(double dx, double dy);
  double intersectionArea(const Circle & otherCircle);
};
