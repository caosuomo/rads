#include "boxset.h"

ostream &operator<<(ostream &output, const BoxSet &bs)
{
  for (int i=0; i<bs.size(); i++)
	output << bs.get_box(i) << endl;
  return output;
}

ostream &operator<<(ostream &output, const UniformBoxSet &ubs)
{
  output << "width: " << ubs.get_width() << endl;
  output << "corners:" << endl;
  for (int i=0; i<ubs.size(); i++)
	output << ubs.get_corner(i) << endl;
  return output;
}
