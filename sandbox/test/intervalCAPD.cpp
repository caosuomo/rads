#include <iostream>
#include <iomanip>
#include "capd/capdlib.h"
#include "capd/intervals/Interval.h"


typedef capd::intervals::Interval< double > DInterval;
typedef capd::intervals::Interval< int > ZInterval;
//typedef capd::intervals::Interval< capd::MpFloat > MpInterval;

int main()
{

    DInterval a;                   // a is in general not initialized (*)
    DInterval b(1.0);              // b = [1.0, 1.0]
    DInterval c(2.0, 3.0);         // c = [2.0, 3.0]
    DInterval d(c);                // d = [2.0, 3.0]
    DInterval e("2.5","3.0");      // e = [2.5, 3.0]

    // member functions
    c.leftBound();                   // the result is 2.0
    c.rightBound();                  // the result is 3.0

    // or global functions
    leftBound(c);                    // the result is 2.0
    rightBound(c);                   // the result is 3.0

    DInterval x(1.0, 2.0);
    std::cout << std::fixed << std::setprecision(6) << x << std::endl;  // output: [1,2]
    //std::cout << fixed << setprecision(6) << x;      // output: [1.000000,2.000000]

    return 0;
}
