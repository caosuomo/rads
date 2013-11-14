#ifndef _point_h
#define _point_h

#include <iostream>
#include <iomanip>
#include <vector>
#include "interval.hpp"  // predefined interval arithmetic
#include "ivector.hpp"

using namespace cxsc;
using namespace std;

template < class T >
class PointBase 
{
  typedef ivector DVector; 

  public:
  DVector v;
  PointBase() {};
  PointBase(int size) : v ( size ) {};
  PointBase(int size, const T &t) : v(size,t) {};
  T& operator[] (int i) { return v[i]; };
  void set (int i, const T& t) { v[i]=t; };
  const T& operator[] (int i) const { return v[i]; };
  int size() const { return VecLen( v ); }; // C-XSC version of dimension
    
    friend ostream& operator<< (ostream &out, const PointBase<T> &p)
    {
      out << p.v;
      return out;
    };
};

typedef interval DInterval;
typedef PointBase< interval > IPoint;
//typedef capd::intervals::Interval< double > DInterval;
//typedef PointBase< capd::intervals::Interval< double > > IPoint;

#include "treeutil.h"			// for the vector operator<< 
#endif

/////////
// FOR DEBUGGING PURPOSES 
/////////
// int main()
// {
//   //typedef capd::intervals::Interval< double > DInterval;

//     double y;
//     int i;
//     cout << "enter a number: ";
//     cin >> y;
//     cout << "enter an integer dimension >= 2: ";
//     cin >> i;
//     cout << "you entered " << y << " and " << i << endl;

//     // initialize an i-dim iinterval vector
//     //PointBase < capd::intervals::Interval < double > > P ( i );

//     IPoint P ( i );

//     cout << "P.v is a " << P.size() 
// 	 << " dimensional interval vector and is intialized as:\n P.v =\n" 
// 	 << P.v << endl;

//     // fill P.v[0]
//     DInterval x0[] = { DInterval( 1., 2. ) };
//     DInterval x1( 3., 4. );

//     interval a,b,c;
//     a = y;
//     b = 3.14;
//     c = 1.1;
    
//     P.v[1] = a;
//     P.v[2] = b;
//     P.set(i,c);

//     cout << "Set entries in P ==> P.v =\n" << P.v << endl;
//     cout << "(The first and last are hardcoded. See the code.)" << endl;
    
//     return 0;

// }
