// compile with:
// g++ -I/home/raf/projects/rads/Profil-2.0.8/include/ -L/home/raf/projects/rads/Profil-2.0.8/lib/ profiltest.cpp -lProfil -llr -lBias -o profiltest

//#include "Interval.h"
//#include "Functions.h"
#include <math.h>
#include <stdlib.h>
#include "point.h"
#include "capd/capdAlglib.h"
#include "capd/intervals/Interval.h"

int main()
{
  double y;
  int i;
  cout << "enter a number: ";
  cin >> y;
  cout << "enter an integer: ";
  cin >> i;
  IPoint x(y);
  cout << "number^integer: " << i * x << endl;
  return 0;
}
