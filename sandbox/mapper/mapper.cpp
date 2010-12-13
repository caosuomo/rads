// compile with:
// g++ -I/home/raf/projects/rads/Profil-2.0.8/include/ -L/home/raf/projects/rads/Profil-2.0.8/lib/ profiltest.cpp -lProfil -llr -lBias -o profiltest

#include "Interval.h"
#include "Functions.h"
#include "box.h"
#include "treeutil.h"
#include <math.h>
#include <stdlib.h>

class HenonMapper
{
  vector<double> params;

  template <class T>
  vector<T> map_point(const vector<T> &v)
  {
	vector<T> w(2);
    w[0] = 1 - params[0] * pow(v[0],2) + v[1];
	w[1] = params[1] * v[0];
	return w;
  }

  vector<vector<T> > map_points(const vector<vector<T> > &v)
  {
	vector<vector<T> > w(v.size());
	for (int i=0; i<v.size(); i++)
	  w[i] = map(v[i]);
  }
}

int main()
{
  HenonMapper hm;

  vector<double> v(2);
  cout << "enter x: ";
  cin >> v[0];
  cout << "enter y: ";
  cin >> v[1];

  cout << "henon(" << v << ") = " << hm.map_point(v) << endl;

  vector<INTERVAL> w(2);
  for (int i; i<v.size(); i++)
	w[i] = INTERVAL(v[i]);

  cout << "henon(" << w << ") = " << hm.map_point(w) << endl;
  return 0;
}
