#include "ricker_cpp.h"

RickerMapper::RickerMapper() : Mapper(2,"henon")
{
  params[r] = 5;
  params[d] = 0.1;
  param_names[r] = 'r';
  param_names[d] = 'd';
}

IPoint HenonMapper::map_point(const IPoint &v) const
{
  IPoint w(dim());
  w[0] = 1 - params[A] * power(v[0],2) + v[1];
  w[1] = params[B] * v[0];
  return w;
}
