// compile with:
// g++ -I/home/raf/projects/rads/Profil-2.0.8/include/ -L/home/raf/projects/rads/Profil-2.0.8/lib/ profiltest.cpp -lProfil -llr -lBias -o profiltest

#include "Interval.h"
#include "Functions.h"
#include "box.h"
#include "treeutil.h"
#include <math.h>
#include <stdlib.h>
#include <string>


template<class T>
class Mapper
{
public:
  const int mydim;
  const string myname;
  
  Mapper(int dimension, string mapname) : mydim(dimension), myname(mapname) {}

  virtual vector<T> map_point (const vector<T> &v) const = 0;

  vector<vector<T> > map_points (const vector<vector<T> > &v) const
  {
	vector<vector<T> > w(v.size());
	for (int i=0; i<v.size(); i++)
	  w[i] = map_point(v[i]);
	return w;
  }

  BoxSet map_points (const UniformBoxSet &bs) const
  {
	vector<vector<T> > w(v.size());
	for (int i=0; i<v.size(); i++)
	  w[i] = map_point(v[i]);
	return w;
  }

  int dim() const { return mydim; }
  string name() const { return myname; }
};

template<class T>
class HenonMapper : public Mapper<T>
{
public:
  T a, b;

  HenonMapper() : Mapper<T>(2,"henon"), a(1.4), b(0.3) {}
  HenonMapper(const T &aa, const T &bb) { a=aa; b=bb; }

  vector<T> map_point(const vector<T> &v) const
  {
	vector<T> w(2);
    w[0] = 1 - a * Power(v[0],2) + v[1];
	w[1] = b * v[0];
	return w;
  }
};


template<class T>
void maptest(const Mapper<T> &m, int trials)
{
  vector<T> v(m.dim());

  for (int t=0; t<trials; t++)
  {
	for (int i=0; i<v.size(); i++)
	  v[i] = rand()*1.0/RAND_MAX;
	cout << m.name() << "(" << v << ") = " << m.map_point(v) << endl;
  }
}
	
int main()
{
  HenonMapper<double> henon_d;
  HenonMapper<INTERVAL> henon_i;
  maptest(henon_d,10);
  maptest(henon_i,10);
  return 0;
}
