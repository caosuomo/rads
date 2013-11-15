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
  //typedef ivector DVector; 

public:

  vector<T> v;
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
    //out << SetPrecision(20,15);
    for ( int i=0; i < p.size(); i++ ) {
      //out << p.v[i] << endl;
      out << p.v[i] << endl;
    }
    return out;
  };

};

typedef PointBase< interval > IPoint;
//typedef PointBase< double > IPoint;

//#include "treeutil.h"			// for the vector operator<< 
#endif

/////////
// FOR DEBUGGING PURPOSES 
/////////
int main()
{
    double y;
    int d;
    cout << "enter a number: ";
    cin >> y;
    cout << "enter an integer dimension >= 2: ";
    cin >> d;
    cout << "you entered " << y << " and " << d << endl;

    // initialize a vector of intervals (an interval point)
    IPoint P ( d );
    
    cout << "P is a " << P.size() 
    	 << " dimensional interval vector and is intialized as:\n P =\n" 
	 << P << endl;

    interval b;
    b = 3.14;
    
    P[1] = y;
    P[2] = b;

    cout << "Set entries in P\n" << P << endl;
    cout << "(The second entry is hardcoded. See the code.)" << endl;
    
    return 0;

}
