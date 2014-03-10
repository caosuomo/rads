#ifndef _point_h
#define _point_h

#include "interval.hpp"  // CXSC interval lib
#include <iostream>

/* // attempt to circumvent evil macro in some random header */
/* #undef isset */

#include <iomanip>
#include <vector>
#undef isset


//#include "interval.hpp"  // CXSC interval lib
//#include "real.hpp"
//#include "ivector.hpp"

using namespace cxsc;
using namespace std;

template < class T >
class PointBase 
{
  // typedef ivector DVector; 

 public:
  //ivector v;
  vector< T > v;
  PointBase() {};
  PointBase(int size) : v ( size ) {};
  PointBase(int size, const T &t) : v(size,t) {};
  T& operator[] (int i) { return v[i]; };
  void set (int i, const T& t) { v[i]=t; };
  const T& operator[] (int i) const { return v[i]; };
  //int size() const { return VecLen( v ); }; // C-XSC version of dimension
  int size() const { return v.size(); };

    friend ostream& operator<< (ostream &out, const PointBase<T> &p)
    {
      for ( int i=0; i < p.size(); i++ ) {
      //out << p.v[i] << endl;
      out << p.v[i] << endl;
      }
      return out;
      
      //out << p.v;
      //return out;
    };
};

//typedef PointBase< interval > Point;
typedef PointBase< double > Point;
typedef PointBase< interval > IPoint;

#include "treeutil.h"			// for the vector operator<< 
#endif
