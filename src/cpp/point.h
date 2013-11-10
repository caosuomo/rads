#ifndef _point_h
#define _point_h

#include <iostream>
#include <iomanip>
#include <vector>
#include "rinterval.hpp"
/* #include "capd/capdAlglib.h" */
/* #include "capd/intervals/Interval.h" */

using namespace cxsc;
using namespace std;

template < class T >
class PointBase 
{
    // vector of arbitrary length. 
    // below v( dim ) will initialize a n-dim vector of type T  
    // This line defines a vector of type T of arbitrary length.
    //typedef capd::vectalg::Vector < T, 0 > DVector;
  
  typedef ivector < T, 0 > DVector; 


  public:
    DVector v;
    PointBase() {};
    PointBase(int size) : v ( size ) {};
    PointBase(int size, const T &t) : v(size,t) {};
    T& operator[] (int i) { return v[i]; };
    void set (int i, const T& t) { v[i]=t; };
    const T& operator[] (int i) const { return v[i]; };
    int size() const { return v.dimension(); };
    
    friend ostream& operator<< (ostream &out, const PointBase<T> &p)
    {
      out << p.v;
      return out;
    };
};

typedef PointBase < double > Point;
typedef PointBase< interval > IPoint;
//typedef capd::intervals::Interval< double > DInterval;
//typedef PointBase< capd::intervals::Interval< double > > IPoint;

#include "treeutil.h"			// for the vector operator<< 
#endif

/////////
// FOR DEBUGGING PURPOSES 
/////////
int main()
{
  //typedef capd::intervals::Interval< double > DInterval;

    double y;
    int i;
    cout << "enter a number: ";
    cin >> y;
    cout << "enter an integer: ";
    cin >> i;
    cout << "you entered " << y << " and " << i << endl;

    // initialize an i-dim iinterval vector
    //PointBase < capd::intervals::Interval < double > > P ( i );

    IPoint P ( i );

    cout << "P.v is " << i << " dimensional: P.v = " << P.v << endl;

    // fill P.v[0]
    DInterval x0[] = { DInterval( 1., 2. ) };
    DInterval x1( 3., 4. );

    P.v[0] = x0[0];
    P.v[1] = x1;

    cout << "Set entries in P ==> P.v = " << P.v << endl;
    
    return 0;

}
