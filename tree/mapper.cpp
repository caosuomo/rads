// compile with:
// g++ -I/home/raf/projects/rads/Profil-2.0.8/include/ -L/home/raf/projects/rads/Profil-2.0.8/lib/ profiltest.cpp -lProfil -llr -lBias -o profiltest

#include "Interval.h"
#include "Functions.h"
#include "box.h"
#include "treeutil.h"
#include <math.h>
#include <stdlib.h>
#include <string>
#include "boxset.h"

class Mapper
{
public:
  const int mydim;
  const string myname;
  
  Mapper(int dimension, string mapname) : mydim(dimension), myname(mapname) {}

  virtual IPoint map_point (const IPoint &v) const = 0;

  BoxSet map_points (const UniformBoxSet &bs) const
  {
	BoxSet vals(bs.size());
	Box b;
	b.w = bs.get_width();
	for (int i=0; i<bs.size(); i++)
	{
	  b.v = bs.get_corner(i);
	  vals.set_box(i,Box(map_point(b)));
	}
	return vals;
  }

  BoxSet map_points (const BoxSet &bs) const
  {
	BoxSet vals(bs.size());
	for (int i=0; i<bs.size(); i++)
	{
	  vals.set_box(i,Box(map_point(bs.get_box(i))));
	}
	return vals;
  }

  int dim() const { return mydim; }
  string name() const { return myname; }
};

class HenonMapper : public Mapper
{
public:
  INTERVAL a, b;

  HenonMapper() : Mapper(2,"henon"), a(1.4), b(0.3) {}

  IPoint map_point(const IPoint &v) const
  {
	IPoint w(dim());
    w[0] = 1 - a * Power(v[0],2) + v[1];
	w[1] = b * v[0];
	return w;
  }
};


void maptest(const Mapper &m, int num_pts)
{
  UniformBoxSet ubs(num_pts);
  ubs.set_width(Point(m.dim(),0.1));
  Point v(m.dim());

  for (int t=0; t<num_pts; t++)
  {
	for (int i=0; i<v.size(); i++)
	  v[i] = rand()*1.0/RAND_MAX;
	ubs.set_corner(t,v);
  }

  cout << "mapping " << m.name() << " on points:" << endl << ubs;
  BoxSet vals = m.map_points(ubs);
  cout << "result: " << endl << vals;

  cout << "mapping " << m.name() << " on result:" << endl;
  vals = m.map_points(vals);
  cout << "result: " << endl << vals;
}
	
int main()
{
  HenonMapper henon;
  maptest(henon,10);
}
