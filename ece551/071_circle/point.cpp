#include "point.h"

#include <cmath>
#include <cstdio>

void Point::move(double dx, double dy) {
  x += dx;
  y += dy;
}

double Point::distanceFrom(const Point & p) {
  double distance;
  distance = sqrt((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y));
  return distance;
}
