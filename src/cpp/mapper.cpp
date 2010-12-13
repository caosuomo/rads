#include "mapper.h"

BoxSet Mapper::map_points (const UniformBoxSet &bs) const
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

BoxSet Mapper::map_points (const BoxSet &bs) const
{
  BoxSet vals(bs.size());
  for (int i=0; i<bs.size(); i++)
  {
	vals.set_box(i,Box(map_point(bs.get_box(i))));
  }
  return vals;
}

// void maptest(const Mapper &m, int num_pts)
// {
//   UniformBoxSet ubs(num_pts);
//   ubs.set_width(Point(m.dim(),0.1));
//   Point v(m.dim());

//   for (int t=0; t<num_pts; t++)
//   {
// 	for (int i=0; i<v.size(); i++)
// 	  v[i] = rand()*1.0/RAND_MAX;
// 	ubs.set_corner(t,v);
//   }

//   cout << "mapping " << m.name() << " on points:" << endl << ubs;
//   BoxSet vals = m.map_points(ubs);
//   cout << "result: " << endl << vals;

//   cout << "mapping " << m.name() << " on result:" << endl;
//   vals = m.map_points(vals);
//   cout << "result: " << endl << vals;
// }
	
// int main()
// {
//   HenonMapper henon;
//   maptest(henon,10);
// }
