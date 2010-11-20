#ifndef _box_h
#define _box_h

#include <iostream>
#include <vector>
#include <Interval.h>
#include "point.h"

using namespace std;

class Box
{
public:
  Point v;
  Point w;

  Box();
  Box(int dimension);
  Box(const Point &_v, const Point &_w);
  Box(const IPoint &i);
  operator IPoint();

  Box scale_down(int i) const;
  bool contains(const Point &_v) const;
  bool intersects(const Box &b) const;
  int child_hit(const Point &_v) const;
  vector<int> children_hit(const Box &b) const;
  Point get_corner(int corner) const;
  int size() const;

  // shorthand for the relevant points in the box
  double beg(int i) const;
  double mid(int i) const;
  double end(int i) const;
};

ostream &operator<<(ostream &output, const Box &b);

#endif
