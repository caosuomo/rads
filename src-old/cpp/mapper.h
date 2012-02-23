#ifndef _mapper_h
#define _mapper_h

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
protected:
  IPoint params;
  vector<string> param_names;
public:
  const int mydim;
  const string myname;
  
  Mapper(int dimension, string mapname)
	: mydim(dimension), myname(mapname),
	  params(dimension), param_names(dimension) {}

  virtual IPoint map_point (const IPoint &v) const = 0;
  BoxSet map_points (const UniformBoxSet &bs) const;
  BoxSet map_points (const BoxSet &bs) const;

  IPoint get_params() { return params; }
  vector<string> get_param_names() { return param_names; }
  void set_params(IPoint p) { params = p; }

  int dim() const { return mydim; }
  string name() const { return myname; }
};

#endif
