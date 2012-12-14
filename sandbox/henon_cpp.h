#ifndef _henon_cpp_h
#define _henon_cpp_h

#include "point.h"
#include "capd/capdAlglib.h"
#include "capd/intervals/Interval.h"

typedef capd::intervals::Interval< double > DInterval;
typedef capd::vectalg::Vector<double,2> DVector2D;

DInterval pow ( const IPoint &x, const int &p )
{
    // convert to straight DInterval ???   
    DInterval din( x[0].leftBound(), x[1].rightBound() );
    DInterval dout;
    dout = power( din, p );

    // convert to IPoint
    IPoint rr( 2 );
    rr[0] = dout.leftBound();
    rr[1] = dout.rightBound();

    return dout;
}


class HenonMapper : public Mapper
{
public:
  IPoint a, b;

  HenonMapper() : Mapper(2,"henon"), a(1.4), b(0.3) {}
  //HenonMapper(const int &aa, const string &bb) { a=aa; b=bb; }

  // note: this implements the pure virtual function map_point in the Mapper class
  vector<IPoint> map_point(const vector<IPoint> &v) const
  {
    //vector<IPoint> w(2);
    /* int p = 2; */
    /* DVector2D new_v( v[0].leftBound(), v[0].rightBound() ); */
    /* DVector2D w; */

    /* // convert v to new_v */
    /* new_v[0] = v[0].leftBound(); */
    /* new_v[1] = v[1].rightBound(); */
    
    w[0] = 1 - a * pow( new_v[0], p ) + new_v[1];
    w[1] = b * new_v[0];
    
    /* w[0] = 1 - a * pow( v[0], p ) + v[1]; */
    /* w[1] = b * v[0]; */
    return w;
  }
};

#endif
