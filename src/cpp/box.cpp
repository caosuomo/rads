#include "point.h"
#include "box.h"
#include "treeutil.h"

Box::Box() { }

Box::Box(int dimension)
{
  v = Point(dimension);
  w = Point(dimension);
}

Box::Box(const Point &_v, const Point &_w)
{
  v = _v;
  w = _w;
}

Box::Box(const IPoint &ints) : v(ints.size()), w(ints.size())
{
  for (int i=0; i<size(); i++)
  {
	v[i] = Inf(ints[i]);
	w[i] = Diam(ints[i]);
  }
}

Box::operator IPoint()
{
  IPoint ints(size());
  for (int i=0; i<size(); i++)
	ints[i] = INTERVAL(beg(i),end(i));
  return ints;
}

// shorthand for the relevant points in the box
inline double Box::beg(int i) const { return v[i]; }
inline double Box::mid(int i) const { return v[i]+w[i]/2; }
inline double Box::end(int i) const { return v[i]+w[i]; }
int Box::size() const { return v.size(); }

// convention: the ith coordinate of child number c is
// (v[i],v[i]+w[i]/2) iff the ith bit of c is 0

Box Box::scale_down(int corner) const
{
  Box b(v,w);

  for (int i=0; i<v.size(); i++)
  {
	b.w[i] /= 2;
	// 1 if corner has a 1 in that bit, 0 otherwise
	b.v[i] += b.w[i] * ((corner>>i) & 1);
  }
  return b;
}

Point Box::get_corner(int corner) const
{
  Point corner_vector = v;
  for (int i=0; i<v.size(); i++)
  {
	// 1 if corner has a 1 in that bit, 0 otherwise
	corner_vector[i] += (w[i]/2) * ((corner>>i) & 1);
  }
  return corner_vector;
}

int Box::child_hit(const Point &p) const
{
  int child=0;
  // use bitwise ops to corner the childnum
  for (int i=0; i<v.size(); i++)
	child |=
	  ((1<<i)*interval_contains(mid(i),end(i),p[i]));
  return child;
}

bool Box::intersects(const Box &b) const
{
  for (int i=0; i<v.size(); i++)
	if (!interval_intersect(b.beg(i),b.end(i),beg(i),end(i)))
	  return false;
  return true;
}

bool Box::contains(const Point &p) const
{
  for (int i=0; i<v.size(); i++)
	if (!interval_contains(beg(i),end(i),p[i]))
	  return false;
  return true;
}

vector<int> Box::children_hit(const Box &b) const
{
  // bit vectors
  unsigned int first=0;			// first half
  unsigned int second=0;		// second half

  vector<int> children;
  int num_children = pow(2,v.size());
  children.reserve(num_children);

  for (int i=0; i<v.size(); i++)
  {
	first |= (interval_intersect(b.beg(i),b.end(i),beg(i),mid(i)) << i);
	second |= (interval_intersect(b.beg(i),b.end(i),mid(i),end(i)) << i);
  }

  for (int c=0; c<num_children; c++)
  {
	if ((((~c) & first)	// bits where c is 0 and first half is hit
		 |				// bitwise-or them to get all matching coords
		 (c & second))	// bits where c is 1 and secord half is hit
		==				// matches bit-by-bit to
		(num_children - 1))	// all ones
	  children.push_back(c);
  }
  return children;
}

ostream &operator<<(ostream &output, const Box &b)
{
  output << "[" << b.v << "," << b.w << "]";  
  return output;
}
