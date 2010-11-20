#ifndef _treeutil_h
#define _treeutil_h
#include <iostream>
#include <vector>
#include <math.h>
#include "point.h"

using namespace std;

inline Point halve_vector(Point v)
{
  for (int i=0; i<v.size(); i++)
	v[i] /= 2;
  return v;
}

inline bool interval_contains(double s, double t, double p)
{
  return ((s <= p) && (p <= t));
}

inline bool interval_intersect(double s, double t, double a, double b)
{
  return ((a <= t) && (s <= b));
}


template <class T>
ostream &operator<<(ostream &output, const vector<T> &v)
{
  output << "[";
  if (v.size() > 0)
  {
	for (int i=0; i<v.size()-1; i++)
	  output << v[i] << " ";
	output << v.back();
  }
  output << "]";
  return output;
}

#endif
