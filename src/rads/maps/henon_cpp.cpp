#include "henon_cpp.h"

HenonMapper::HenonMapper() : Mapper(2,"henon")
{
  params[A] = 1.4;
  params[B] = 0.3;
  param_names[A] = 'a';
  param_names[B] = 'b';
}

IPoint HenonMapper::map_point(const IPoint &v) const
{
  IPoint w(dim());
  w[0] = 1 - params[A] * Power(v[0],2) + v[1];
  w[1] = params[B] * v[0];
  return w;
}
