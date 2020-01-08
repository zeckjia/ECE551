#include "circle.h"

#include <cmath>
#include <cstdio>

using std::abs;

Circle::Circle(Point init_c, double init_r) : center(init_c), radius(init_r) {
}

void Circle::move(double dx, double dy) {
  center.move(dx, dy);
}

double Circle::intersectionArea(const Circle & otherCircle) {
  double area;
  double d = center.distanceFrom(otherCircle.center);
  if (d >= radius + otherCircle.radius) {
    area = 0;
  }
  else if (d <= abs(radius - otherCircle.radius)) {
    if (radius <= otherCircle.radius) {
      area = M_PI * radius * radius;
    }
    else {
      area = M_PI * otherCircle.radius * otherCircle.radius;
    }
  }
  else {
    double theta1 =
        acos((radius * radius + d * d - otherCircle.radius * otherCircle.radius) /
             (2 * d * radius));
    double theta2 =
        acos((-radius * radius + d * d + otherCircle.radius * otherCircle.radius) /
             (2 * d * otherCircle.radius));
    area = radius * radius * theta1 + otherCircle.radius * otherCircle.radius * theta2 -
           sin(theta1) * radius * d;
  }
  return area;
}
