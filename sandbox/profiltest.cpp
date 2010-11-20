// compile with:
// g++ -I/home/raf/projects/rads/Profil-2.0.8/include/ -L/home/raf/projects/rads/Profil-2.0.8/lib/ profiltest.cpp -lProfil -llr -lBias -o profiltest

#include "Interval.h"
#include "Functions.h"
#include <math.h>
#include <stdlib.h>

int main()
{
  double y;
  int i;
  cout << "enter a number: ";
  cin >> y;
  cout << "enter an integer: ";
  cin >> i;
  INTERVAL x(y);
  cout << "number^integer: " << Power(x,i) << endl;
  return 0;
}
