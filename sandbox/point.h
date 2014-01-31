#ifndef _point_h
#define _point_h

#include <iostream>
#include <iomanip>
#include <vector>
#include "interval.hpp"
//#include "capd/intervals/Interval.h"

using namespace std;
using namespace cxsc;

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
  int size() const { return VecLen( v ); };

  friend ostream& operator<< (ostream &out, const PointBase<T> &p)
  {
	out << p.v;
	return out;
  };
};

typedef PointBase<double> Point;
typedef PointBase< interval > IPoint;

//#include "treeutil.h"			// for the vector operator<< 
#endif

int main()
{ 
  double y,z;
  y = 5.0;
  z = 3.0;


  Point py( y ); 
  Point pz( z );
  IPoint ip( 2 );
  ip[0] = py;
  ip[1] = pz;

  cout << "Interval py=" << py <<  "; pz=" << pz << endl;
  cout << "Ivector ip=" << ip << endl;
  return 0;

}
