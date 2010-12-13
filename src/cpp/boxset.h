#ifndef _boxset_h
#define _boxset_h

#include <iostream>
#include <vector>
#include "point.h"
#include "box.h"

using namespace std;

class BoxSet
{
  vector<Box> boxes;

public:
  BoxSet() {};
  BoxSet(int size) : boxes(size) {};

  void set_box(int i, const Box &b) { boxes[i] = b; }
  Box get_box(int i) const { return boxes[i]; }
  int size() const { return boxes.size(); }
  int dim() const { return ((size()==0) ? 0 : boxes[0].size()); }
};

class UniformBoxSet
{
  vector<Point > corners;
  Point width;
public:
  
  UniformBoxSet() {};
  UniformBoxSet(int size) : corners(size) {};
  UniformBoxSet(const Point &w) : width(w) {};
  UniformBoxSet(int size, const Point &w) : corners(size), width(w) {};
  UniformBoxSet(const Point &w, const vector<Point > &c) : corners(c), width(w) {};

  void set_width(const Point &w) { width = w; }
  Point get_width() const { return width; }
  void set_corner(int i, const Point &v) {	corners[i] = v; }
  Point get_corner(int i) const { return corners[i]; }
  Box get_box(int i) const { return Box(corners[i],width); }
  int size() const { return corners.size(); }
  int dim() const { return width.size(); }
};


ostream &operator<<(ostream &output, const BoxSet &bs);
ostream &operator<<(ostream &output, const UniformBoxSet &ubs);

#endif
