#ifndef _point_h
#define _point_h

#include <iostream>
#include <iomanip>
#include <vector>
#include "capd/capdAlglib.h"
#include "capd/intervals/Interval.h"

using namespace std;

template <class T>
class PointBase
{
  vector<T> v;
public:
  PointBase() {};
  PointBase(int size) : v(size) {};
  PointBase(int size, const T &t) : v(size,t) {};
  T& operator[] (int i) { return v[i]; };
  void set (int i, const T& t) { v[i]=t; };
  const T& operator[] (int i) const { return v[i]; };
  int size() const { return v.size(); };

  friend ostream& operator<< (ostream &out, const PointBase<T> &p)
  {
	out << p.v;
	return out;
  };
};

typedef PointBase<double> Point;
//typedef capd::intervals::Interval< double > DInterval;
typedef PointBase< capd::intervals::Interval< double > > IPoint;

#include "treeutil.h"			// for the vector operator<< 
#endif
